# TinyRenderer: A minimal renderer for prototyping 3D applications

[![GitHub license][1]][2]

This repo contains the implementation of a basic 3D OpenGL-based renderer. It
contains basic abstractions of resources like VBOs, FBOs, etc., as well as a
small OOP-based API to create a scene, similarly to [ThreeJs][3], [BabylonJs][4]
and [Ogre3D][5]. Some sample applications that can be built with this renderer
are shown below:

![demo-1][6]

![demo-2][7]

## Setup

We're currently refactoring the whole library in the `dev` branch. Hopefully in
some days we'll have the codebase up and running again. In the meantime, the
`master` branch holds the latest stable version (still not refactored). I'm
sorry in advance for any inconvenience.

[1]: <https://img.shields.io/badge/license-MIT-blue.svg> (badge-mit-license)
[2]: <https://github.com/wpumacay/tiny_renderer/blob/master/LICENSE> (file-mit-license)
[3]: <https://github.com/mrdoob/three.js> (three-js-github-repo)
[4]: <https://github.com/BabylonJS/Babylon.js> (babylon-js-github-repo)
[5]: <https://github.com/OGRECave/ogre> (ogre3d-github-repo)
[6]: <https://media.giphy.com/media/SqrtZMytk0O0FnDtex/giphy.gif> (gif-demo-1)
[7]: <https://media.giphy.com/media/ZDEAQSUraLao0fOhHi/giphy.gif> (gif-demo-2)
