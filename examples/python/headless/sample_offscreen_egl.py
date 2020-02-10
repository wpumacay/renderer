#!/usr/bin/env python

import tinyrenderer as tr
import tinymath as tm
import numpy as np
from OpenGL.GL import *
from numpngw import write_apng

from IPython.core.debugger import set_trace

if __name__ == '__main__' :
    windowProps = tr.core.WindowProps()
    windowProps.width = 1024
    windowProps.height = 768
    windowProps.title = 'resizable-application'
    windowProps.clearColor = [ 0.2, 0.2, 0.2, 1.0 ]
    windowProps.resizable = True

    app = tr.core.Application( windowProps )

    scene = tr.core.Scene()

    #### create some lights for the scene ##########################################################
    dirLight = tr.core.DirectionalLight( 'directional',
                                    [ 0.4, 0.4, 0.4 ],
                                    [ 0.8, 0.8, 0.8 ],
                                    [ 0.8, 0.8, 0.8 ],
                                    [ -1.0, -1.0, -1.0 ] )

    pointLight = tr.core.PointLight( 'point',
                                [ 0.4, 0.4, 0.4 ],
                                [ 0.8, 0.8, 0.8 ],
                                [ 0.8, 0.8, 0.8 ],
                                [ 5.0, 5.0, 5.0 ],
                                1.0, 0.0, 0.0 )

    scene.AddLight( dirLight )
    scene.AddLight( pointLight )
    #### create some cameras for the scene #########################################################
    cameraProjData = tr.core.CameraProjData()
    cameraProjData.aspect = app.window().aspect
    cameraProjData.width = 10.0 * app.window().aspect
    cameraProjData.height = 10.0

    fixedCamera = tr.core.FixedCamera( 'fixed',
                                  [ 5.0, 5.0, 5.0 ],
                                  [ 0.0, 0.0, 0.0 ],
                                  tr.core.Axis.Z,
                                  cameraProjData )
    scene.AddCamera( fixedCamera )

    orbitCamera = tr.core.OrbitCamera( 'orbit',
                                  [ 5.0, 5.0, 5.0 ],
                                  [ 0.0, 0.0, 0.0 ],
                                  tr.core.Axis.Z,
                                  cameraProjData,
                                  app.window().width,
                                  app.window().height )
    scene.AddCamera( orbitCamera )

    cameraSensitivity  = 0.1
    cameraSpeed        = 50.0
    cameraMaxDelta     = 10.0
    
    fpsCamera = tr.core.FpsCamera( 'fps',
                              [ 5.0, 5.0, 5.0 ],
                              [ 0.0, 0.0, 0.0 ],
                              tr.core.Axis.Z,
                              cameraProjData,
                              cameraSensitivity,
                              cameraSpeed,
                              cameraMaxDelta )
    scene.AddCamera( fpsCamera )
    #### add some effects like fog and a skybox ####################################################

    #### skybox = tr.core.Skybox()
    #### skybox.setCubemap( tr.core.TextureManager.GetCachedTextureCube( 'cloudtop' ) )
    #### scene.AddSkybox( skybox )

    ################################################################################################
    nWidthSamples = 50
    nDepthSamples = 50
    widthExtent = 10.0
    depthExtent = 10.0
    centerX = widthExtent / 2.0
    centerY = depthExtent / 2.0
    heightData = []
    for i in range( nWidthSamples ) :
        for j in range( nDepthSamples ) :
            x = widthExtent * ( i / nWidthSamples - 0.5 )
            y = depthExtent * ( j / nDepthSamples - 0.5 )
            
            #### z = 10.0 * ( x * x + y * y ) / ( widthExtent * widthExtent + depthExtent * depthExtent )

            u = x * 2.0
            v = y * 2.0
            z = np.cos( np.sqrt( ( u * u + v * v ) ) )
            #### z = 1.0

            heightData.append( z )

    patch = tr.core.MeshBuilder.CreateHeightField( nWidthSamples, nDepthSamples,
                                              widthExtent, depthExtent,
                                              centerX, centerY,
                                              np.array( heightData ), 1.0,
                                              tr.core.Axis.Z )
    patch.material.ambient = [ 0.5, 0.5, 0.5 ]
    patch.material.diffuse = [ 0.5, 0.5, 0.5 ]
    patch.material.specular = [ 0.5, 0.5, 0.5 ]
    patch.material.shininess = 32.0

    floor = tr.core.MeshBuilder.CreatePlane( widthExtent, depthExtent, tr.core.Axis.Z )
    floor.material.ambient = [ 0.3, 0.5, 0.8 ]
    floor.material.diffuse = [ 0.3, 0.5, 0.8 ]
    floor.material.specular = [ 0.3, 0.5, 0.8 ]
    floor.material.shininess = 32.0

    #### texture = tr.core.TextureManager.GetCachedTexture( 'img_grid' )
    #### texture = tr.core.TextureManager.GetCachedTexture( 'img_smiley' )
    texture = tr.core.TextureManager.GetCachedTexture( 'built_in_chessboard' )
    patch.material.albedoMap = texture
    floor.material.albedoMap = texture

    #### create some renderables in our scene ######################################################
    modelpath = tr.core.ENGINE_RESOURCES_PATH + 'models/pokemons/lizardon/lizardon.obj'
    model = tr.core.MeshBuilder.CreateModelFromFile( modelpath )
    model.scale = [ 0.1, 0.1, 0.1 ]
    model.position = [ 0.0, 0.0, 2.0 ]

    scene.AddRenderable( model )
    scene.AddRenderable( patch )
    scene.AddRenderable( floor )
    ################################################################################################

    app.setScene( scene )
    app.setOffScreenRendering( True )

    ## app.renderOptions.useShadowMapping = True
    ## app.renderOptions.shadowMapRangeConfig.type = tr.core.ShadowRangeType.FIXED_USER
    ## app.renderOptions.shadowMapRangeConfig.worldUp = [ 0.0, 1.0, 0.0 ]
    ## app.renderOptions.shadowMapRangeConfig.camera = orbitCamera
    ## app.renderOptions.shadowMapRangeConfig.clipSpaceWidth = 40.0
    ## app.renderOptions.shadowMapRangeConfig.clipSpaceHeight = 40.0
    ## app.renderOptions.shadowMapRangeConfig.clipSpaceDepth = 40.0
    #### app.renderOptions.shadowMapRangeConfig.pointLight = pointLight
    ## app.renderOptions.shadowMapRangeConfig.dirLight = dirLight

    rho = 8.660254037844386 # fixed rho, computed from initial camera position (5,5,5)
    phi = 0.6154797086703873 # fixed phi, computed from initial camera position (5,5,5)
    theta = 0.7853981633974483 # initial rho, computed from initial camera position (5,5,5)

    frames = []
    while ( app.active() ) :
        if ( tr.core.InputManager.IsKeyDown( tr.core.Keys.KEY_ESCAPE ) ) :
            break
        #### else if ( tr.core.InputManager.CheckSingleKeyPress( tr.core.Keys.KEY_G ) )
        ####     app.setGuiActive( !app.guiActive() )
        #### else if ( tr.core.InputManager.CheckSingleKeyPress( tr.core.Keys.KEY_U ) )
        ####     app.setGuiUtilsActive( !app.guiUtilsActive() )

        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        theta += 0.2 * tr.core.Time.GetAvgTimeStep()
        ## theta += 0.2 * tr.core.Time.GetAvgTimeStep() * 50.
        _sphi = np.sin( phi )
        _cphi = np.cos( phi )
        _stheta = np.sin( theta )
        _ctheta = np.cos( theta )

        x = rho * _sphi * _ctheta
        y = rho * _sphi * _stheta
        z = rho * _cphi

        fixedCamera.position = [ x, y, z ]

        app.update()

        app.begin()
        app.render()
        app.end()

        frame = app.renderTarget().read()
        frames.append( frame )

        if ( len( frames ) > 1 * 1 ) :
          break

        print( 'frame {}'.format( len( frames ) ) )

    write_apng( 'sample.png', frames, delay=100 )