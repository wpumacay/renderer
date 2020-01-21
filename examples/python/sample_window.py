#!/usr/bin/env python

import tinyrenderer as tr

g_terminate = False
g_numClicks = 0

def keyCallback( key : int, action : int ) :
    global g_terminate
    print( 'key: {}, action: {}'.format( key, action ) )
    g_terminate = ( ( key == tr.Keys.KEY_ESCAPE ) and ( action == tr.KeyAction.KEY_PRESSED ) )

def mouseCallback( button : int, action : int, x : float, y : float ) :
    global g_numClicks
    print( 'button: {}, action: {}, x: {}, y: {}'.format( button, action, x, y ) )
    if ( ( button == tr.Mouse.BUTTON_LEFT ) and ( action == tr.MouseAction.BUTTON_PRESSED ) ) :
        g_numClicks += 1
    print( 'num-clicks: {}'.format( g_numClicks ) )

def mouseMoveCallback( x : float, y : float ) :
    print( 'x: {}, y: {}'.format( x, y ) )

def scrollCallback( xOff : float, yOff : float ) :
    print( 'xOff: {}, yOff: {}'.format( xOff, yOff ) )

if __name__ == '__main__' :
    windowProps = tr.WindowProps()
    windowProps.width = 800
    windowProps.height = 600
    windowProps.title = 'sample-window-python'
    windowProps.clearColor = [ 0.2, 0.4, 0.8, 1.0 ]

    windowGL = tr.OpenGLWindow( windowProps )
    windowGL.registerKeyCallback( keyCallback )
    windowGL.registerMouseCallback( mouseCallback )
    windowGL.registerMouseMoveCallback( mouseMoveCallback )
    windowGL.registerScrollCallback( scrollCallback )

    while windowGL.active() :
        windowGL.begin()

        if g_terminate :
            windowGL.requestClose()

        windowGL.end()