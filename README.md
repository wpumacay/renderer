[![Build Status](https://travis-ci.com/wpumacay/tiny_renderer.svg?branch=master)](https://travis-ci.com/wpumacay/tiny_renderer)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/wpumacay/tiny_renderer/blob/master/LICENSE)

# TinyRenderer: A minimal renderer for prototyping 3D applications

This repo contains the implementation of a basic 3D OpenGL-based renderer. It contains basic abstractions
of resources like VBOs, FBOs, etc., as well as a small OOP-based API to create a scene, in a similar way
to [ThreeJs](https://github.com/mrdoob/three.js/), [BabylonJs](https://github.com/BabylonJS/Babylon.js) 
and [Ogre3D](https://github.com/OGRECave/ogre). Some sample application that can be built with this renderer
are shown below:

![demo-1](https://media.giphy.com/media/SqrtZMytk0O0FnDtex/giphy.gif)

![demo-2](https://media.giphy.com/media/ZDEAQSUraLao0fOhHi/giphy.gif)

## Setup

### Requirements

There are two types of requirements this package uses:

* **Libraries from distribution**: We require `build-essential` or equivalent (make, gcc, g++), `cmake`
  and `pkg-config`. These can be grabbed using your distribution's package manager.

#### Ubuntu >= 16.04

```bash
sudo apt install build-essential cmake pkg-config
sudo apt install libassimp-dev libglfw3-dev libglew-dev
```

#### MacOS

```bash
# toolchain (g++, make) is very likely installed along Xcode, just install cmake and pkg-config using homebrew
brew install cmake pkg-config
# install remaining dependencies
brew install glew glfw assimp 
```

* **Third party repositories**: These are dependencies available through github, which can be grabbed 
  using the provided [setup_dependencies.sh](https://github.com/wpumacay/tiny_renderer/blob/master/scripts/setup_dependencies.sh) 
  script.

```bash
./scripts/setup_dependencies.sh
```

### Building

Use the provided _CMake_ rules:

```bash
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

And that's it. Try some of the examples that were built along the library:

```bash
# make sure you're in the build directory to run the examples
./examples/application/sample_application
```