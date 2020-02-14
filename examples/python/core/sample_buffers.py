#!/usr/bin/env python

from tinyrenderer.core import engine
import numpy as np
from OpenGL.GL import *

def createGeometryVer1NoIndices() :
    bufferData = np.array( [ 
    #|     pos     |    color    |#
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5, -0.5, 0.0, 1.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0
    ], dtype = np.float32 )

    layout = engine.VertexBufferLayout( [ [ 'pos', engine.ElementType.Float2, False ],
                                      [ 'color', engine.ElementType.Float3, False ] ] )

    vbuffer = engine.VertexBuffer( layout,
                               engine.BufferUsage.STATIC,
                               bufferData.size * bufferData.itemsize,
                               bufferData )

    varray = engine.VertexArray()
    varray.addVertexBuffer( vbuffer )
    return varray

def createGeometryVer2NoIndices() :
    bufferPosData = np.array( [
    #|     pos     |#
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5 
    ], dtype = np.float32 )

    bufferColData = np.array( [
    #|      color      |#
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    ], dtype = np.float32 )

    layoutPos = engine.VertexBufferLayout( [ [ 'pos', engine.ElementType.Float2, False ] ] )
    layoutCol = engine.VertexBufferLayout( [ [ 'color', engine.ElementType.Float3, False ] ] )

    vbufferPos = engine.VertexBuffer( layoutPos,
                                  engine.BufferUsage.STATIC,
                                  bufferPosData.size * bufferPosData.itemsize,
                                  bufferPosData )

    vbufferCol = engine.VertexBuffer( layoutCol,
                                  engine.BufferUsage.STATIC,
                                  bufferColData.size * bufferColData.itemsize,
                                  bufferColData )

    varray = engine.VertexArray()
    varray.addVertexBuffer( vbufferPos )
    varray.addVertexBuffer( vbufferCol )

    return varray

def createGeometryVer1WithIndices() :
    bufferData = np.array( [
    #|     pos     |     color      |#
        -0.5, -0.5, 1.0, 0.0, 0.0,
         0.5, -0.5, 0.0, 1.0, 0.0,
         0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5,  0.5, 1.0, 1.0, 1.0
    ], dtype = np.float32 )

    indices = np.array( [
        0, 1, 2, # first triangle
        0, 2, 3  # second triangle
    ], dtype = np.uint32 )

    layout = engine.VertexBufferLayout( [ [ 'pos', engine.ElementType.Float2, False ],
                                      [ 'color', engine.ElementType.Float3, False ] ] )

    vbuffer = engine.VertexBuffer( layout,
                               engine.BufferUsage.STATIC,
                               bufferData.size * bufferData.itemsize,
                               bufferData )

    ibuffer = engine.IndexBuffer( engine.BufferUsage.STATIC, 6, indices )

    varray = engine.VertexArray()
    varray.addVertexBuffer( vbuffer )
    varray.setIndexBuffer( ibuffer )

    return varray

def createGeometryVer2WithIndices() :
    bufferPosData = np.array( [
    #|     pos     |#
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5 
    ], dtype = np.float32 )

    bufferColData = np.array( [
    #|      color      |#
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    ], dtype = np.float32 )

    indices = np.array( [
        0, 1, 2, # first triangle
        0, 2, 3  # second triangle
    ], dtype = np.uint32 )

    layoutPos = engine.VertexBufferLayout( [ [ 'pos', engine.ElementType.Float2, False ] ] )
    layoutCol = engine.VertexBufferLayout( [ [ 'color', engine.ElementType.Float3, False ] ] )

    vbufferPos = engine.VertexBuffer( layoutPos,
                                  engine.BufferUsage.STATIC,
                                  bufferPosData.size * bufferPosData.itemsize,
                                  bufferPosData )

    vbufferCol = engine.VertexBuffer( layoutCol,
                                  engine.BufferUsage.STATIC,
                                  bufferColData.size * bufferColData.itemsize,
                                  bufferColData )

    ibuffer = engine.IndexBuffer( engine.BufferUsage.STATIC, 6, indices )

    varray = engine.VertexArray()
    varray.addVertexBuffer( vbufferPos )
    varray.addVertexBuffer( vbufferCol )
    varray.setIndexBuffer( ibuffer )

    return varray

if __name__ == '__main__' :
    app = engine.Application()

    shader = engine.ShaderManager.GetCachedShader( 'basic2d_no_textures' )

    geometryVAO = createGeometryVer1NoIndices()
    ## geometryVAO = createGeometryVer2NoIndices()
    ## geometryVAO = createGeometryVer1WithIndices()
    ## geometryVAO = createGeometryVer2WithIndices()

    while app.active() :
        app.begin()

        if ( engine.InputManager.IsKeyDown( engine.Keys.KEY_ESCAPE ) ) :
            break

        shader.bind()
        geometryVAO.bind()

        if ( geometryVAO.indexBuffer() ) :
            glDrawElements( GL_TRIANGLES, geometryVAO.indexBuffer().count(), GL_UNSIGNED_INT, None )
        else :
            glDrawArrays( GL_TRIANGLES, 0, 6 )

        geometryVAO.unbind()
        shader.unbind()
        app.end()

####     # check the addresses and str-representation of the objects
####     print( 'vao-py-address: {} \n\robj: {}'.format( hex( id( geometryVAO ) ), geometryVAO ) )
####     for vbo in geometryVAO.vertexBuffers() :
####         print( 'vbo-py-address: {}\n\robj: {}'.format( hex( id( vbo ) ), vbo ) )
####     print( 'ebo-py-address: {} \n\robj: {}'.format( hex( id( geometryVAO.indexBuffer() ) ), geometryVAO.indexBuffer() ) )