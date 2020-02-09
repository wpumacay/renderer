#!/usr/bin/env python

import tinyrenderer as tr
import numpy as np
from OpenGL.GL import *

if __name__ == '__main__' :
    app = tr.core.Application()

    textureDataJpg = tr.core.TextureManager.GetCachedTextureData( 'img_container' )
    textureDataPng = tr.core.TextureManager.GetCachedTextureData( 'img_smiley' )

    if ( textureDataJpg ) :
        print( 'texture-data-jpg: \n\r{}'.format( textureDataJpg ) )

    if ( textureDataPng ) :
        print( 'texture-data-png: \n\r{}'.format( textureDataPng ) )

    textureContainer = tr.core.TextureManager.GetCachedTexture( 'img_container' )
    assert textureContainer, 'Could not retrieve a valid texture for the sample: container'

    textureSmiley = tr.core.TextureManager.GetCachedTexture( 'img_smiley' )
    assert textureSmiley, 'Could not retrieve a valid texture for the sample: smiley'

    bufferData = np.array( [
     #|    pos    |     color    |  texture  |#
        -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 0.0,
         0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0, 1.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 1.0
    ], dtype = np.float32 )

    indices = np.array( [
        0, 1, 2, # first triangle
        0, 2, 3  # second triangle
    ], dtype = np.uint32 )

    layout = tr.core.VertexBufferLayout( [ [ 'pos', tr.core.ElementType.Float2, False ],
                                      [ 'col', tr.core.ElementType.Float3, False ],
                                      [ 'uv',  tr.core.ElementType.Float2, False ] ] )

    vbuffer = tr.core.VertexBuffer( layout,
                               tr.core.BufferUsage.STATIC,
                               bufferData.size * bufferData.itemsize,
                               bufferData )

    ibuffer = tr.core.IndexBuffer( tr.core.BufferUsage.STATIC, 6, indices )

    varray = tr.core.VertexArray()
    varray.addVertexBuffer( vbuffer )
    varray.setIndexBuffer( ibuffer )

    shader = tr.core.ShaderManager.GetCachedShader( 'basic2d_textures' )

    while app.active() :
        app.begin()

        if ( tr.core.InputManager.IsKeyDown( tr.core.Keys.KEY_ESCAPE ) ) :
            break

        shader.bind()
        varray.bind()
        ## textureContainer.bind()
        textureSmiley.bind()

        if ( varray.indexBuffer() ) :
            glDrawElements( GL_TRIANGLES, varray.indexBuffer().count(), GL_UNSIGNED_INT, None )
        else :
            glDrawArrays( GL_TRIANGLES, 0, 6 )

        textureSmiley.unbind()
        ## textureContainer.unbind()
        varray.unbind()
        shader.unbind()
        app.end()
