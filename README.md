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
