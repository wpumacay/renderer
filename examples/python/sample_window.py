#!/usr/bin/env python

import tinyrenderer as tr

def keyCallback( key : int, action : int ) :
    print( 'key: {}, action: {}'.format( key, action ) )

def mouseCallback( button : int, action : int, x : float, y : float ) :
    print( 'button: {}, action: {}, x: {}, y: {}'.format( button, action, x, y ) )

def mouseMoveCallback( x : float, y : float ) :
    print( 'x: {}, y: {}'.format( x, y ) )

def scrollCallback( xOff : float, yOff : float ) :
    print( 'xOff: {}, yOff: {}'.format( xOff, yOff ) )

if __name__ == '__main__' :
    windowProps = tr.WindowProps()
    windowGL = tr.OpenGLWindow( windowProps )
    windowGL.registerKeyCallback( keyCallback )
    windowGL.registerMouseCallback( mouseCallback )
    windowGL.registerMouseMoveCallback( mouseMoveCallback )
    windowGL.registerScrollCallback( scrollCallback )

    while windowGL.active() :
        windowGL.begin()

        # render some stuff here
        # ...

        windowGL.end()