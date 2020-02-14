#!/usr/bin/env python

from tinyrenderer.core import engine
import numpy as np
from OpenGL.GL import *

if __name__ == '__main__' :
    app = engine.Application()

    fbColorConfig = engine.AttachmentConfig()
    fbColorConfig.name                 = 'color_attachment'
    fbColorConfig.attachment           = engine.FboAttachment.COLOR
    fbColorConfig.texInternalFormat    = engine.TextureFormat.RGB
    fbColorConfig.texFormat            = engine.TextureFormat.RGB
    fbColorConfig.texPixelDataType     = engine.PixelDataType.UINT_8
    fbColorConfig.texWrapU             = engine.TextureWrap.REPEAT
    fbColorConfig.texWrapV             = engine.TextureWrap.REPEAT

    fbDepthConfig = engine.AttachmentConfig()
    fbDepthConfig.name                 = 'depth_attachment'
    fbDepthConfig.attachment           = engine.FboAttachment.DEPTH
    fbDepthConfig.texInternalFormat    = engine.TextureFormat.DEPTH
    fbDepthConfig.texFormat            = engine.TextureFormat.DEPTH
    fbDepthConfig.texPixelDataType     = engine.PixelDataType.UINT_32
    fbDepthConfig.texWrapU             = engine.TextureWrap.REPEAT
    fbDepthConfig.texWrapV             = engine.TextureWrap.REPEAT

    framebuffer = engine.FrameBuffer( app.window().width, app.window().height )
    framebuffer.addAttachment( fbColorConfig )
    framebuffer.addAttachment( fbDepthConfig )

    quad_buffData = np.array( [ 
    ##|  positions |    colors    |   uvs   |##
        -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
         0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
         0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 1.0
    ], dtype = np.float32 )

    quad_indices = np.array( [
        0, 1, 2, 
        0, 2, 3
    ], dtype = np.uint32 )

    layout = engine.VertexBufferLayout( [ [ 'pos'  , engine.ElementType.Float2, False ],
                                      [ 'color', engine.ElementType.Float3, False ],
                                      [ 'uv'   , engine.ElementType.Float2, False ] ] )

    quad_vbuffer = engine.VertexBuffer( layout,
                                    engine.BufferUsage.STATIC,
                                    quad_buffData.size * quad_buffData.itemsize,
                                    quad_buffData )

    quad_ibuffer = engine.IndexBuffer( engine.BufferUsage.STATIC, 6, quad_indices )

    quad_varray = engine.VertexArray()
    quad_varray.addVertexBuffer( quad_vbuffer )
    quad_varray.setIndexBuffer( quad_ibuffer )

    #### setup a simple scene to render to our target *********************************

    scene = app.scene()

    cameraProjData = engine.CameraProjData()
    cameraProjData.projection  = engine.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0

    camera = engine.OrbitCamera( 'orbit',
                             [ 4.0, 4.0, 4.0 ],
                             [ 0.0, 0.0, 0.0 ],
                             engine.Axis.Z,
                             cameraProjData,
                             app.window().width,
                             app.window().height )
    scene.AddCamera( camera )

    light = engine.DirectionalLight( 'directional_1',
                                 [ 0.8, 0.8, 0.8 ],
                                 [ 0.8, 0.8, 0.8 ],
                                 [ 0.3, 0.3, 0.3 ],
                                 [ 0.0, 0.0, -1.0 ] )
    scene.AddLight( light )

    plane = engine.MeshBuilder.CreatePlane( 10.0, 10.0 )
    plane.material.ambient  = [ 0.2, 0.3, 0.4 ]
    plane.material.diffuse  = [ 0.2, 0.3, 0.4 ]
    plane.material.specular = [ 0.2, 0.3, 0.4 ]
    scene.AddRenderable( plane )

    box = engine.MeshBuilder.CreateBox( 0.25, 0.5, 1.0 )
    box.material.ambient  = [ 0.7, 0.5, 0.3 ]
    box.material.diffuse  = [ 0.7, 0.5, 0.3 ]
    box.material.specular = [ 0.7, 0.5, 0.3 ]
    box.position = [ 1.0, 1.0, 1.0 ]
    scene.AddRenderable( box )

    print( 'fbo-config - color_attachment: \n{}'.format( framebuffer.getConfigAttachment( 'color_attachment' ) ) )
    print( 'fbo-config - depth_attachment: \n{}'.format( framebuffer.getConfigAttachment( 'depth_attachment' ) ) )

    print( 'fbo-texture-data - color_attachment: \n{}'.format( framebuffer.getTextureAttachment( 'color_attachment' ).data ) )
    print( 'fbo-texture-data - depth_attachment: \n{}'.format( framebuffer.getTextureAttachment( 'depth_attachment' ).data ) )

    while( app.active() ) :
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        engine.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        if ( engine.InputManager.IsKeyDown( engine.Keys.KEY_ESCAPE ) ) :
            break

        app.update()

        #### Render to a custom target given by our framebuffer ####

        # set the render target to our framebuffer
        framebuffer.bind()

        app.begin()

        # let the renderer to the thing :D
        app.render()

        # release our render targets
        framebuffer.unbind()

        #### Render what's inside our framebuffer into a quad on the screen ####

        glDisable( GL_DEPTH_TEST )
        glClearColor( 0.5, 0.5, 0.5, 1.0 )
        glClear( GL_COLOR_BUFFER_BIT )
        shaderTex2d = engine.ShaderManager.GetCachedShader( 'basic2d_textures' )
        assert shaderTex2d, 'No shader to render our render target texture into a quad :('

        shaderTex2d.bind()
        framebuffer.getTextureAttachment( 'color_attachment' ).bind()
        quad_varray.bind()

        glDrawElements( GL_TRIANGLES, quad_varray.indexBuffer().count(), GL_UNSIGNED_INT, None )

        quad_varray.unbind()
        framebuffer.getTextureAttachment( 'color_attachment' ).unbind()
        shaderTex2d.unbind()
        glEnable( GL_DEPTH_TEST )

        app.end()