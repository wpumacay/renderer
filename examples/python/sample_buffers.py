#!/usr/bin/env python

import tinyrenderer as tr

def createGeometryVer1NoIndices() :
    return None # here we should return a buffer

def createGeometryVer2NoIndices() :
    return None # here we should return a buffer

def createGeometryVer1WithIndices() :
    return None # here we should return a buffer

def createGeometryVer2WithIndices() :
    return None # here we should return a buffer

if __name__ == '__main__' :
    app = tr.Application()

    while app.active() :
        app.begin()

        if ( tr.InputManager.IsKeyDown( tr.Keys.KEY_ESCAPE ) ) :
            break

        # render some stuff here
        # ...

        app.end()