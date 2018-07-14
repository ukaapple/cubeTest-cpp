OpenGL test with OpenGL 4.1 using Instancing
============================================

Features
========

- Create 1 million cube with OpenGL 4.1 using Instancing, and display fps.
- Using glfw and glew

Checked environment
===================

- Windows 7
- Ubuntu 18.04
- macOS High Sierra 10.13.5 (MacBook Pro Retina, 15-inch Mid 2015)

Required libraries
==================

- for Ubuntu

libgdiplus

```
$ sudo apt-get update
$ sudo apt-get install libgdiplus
```

libgbm-dev

```
$ sudo apt-get install libgbm-dev
```

libEGL

```
$ sudo apt-get install libegl1-mesa-dev
```

glfw

```
$ sudo apt-get install libglfw3-dev
```

glew

```
$ sudo apt-get install libglew-dev
```

- for mac

X11 from https://www.xquartz.org and create symbolic link

```
ln -s /opt/X11/include/X11 /usr/local/include/X11
```

libgdiplus
(Errors and warnings may occured. You need to respond flexibly)

```
$ brew install autoconf 
$ brew install pkg-config 
$ brew install readline 
$ brew install automake 
$ brew install gettext 
$ brew install glib 
$ brew install intltool 
$ brew install libtool 
$ brew install cairo
$ brew install jpeg
$ brew install libtiff
$ brew install glfw
$ brew install glew
$ git clone https://github.com/mono/libgdiplus.git
$ cd libgdiplus
$ CPPFLAGS="-I/usr/local/opt/libpng12/include -I/opt/X11/include" LDFLAGS="-L/usr/local/opt/libpng12/lib -L/usr/X11/lib" ./autogen.sh
$ ./configure
$ make && make install
```

- for windows

1. Install MinGW, CMake and Make.

2. Download glfw from http://www.glfw.org and compile, then copy libglfw3.a to MinGW/lib directory.

3. Download glew from http://glew.sourceforge.net and compile, then copy libglew32.a and libglew32.dll.a to MinGW/lib folder. And copy glew32.dll to a directory that is in your PATH.