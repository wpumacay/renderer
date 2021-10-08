#!/usr/bin/env python

from tinyrenderer.core import engine
import tinymath as tm
import tinyutils as tu
import numpy as np
import time

if __name__ == '__main__' :
    windowProps = engine.WindowProps()
    windowProps.width = 1024
    windowProps.height = 768
    windowProps.title = 'resizable-application'
    windowProps.clearColor = [ 0.2, 0.2, 0.2, 1.0 ]
    windowProps.resizable = False

    app = engine.Application( windowProps )

    scene = engine.Scene()

    #### create some lights for the scene ##########################################################
    dirLight = engine.DirectionalLight( 'directional',
                                        [ 0.4, 0.4, 0.4 ],
                                        [ 0.8, 0.8, 0.8 ],
                                        [ 0.8, 0.8, 0.8 ],
                                        [ -1.0, -1.0, -1.0 ] )

    pointLight = engine.PointLight( 'point',
                                    [ 0.4, 0.4, 0.4 ],
                                    [ 0.8, 0.8, 0.8 ],
                                    [ 0.8, 0.8, 0.8 ],
                                    [ 5.0, 5.0, 5.0 ],
                                    1.0, 0.0, 0.0 )

    scene.AddLight( dirLight )
    scene.AddLight( pointLight )
    #### create some cameras for the scene #########################################################
    cameraProjData = engine.CameraProjData()
    cameraProjData.aspect = app.window().aspect
    cameraProjData.width = 10.0 * app.window().aspect
    cameraProjData.height = 10.0

    fixedCamera = engine.FixedCamera( 'fixed',
                                      [ 5.0, 5.0, 5.0 ],
                                      [ 0.0, 0.0, 0.0 ],
                                      engine.Axis.Z,
                                      cameraProjData )
    scene.AddCamera( fixedCamera )

    orbitCamera = engine.OrbitCamera( 'orbit',
                                      [ 5.0, 5.0, 5.0 ],
                                      [ 0.0, 0.0, 0.0 ],
                                      engine.Axis.Z,
                                      cameraProjData,
                                      app.window().width,
                                      app.window().height )
    scene.AddCamera( orbitCamera )

    cameraSensitivity  = 0.1
    cameraSpeed        = 50.0
    cameraMaxDelta     = 10.0

    fpsCamera = engine.FpsCamera( 'fps',
                              [ 5.0, 5.0, 5.0 ],
                              [ 0.0, 0.0, 0.0 ],
                              engine.Axis.Z,
                              cameraProjData,
                              cameraSensitivity,
                              cameraSpeed,
                              cameraMaxDelta )
    scene.AddCamera( fpsCamera )
    #### add some effects like fog and a skybox ####################################################

    #### skybox = engine.Skybox()
    #### skybox.setCubemap( engine.TextureManager.GetCachedTextureCube( 'cloudtop' ) )
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

    patch = engine.MeshBuilder.CreateHeightField( nWidthSamples, nDepthSamples,
                                              widthExtent, depthExtent,
                                              centerX, centerY,
                                              np.array( heightData ), 1.0,
                                              engine.Axis.Z )
    patch.material.ambient = [ 0.5, 0.5, 0.5 ]
    patch.material.diffuse = [ 0.5, 0.5, 0.5 ]
    patch.material.specular = [ 0.5, 0.5, 0.5 ]
    patch.material.shininess = 32.0

    floor = engine.MeshBuilder.CreatePlane( widthExtent, depthExtent, engine.Axis.Z )
    floor.material.ambient = [ 0.3, 0.5, 0.8 ]
    floor.material.diffuse = [ 0.3, 0.5, 0.8 ]
    floor.material.specular = [ 0.3, 0.5, 0.8 ]
    floor.material.shininess = 32.0

    #### texture = engine.TextureManager.GetCachedTexture( 'img_grid' )
    #### texture = engine.TextureManager.GetCachedTexture( 'img_smiley' )
    texture = engine.TextureManager.GetCachedTexture( 'built_in_chessboard', '' )
    patch.material.albedoMap = texture
    floor.material.albedoMap = texture

    #### create some renderables in our scene ######################################################
    modelpath = engine.ENGINE_RESOURCES_PATH + 'models/pokemons/lizardon/lizardon.obj'
    model = engine.MeshBuilder.CreateModelFromFile( modelpath )
    model.scale = [ 0.1, 0.1, 0.1 ]
    model.position = [ 0.0, 0.0, 2.0 ]
    model.rotation = tm.rotationZf( np.pi / 2.0 ) * tm.rotationXf( np.pi / 2.0 )

    scene.AddRenderable( model )
    scene.AddRenderable( patch )
    scene.AddRenderable( floor )
    ################################################################################################

    app.setScene( scene )

    app.renderOptions().useShadowMapping = True
    app.renderOptions().shadowMapRangeConfig.type = engine.ShadowRangeType.FIXED_USER
    app.renderOptions().shadowMapRangeConfig.worldUp = [ 0.0, 1.0, 0.0 ]
    app.renderOptions().shadowMapRangeConfig.camera = orbitCamera
    app.renderOptions().shadowMapRangeConfig.clipSpaceWidth = 40.0
    app.renderOptions().shadowMapRangeConfig.clipSpaceHeight = 40.0
    app.renderOptions().shadowMapRangeConfig.clipSpaceDepth = 40.0
    #### app.renderOptions().shadowMapRangeConfig.pointLight = pointLight
    app.renderOptions().shadowMapRangeConfig.dirLight = dirLight

    rho = 8.660254037844386 # fixed rho, computed from initial camera position (5,5,5)
    phi = 0.6154797086703873 # fixed phi, computed from initial camera position (5,5,5)
    theta = 0.7853981633974483 # initial rho, computed from initial camera position (5,5,5)

    while ( app.active() ) :
        tu.Clock.Tick()
        if ( engine.InputManager.IsKeyDown( engine.Keys.KEY_ESCAPE ) ) :
            break

        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        theta += 0.2 * tu.Clock.GetAvgTimeStep()
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
        tu.Clock.Tock()
