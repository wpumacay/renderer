# Assignments of the Computer Graphics course :D

## Dependencies :

* CMake
* GLUT
* GLU
* GLEW -> need to be installed, wip: adding git submodule
* GLM -> included as a submodule, just clone with --recursive

## Cloning it :

Make sure to clone the repo using --recursive

```
git clone --recursive https://github.com/wpumacay/wilbert-pumacay-graficos-2017-2.git
```

This will clone also the dependencies that reside in other repositories, like glm. If you have problems with other libraries, please let me know at wilbert.pumacay@ucsp.edu.pe so that I can add the submodules or fix the CMake files :D. ( Maybe I forgot some preprocessor flag to skip some dependencies )

I intend to add glfw and glew as submodules, as I always run into the issue of installation of shared libraries vs static :(. Better to have the whole dependencies thete.

## To compile :

From the root directory of the project, run :

```
cd [repo_root]
cmake .
make
```

The default configuration can is to use GLUT as window creation system, and
by default uses legacy OpenGL.

To use modern OpenGL, just change the definition variable in the root CMakeLists.txt file. Comment the following 

```
add_definitions( -DUSE_MODERN_OPENGL )
```

## Assignments

WIP -> Adding summary of the project

This part should have a summary of the assignment, I will post it in a few hours if possible.

* [Assignment 1] ( https://github.com/wpumacay/wilbert-pumacay-graficos-2017-2/tree/master/assignments/assignment1 )
