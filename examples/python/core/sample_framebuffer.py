#!/usr/bin/env python

import tinyrenderer as tr
import tinymath as tm
import numpy as np
from OpenGL.GL import *

if __name__ == '__main__' :
    app = tr.core.Application()

    fbColorConfig = tr.core.AttachmentConfig()
    fbColorConfig.name                 = 'color_attachment'
    fbColorConfig.attachment           = tr.core.FboAttachment.COLOR
    fbColorConfig.texInternalFormat    = tr.core.TextureFormat.RGB
    fbColorConfig.texFormat            = tr.core.TextureFormat.RGB
    fbColorConfig.texPixelDataType     = tr.core.PixelDataType.UINT_8
    fbColorConfig.texWrapU             = tr.core.TextureWrap.REPEAT
    fbColorConfig.texWrapV             = tr.core.TextureWrap.REPEAT

    fbDepthConfig = tr.core.AttachmentConfig()
    fbDepthConfig.name                 = 'depth_attachment'
    fbDepthConfig.attachment           = tr.core.FboAttachment.DEPTH
    fbDepthConfig.texInternalFormat    = tr.core.TextureFormat.DEPTH
    fbDepthConfig.texFormat            = tr.core.TextureFormat.DEPTH
    fbDepthConfig.texPixelDataType     = tr.core.PixelDataType.UINT_32
    fbDepthConfig.texWrapU             = tr.core.TextureWrap.REPEAT
    fbDepthConfig.texWrapV             = tr.core.TextureWrap.REPEAT

    framebuffer = tr.core.FrameBuffer( app.window().width, app.window().height )
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

    layout = tr.core.VertexBufferLayout( [ [ 'pos'  , tr.core.ElementType.Float2, False ],
                                      [ 'color', tr.core.ElementType.Float3, False ],
                                      [ 'uv'   , tr.core.ElementType.Float2, False ] ] )

    quad_vbuffer = tr.core.VertexBuffer( layout,
                                    tr.core.BufferUsage.STATIC,
                                    quad_buffData.size * quad_buffData.itemsize,
                                    quad_buffData )

    quad_ibuffer = tr.core.IndexBuffer( tr.core.BufferUsage.STATIC, 6, quad_indices )

    quad_varray = tr.core.VertexArray()
    quad_varray.addVertexBuffer( quad_vbuffer )
    quad_varray.setIndexBuffer( quad_ibuffer )

    #### setup a simple scene to render to our target *********************************

    scene = app.scene()

    cameraProjData = tr.core.CameraProjData()
    cameraProjData.projection  = tr.core.CameraProjection.PERSPECTIVE
    cameraProjData.fov         = 45.0
    cameraProjData.aspect      = app.window().aspect
    cameraProjData.zNear       = 0.1
    cameraProjData.zFar        = 100.0

    camera = tr.core.OrbitCamera( 'orbit',
                             [ 4.0, 4.0, 4.0 ],
                             [ 0.0, 0.0, 0.0 ],
                             tr.core.Axis.Z,
                             cameraProjData,
                             app.window().width,
                             app.window().height )
    scene.AddCamera( camera )

    light = tr.core.DirectionalLight( 'directional_1',
                                 [ 0.8, 0.8, 0.8 ],
                                 [ 0.8, 0.8, 0.8 ],
                                 [ 0.3, 0.3, 0.3 ],
                                 [ 0.0, 0.0, -1.0 ] )
    scene.AddLight( light )

    plane = tr.core.MeshBuilder.CreatePlane( 10.0, 10.0 )
    plane.material.ambient  = [ 0.2, 0.3, 0.4 ]
    plane.material.diffuse  = [ 0.2, 0.3, 0.4 ]
    plane.material.specular = [ 0.2, 0.3, 0.4 ]
    scene.AddRenderable( plane )

    box = tr.core.MeshBuilder.CreateBox( 0.25, 0.5, 1.0 )
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
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 5.0, 0.0, 0.0 ], [ 1.0, 0.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 5.0, 0.0 ], [ 0.0, 1.0, 0.0 ] )
        tr.core.DebugDrawer.DrawLine( [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 5.0 ], [ 0.0, 0.0, 1.0 ] )

        if ( tr.core.InputManager.IsKeyDown( tr.core.Keys.KEY_ESCAPE ) ) :
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
        shaderTex2d = tr.core.ShaderManager.GetCachedShader( 'basic2d_textures' )
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