# Define the ANSI escape sequence for red color and reset
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

readonly SCRIPT_DIR=$(dirname "$0")
readonly KERNEL_PATH="${OS_BUILD_DIR}/kernel/kernel"
readonly IMG_FILE="${OS_BUILD_DIR}/limine_image.iso"
readonly LIMINE_DIR="${OS_BUILD_DIR}/limine"
readonly TMP_IMG="/tmp/disk.iso"

if [ ! -e $KERNEL_PATH ]; then
  printf "${RED}Kernel file could not be found!${RESET}\n";
  exit 1;
fi


printf "${GREEN}Creating the image...${RESET}\n";
dd if=/dev/zero of=${TMP_IMG} bs=1M count=0 seek=64
if [ $? -ne 0 ]; then
  printf "${RED}Could not create the tmp image file!${RESET}\n";
  exit 1;
fi

printf "${GREEN}Preparing the image...${RESET}\n";
# Create a GPT partition table.
parted -s ${TMP_IMG} mklabel gpt
if [ $? -ne 0 ]; then
  printf "${RED}Could not create the partition table on tmp image file!${RESET}\n";
  exit 1;
fi
 
# Create an ESP partition that spans the whole disk.
parted -s ${TMP_IMG} mkpart ESP fat32 2048s 100%
if [ $? -ne 0 ]; then
  printf "${RED}Could not format the partition on tmp image file!${RESET}\n";
  exit 1;
fi

parted -s ${TMP_IMG} set 1 esp on
if [ $? -ne 0 ]; then
  printf "${RED}Could not set the partition as ESP on tmp image file!${RESET}\n";
  exit 1;
fi

printf "${GREEN}Preparing the limine...${RESET}\n";
if [ ! -d "${LIMINE_DIR}" ]; then
  git clone --branch=v4.x-branch-binary --depth=1 https://github.com/limine-bootloader/limine.git ${LIMINE_DIR}
  make -C "${LIMINE_DIR}"
fi

printf "${GREEN}Depolying limine on the image...${RESET}\n";
${LIMINE_DIR}/limine-deploy ${TMP_IMG}
if [ $? -ne 0 ]; then
  printf "${RED}Could not deploy limine on tmp image file!${RESET}\n";
  exit 1;
fi

printf "${GREEN}Setting up the loopback device...${RESET}\n";
readonly USED_LOOPBACK=$(sudo losetup -Pf --show ${TMP_IMG})

printf "${GREEN}Formatting the boot partition...${RESET}\n";
sudo mkfs.fat -F 32 ${USED_LOOPBACK}p1

printf "${GREEN}Mounting the boot partition...${RESET}\n";
mkdir -p img_mount
sudo mount ${USED_LOOPBACK}p1 img_mount
 
printf "${GREEN}Setting up files...${RESET}\n";
sudo mkdir -p img_mount/EFI/BOOT
sudo cp -v ${KERNEL_PATH} ${LIMINE_CFG} ${OS_BUILD_DIR}/limine/limine.sys img_mount/
sudo cp -v ${OS_BUILD_DIR}/limine/BOOTX64.EFI img_mount/EFI/BOOT/
 
# Sync system cache and unmount partition and loopback device.
sync
sudo umount img_mount
sudo losetup -d ${USED_LOOPBACK}

cp -v ${TMP_IMG} ${IMG_FILE}
printf "${GREEN}Done!${RESET}\n";
