#!/usr/bin/env python

import tinyrenderer as tr
import tinymath as tm
import numpy as np
from OpenGL.GL import *

def computeSkyboxCorrectionMat( axis ) :
    if ( axis == tr.core.Axis.X ) :
        return tm.Matrix4f( tm.rotationZf( -tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    elif ( axis == tr.core.Axis.Z ) :
        return tm.Matrix4f( tm.rotationXf( tm.pi / 2.0 ), tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )
    else :
        return tm.Matrix4f()

if __name__ == '__main__' :
    app = tr.core.Application()

    textureCubeDataCloudtop   = tr.core.TextureManager.GetCachedTextureCubeData( 'cloudtop' )
    textureCubeDataStarfield  = tr.core.TextureManager.GetCachedTextureCubeData( 'starfield' )
    textureCubeDataSiege      = tr.core.TextureManager.GetCachedTextureCubeData( 'siege' )

    if ( textureCubeDataCloudtop ) :
        print( textureCubeDataCloudtop )
    
    if ( textureCubeDataStarfield ) :
        print( textureCubeDataStarfield )

    if ( textureCubeDataSiege ) :
        print( textureCubeDataSiege )

    textureCubeCloudtop = tr.core.TextureManager.GetCachedTextureCube( 'cloudtop' )
    textureCubeStarfield = tr.core.TextureManager.GetCachedTextureCube( 'starfield' )
    textureCubeSiege = tr.core.TextureManager.GetCachedTextureCube( 'siege' )
    assert textureCubeCloudtop, 'Could not retrieve valid texture-cube for the sample - cloudtop'
    assert textureCubeStarfield, 'Could not retrieve valid texture-cube for the sample - starfield'
    assert textureCubeSiege, 'Could not retrieve valid texture-cube for the sample - siege'

    shader = tr.core.ShaderManager.GetCachedShader( 'skybox' )

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

    vbufferLayout = tr.core.VertexBufferLayout( [ [ 'position', tr.core.ElementType.Float3, False ] ] )
    vbuffer = tr.core.VertexBuffer( vbufferLayout, 
                               tr.core.BufferUsage.STATIC,
                               cubemapVertices.size * cubemapVertices.itemsize, 
                               cubemapVertices )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbuffer )

    scene = app.scene()

    # make a sample camera
    cameraProjData = tr.core.CameraProjData()
    cameraProjData.projection  = tr.core.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0
    print( cameraProjData.aspect )

    camera = tr.core.OrbitCamera( 'orbit',
                             tm.Vector3f( [ 2.0, 2.0, 2.0 ] ),
                             tm.Vector3f( [ 0.0, 0.0, 0.0 ] ),
                             tr.core.Axis.Z,
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
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        app.update()

        app.begin()
        app.render()

        if ( tr.core.InputManager.IsKeyDown( tr.core.Keys.KEY_ESCAPE ) ) :
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
