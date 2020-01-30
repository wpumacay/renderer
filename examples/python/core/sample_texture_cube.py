#!/usr/bin/env python

import tinyrenderer as tr
import tinymath as tm
import numpy as np
from OpenGL.GL import *

def computeSkyboxCorrectionMat( axis ) :
    if ( axis == tr.Axis.X ) :
        return tm.Matrix4f( tm.rotationZf( -tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    elif ( axis == tr.Axis.Z ) :
        return tm.Matrix4f( tm.rotationXf( tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    else :
        return tm.Matrix4f()

if __name__ == '__main__' :
    app = tr.Application()

    textureCubeDataCloudtop   = tr.TextureManager.GetCachedTextureCubeData( 'cloudtop' )
    textureCubeDataStarfield  = tr.TextureManager.GetCachedTextureCubeData( 'starfield' )
    textureCubeDataSiege      = tr.TextureManager.GetCachedTextureCubeData( 'siege' )

    if ( textureCubeDataCloudtop ) :
        print( textureCubeDataCloudtop )
    
    if ( textureCubeDataStarfield ) :
        print( textureCubeDataStarfield )

    if ( textureCubeDataSiege ) :
        print( textureCubeDataSiege )

    textureCubeCloudtop = tr.TextureManager.GetCachedTextureCube( 'cloudtop' )
    textureCubeStarfield = tr.TextureManager.GetCachedTextureCube( 'starfield' )
    textureCubeSiege = tr.TextureManager.GetCachedTextureCube( 'siege' )
    assert textureCubeCloudtop, 'Could not retrieve valid texture-cube for the sample - cloudtop'
    assert textureCubeStarfield, 'Could not retrieve valid texture-cube for the sample - starfield'
    assert textureCubeSiege, 'Could not retrieve valid texture-cube for the sample - siege'

    shader = tr.ShaderManager.GetCachedShader( 'skybox' )

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

    vbufferLayout = tr.VertexBufferLayout( [ [ 'position', tr.ElementType.Float3, False ] ] )
    vbuffer = tr.VertexBuffer( vbufferLayout, 
                               tr.BufferUsage.STATIC,
                               cubemapVertices.size * cubemapVertices.itemsize, 
                               cubemapVertices )

    varray = tr.VertexArray()
    varray.addVertexBuffer( vbuffer )

    scene = app.scene()

    # make a sample camera
    cameraProjData = tr.CameraProjData()
    cameraProjData.projection  = tr.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0
    print( cameraProjData.aspect )

    camera = tr.OrbitCamera( 'orbit',
                             tm.Vector3f( [ 2.0, 2.0, 2.0 ] ),
                             tm.Vector3f( [ 0.0, 0.0, 0.0 ] ),
                             tr.Axis.Z,
                             cameraProjData,
                             app.window().width,
                             app.window().height )
    print( 'cam-info: \n{}'.format( camera ) )

    scene.addCamera( camera )

    textureCubemap = textureCubeCloudtop
    #### textureCubemap = textureCubeStarfield
    #### textureCubemap = textureCubeSiege

    correctionMat = computeSkyboxCorrectionMat( camera.upAxis() )

    while( app.active() ) :
        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        app.update()

        app.begin()
        app.render()

        if ( tr.InputManager.IsKeyDown( tr.Keys.KEY_ESCAPE ) ) :
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
