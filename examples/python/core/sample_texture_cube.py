#!/usr/bin/env python

from tinyrenderer.core import engine
import tinymath as tm
import numpy as np
from OpenGL.GL import *

def computeSkyboxCorrectionMat( axis ) :
    if ( axis == engine.Axis.X ) :
        return tm.Matrix4f( tm.rotationZf( -tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    elif ( axis == engine.Axis.Z ) :
        return tm.Matrix4f( tm.rotationXf( tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    else :
        return tm.Matrix4f()

if __name__ == '__main__' :
    app = engine.Application()

    textureCubeDataCloudtop   = engine.TextureManager.GetCachedTextureCubeData( 'cloudtop' )
    textureCubeDataStarfield  = engine.TextureManager.GetCachedTextureCubeData( 'starfield' )
    textureCubeDataSiege      = engine.TextureManager.GetCachedTextureCubeData( 'siege' )

    if ( textureCubeDataCloudtop ) :
        print( textureCubeDataCloudtop )
    
    if ( textureCubeDataStarfield ) :
        print( textureCubeDataStarfield )

    if ( textureCubeDataSiege ) :
        print( textureCubeDataSiege )

    textureCubeCloudtop = engine.TextureManager.GetCachedTextureCube( 'cloudtop' )
    textureCubeStarfield = engine.TextureManager.GetCachedTextureCube( 'starfield' )
    textureCubeSiege = engine.TextureManager.GetCachedTextureCube( 'siege' )
    assert textureCubeCloudtop, 'Could not retrieve valid texture-cube for the sample - cloudtop'
    assert textureCubeStarfield, 'Could not retrieve valid texture-cube for the sample - starfield'
    assert textureCubeSiege, 'Could not retrieve valid texture-cube for the sample - siege'

    shader = engine.ShaderManager.GetCachedShader( 'skybox' )

    cubemapVertices = np.array( [
            -1.0,  1.0, -1.0,
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,

            -1.0, -1.0,  1.0,
            -1.0, -1.0, -1.0,
            -1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
            -1.0,  1.0,  1.0,
            -1.0, -1.0,  1.0,

             1.0, -1.0, -1.0,
             1.0, -1.0,  1.0,
             1.0,  1.0,  1.0,
             1.0,  1.0,  1.0,
             1.0,  1.0, -1.0,
             1.0, -1.0, -1.0,

            -1.0, -1.0,  1.0,
            -1.0,  1.0,  1.0,
             1.0,  1.0,  1.0,
             1.0,  1.0,  1.0,
             1.0, -1.0,  1.0,
            -1.0, -1.0,  1.0,

            -1.0,  1.0, -1.0,
             1.0,  1.0, -1.0,
             1.0,  1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0,  1.0, -1.0,

            -1.0, -1.0, -1.0,
            -1.0, -1.0,  1.0,
             1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0
    ], dtype = np.float32 )

    vbufferLayout = engine.VertexBufferLayout( [ [ 'position', engine.ElementType.Float3, False ] ] )
    vbuffer = engine.VertexBuffer( vbufferLayout, 
                               engine.BufferUsage.STATIC,
                               cubemapVertices.size * cubemapVertices.itemsize, 
                               cubemapVertices )

    varray = engine.VertexArray()
    varray.addVertexBuffer( vbuffer )

    scene = app.scene()

    # make a sample camera
    cameraProjData = engine.CameraProjData()
    cameraProjData.projection  = engine.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0
    print( cameraProjData.aspect )

    camera = engine.OrbitCamera( 'orbit',
                             tm.Vector3f( [ 2.0, 2.0, 2.0 ] ),
                             tm.Vector3f( [ 0.0, 0.0, 0.0 ] ),
                             engine.Axis.Z,
                             cameraProjData,
                             app.window().width,
                             app.window().height )
    print( 'cam-info: \n{}'.format( camera ) )

    scene.AddCamera( camera )

    textureCubemap = textureCubeCloudtop
    #### textureCubemap = textureCubeStarfield
    #### textureCubemap = textureCubeSiege

    correctionMat = computeSkyboxCorrectionMat( camera.upAxis() )

    while( app.active() ) :
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        app.update()

        app.begin()
        app.render()

        if ( engine.InputManager.IsKeyDown( engine.Keys.KEY_ESCAPE ) ) :
            break

        glDepthFunc( GL_LEQUAL )

        shader.bind()
        shader.setMat4( 'u_tProj', camera.matProj() )
        shader.setMat4( 'u_tView', np.dot( tm.Matrix4f( tm.Matrix3f( camera.matView() ), [ 0.0, 0.0, 0.0 ] ), correctionMat ) )

        textureCubemap.bind()
        varray.bind()

        glDrawArrays( GL_TRIANGLES, 0, 36 )

        varray.unbind()
        textureCubemap.unbind()

        shader.unbind()

        glDepthFunc( GL_LESS )

        app.end()
