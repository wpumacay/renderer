#!/usr/bin/env python

import tinyrenderer as tr
import tinymath as tm
import numpy as np
from OpenGL.GL import *

if __name__ == '__main__' :
    app = tr.Application()

    cameraProjData = tr.CameraProjData()
    cameraProjData.projection  = tr.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0

    camera = tr.OrbitCamera( 'orbit',
                             [ 0.0, 0.0, 3.0 ],
                             [ 0.0, 0.0, 0.0 ],
                             tr.Axis.Z,
                             cameraProjData,
                             app.window().width,
                             app.window().height )

    #### cameraSensitivity  = 0.25
    #### cameraSpeed        = 25.0
    #### cameraMaxDelta     = 10.0
    #### camera = tr.FpsCamera( 'fps',
    ####                        [ 0.0, 0.0, 3.0 ],
    ####                        [ 0.0, 0.0, 0.0 ],
    ####                        tr.Axis.Z,
    ####                        cameraProjData,
    ####                        cameraSensitivity,
    ####                        cameraSpeed,
    ####                        cameraMaxDelta )

    box = tr.MeshBuilder.CreateBox( 3.0, 3.0, 3.0 )
    sphere = tr.MeshBuilder.CreateSphere( 1.5 )
    gizmo = tr.MeshBuilder.CreateBox( 0.2, 0.2, 0.2 )
    gizmo.position = [ 0.0, 0.0, 2.0 ]

    # load the shader used for this example
    baseNamePhong = tr.ENGINE_EXAMPLES_PATH + 'lights/shaders/phong'
    shaderPhong = tr.ShaderManager.CreateShaderFromFiles( 'phong_shader',
                                                          baseNamePhong + '_vs.glsl',
                                                          baseNamePhong + '_fs.glsl' )

    baseNameGouraud = tr.ENGINE_EXAMPLES_PATH + 'lights/shaders/gouraud'
    shaderGouraud = tr.ShaderManager.CreateShaderFromFiles( 'gouraud_shader',
                                                            baseNameGouraud + '_vs.glsl',
                                                            baseNameGouraud + '_fs.glsl' )

    assert shaderPhong, 'Could not load phong shader for our tests :('
    assert shaderGouraud, 'Could not load gouraud shader for our tests :('

    # grab a simple shader to render the camera gizmo
    shaderGizmo = tr.ShaderManager.GetCachedShader( "basic3d_no_textures" )
    assert shaderGizmo, 'Could not grab the basic3d shader to render the light gizmo :('

    # select shader to use
    shaderLighting = shaderPhong
    mesh = sphere
    # mesh.position = [ 1.0, 1.0, 0.0 ]
    # mesh.scale = [ 0.5, 1.0, 1.5 ]

    moveLight = False
    mvParam = 0.0

    while( app.active() ) :
        if ( tr.InputManager.IsKeyDown( tr.Keys.KEY_ESCAPE ) ) :
            break
        elif ( tr.InputManager.CheckSingleKeyPress( tr.Keys.KEY_S ) ) :
            shaderLighting = shaderGouraud if ( shaderLighting.name == 'phong_shader' ) else shaderPhong
            print( 'Using shader: {}'.format( shaderLighting.name ) )
        elif ( tr.InputManager.CheckSingleKeyPress( tr.Keys.KEY_P ) ) :
            moveLight = not moveLight
            print( 'Light state: {}'.format( 'moving' if moveLight else 'fixed' ) )

        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        tr.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        app.begin()
        camera.update()

        if ( moveLight ) :
            mvParam += 10.0 * tr.Time.GetTimeStep()
            #### gizmo.position = [ 1.0 + np.sin( mvParam ) * 2.0, np.sin( mvParam / 2.0 ) * 1.0 ]
            gizmo.position = [ 10.0 * np.sin( mvParam ), 10.0 * np.cos( mvParam ), 0.0 ]

        # do our thing here ######################################################
        shaderLighting.bind()
        shaderLighting.setMat4( "u_modelMat", mesh.matModel() )
        shaderLighting.setMat4( "u_viewProjMat", np.dot(camera.matProj(), camera.matView() ) )
        shaderLighting.setMat4( "u_normalMat", tm.inverse( mesh.matModel() ).transpose() )
        shaderLighting.setVec3( "u_objectColor", [ 1.0, 0.5, 0.31 ] )
        shaderLighting.setVec3( "u_lightColor", [ 1.0, 1.0, 1.0 ] )
        shaderLighting.setVec3( "u_lightPosition", gizmo.position )
        shaderLighting.setVec3( "u_viewerPosition", camera.position )

        mesh.render()

        shaderLighting.unbind()

        shaderGizmo.bind()
        shaderGizmo.setMat4( 'u_tModel', gizmo.matModel() )
        shaderGizmo.setMat4( 'u_tView', camera.matView() )
        shaderGizmo.setMat4( 'u_tProj', camera.matProj() )
        shaderGizmo.setVec3( 'u_color', [ 1.0, 1.0, 1.0 ] )

        gizmo.render()

        shaderGizmo.unbind()
        ##########################################################################

        #### tr.DebugDrawer.DrawNormals( mesh, [ 0.0, 0.0, 1.0 ] )
        tr.DebugDrawer.Render( camera )

        app.end()
