
# Cat1: A small renderer for prototyping 3D applications

This repo contains the implementation of a basic 3D OpenGL-based renderer. It contains basic abstractions
of resources like VBOs, FBOs, etc., as well as a small OOP-based API to create a scene, in a similar way
to [ThreeJs](https://github.com/mrdoob/three.js/), [BabylonJs](https://github.com/BabylonJS/Babylon.js) 
and [Ogre3D](https://github.com/OGRECave/ogre). Some sample application that can be built with this renderer
are shown below:

![demo-1](https://media.giphy.com/media/SqrtZMytk0O0FnDtex/giphy.gif)

![demo-2](https://media.giphy.com/media/ZDEAQSUraLao0fOhHi/giphy.gif)

## Setup

### Requirements

#### Ubuntu >= 16.04

```bash
sudo apt install make cmake pkg-config
sudo apt install libassimp-dev libglfw3-dev libglew-dev
```