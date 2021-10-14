readonly ROOT="build/toolchain"

readonly DOWNLOAD_DIR=$ROOT/"Downloads"

readonly PREFIX=$PWD/$ROOT
readonly TARGET="i686-elf"

# Path should be updated manually in every script
# The location is build/toolchain/bin
PATH=$PREFIX/bin:$PATH

############## Tool data ##############
readonly TOOL_LIST_NAME=(binutils gcc)
readonly TOOL_LIST_LINK=("https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz"
                         "https://mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-11.2.0/gcc-11.2.0.tar.gz")
readonly TOOL_LIST_FUNC=(__make_binutils __make_gcc)

# these functions should be called from appropriate folders, so cd/pushd calls have to be done prior
__make_binutils() {
  mkdir -p build && cd build \
  && ../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror \
  && make && make install
}

__make_gcc() {
  mkdir -p build && cd build \
  && ../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers\
  && make all-gcc \
  && make all-target-libgcc \
  && make install-gcc \
  && make install-target-libgcc
}

pushd() {
  command pushd "$@" > /dev/null
  return $?
}

popd() {
  command popd "$@" > /dev/null
  return $?
}

__initialize() {
  mkdir -p $ROOT
  mkdir -p $DOWNLOAD_DIR
}

__download_sources() {
  pushd $DOWNLOAD_DIR

  for i in ${!TOOL_LIST_NAME[@]}; do
    local filename=$(basename ${TOOL_LIST_LINK[$i]})

    if [ ! -f $filename  ]; then
      wget ${TOOL_LIST_LINK[$i]}
    fi
  done

  popd
}

__unpack_sources() {
  pushd $DOWNLOAD_DIR

  for i in ${!TOOL_LIST_NAME[@]}; do
    local filename=$(basename ${TOOL_LIST_LINK[$i]})
    local dirname=$(basename -s ".tar.gz" $filename)

    if [ ! -d  $dirname ]; then
      if tar xf $filename; then
        echo "Unpacked " $filename
      fi
    fi
  done

  popd
}

__compile_sources() {
  pushd $DOWNLOAD_DIR

  for i in ${!TOOL_LIST_NAME[@]}; do
    local filename=$(basename ${TOOL_LIST_LINK[$i]})
    local dirname=$(basename -s ".tar.gz" $filename)

    if [ -d  $dirname ]; then
      pushd $dirname

      if ${TOOL_LIST_FUNC[$i]}; then
        echo "Installed " ${TOOL_LIST_NAME[$i]} " to " $PREFIX
      else
        echo "Complitaion/installation failed for " ${TOOL_LIST_NAME[$i]}
      fi

      popd
    else
      echo "The directory " $dirname "not found!"
      return 1
    fi
  done

  popd
}

__initialize
__download_sources
__unpack_sources
__compile_sources
