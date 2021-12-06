## About OS Project

This is a completely experimantal/educational project. I don't intend to follow POSIX, or other standards and common 
practices that may have something to offer because the purpose isn't to make something that works or would work on a large
scale but rather trying out many different designs, paradigms, technologies, techniques that would be just a hurdle to
implement in existing projects.

### Language

Using C with gcc. The reason is definitely the control and simplicity.
I do follow C17 here and there as a guidance but really I plan to use 'gcc' to its every extend including 
extensions so the standard I am using is actually 'gnu17'.

### Build System

This system is built using make, because it gives me more control over the commands. I did, however prepare a patch
transitioning to CMake but I didn't commit it, ultimately because CMake solved all of my problems with make by introducing
a new set of problems. I would actually like to use something different than make but right now I do not see a better
alternative. Having said that, there is an idea to make a build system using a scripting language, definitely going to try
that.

## Building the Project

To build to project you need to first set up a toolchain, there is a script provided that will download and install
'binutils' and 'gcc for i686' under /opt. You can update the variables to install to a different location but you're
going to need to update the 'Makefile' in the project root.

Note that this script will download and "compile" these tools so you will need to install their build dependencies 
first. To do that(on Ubuntu):

```
sudo apt install build-essential wget m4 flex bison
```

Then to set up the toolchain:

```
bash toolchain.sh
```

Once the toolchain is installed, you can just call make in the project root and build the project, the output file will be
named 'os.iso' under build/:

```
make
```
