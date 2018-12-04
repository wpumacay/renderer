

#include "LMesh.h"

using namespace std;

namespace engine
{

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals )
        : LIRenderable()
    {
        m_usesIndices = false;

        m_type = RENDERABLE_TYPE_MESH;

        m_vertices = vertices;
        m_normals = normals;

        m_vBuffer = new LVertexBuffer();
        m_vBuffer->setData( sizeof( LVec3 ) * vertices.size(),
                            3, (GLfloat*) vertices.data() );

        m_nBuffer = new LVertexBuffer();
        m_nBuffer->setData( sizeof( LVec3 ) * normals.size(),
                            3, (GLfloat*) normals.data() );

        m_tBuffer = NULL;
        m_indexBuffer = NULL;

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( m_vBuffer, 0 );
        m_vertexArray->addBuffer( m_nBuffer, 1 );

        // Create a default material
        m_material = new LMaterial();

        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LVec2>& texCoords )
        : LIRenderable()
    {
        m_usesIndices = false;

        m_type = RENDERABLE_TYPE_MESH;

        m_vertices = vertices;
        m_normals = normals;
        m_texCoords = texCoords;

        m_vBuffer = new LVertexBuffer();
        m_vBuffer->setData( sizeof( LVec3 ) * vertices.size(),
                            3, (GLfloat*) vertices.data() );

        m_nBuffer = new LVertexBuffer();
        m_nBuffer->setData( sizeof( LVec3 ) * normals.size(),
                            3, (GLfloat*) normals.data() );

        m_tBuffer = new LVertexBuffer();
        m_tBuffer->setData( sizeof( LVec2 ) * texCoords.size(),
                            2, ( GLfloat* ) texCoords.data() );

        m_indexBuffer = NULL;

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( m_vBuffer, 0 );
        m_vertexArray->addBuffer( m_nBuffer, 1 );
        m_vertexArray->addBuffer( m_tBuffer, 2 );

        // Create a default material
        m_material = new LMaterial();

        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LVec2>& texCoords,
                  const vector<LInd3>& indices )
        : LIRenderable()
    {
        m_usesIndices = true;

        m_type = RENDERABLE_TYPE_MESH;

        m_vertices = vertices;
        m_normals = normals;
        m_indices = indices;
        m_texCoords = texCoords;

        m_vBuffer = new LVertexBuffer();
        m_vBuffer->setData( sizeof( LVec3 ) * vertices.size(),
                            3, (GLfloat*) vertices.data() );

        m_nBuffer = new LVertexBuffer();
        m_nBuffer->setData( sizeof( LVec3 ) * normals.size(),
                            3, (GLfloat*) normals.data() );

        m_tBuffer = new LVertexBuffer();
        m_tBuffer->setData( sizeof( LVec2 ) * texCoords.size(),
                            2, ( GLfloat* ) texCoords.data() );

        m_indexBuffer = new LIndexBuffer();
        m_indexBuffer->setData( sizeof( LInd3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( m_vBuffer, 0 );
        m_vertexArray->addBuffer( m_nBuffer, 1 );
        m_vertexArray->addBuffer( m_tBuffer, 2 );

        // Create a default material
        m_material = new LMaterial();

        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LInd3>& indices )
        : LIRenderable()
    {
        m_usesIndices = true;

        m_type = RENDERABLE_TYPE_MESH;

        m_vertices = vertices;
        m_normals = normals;
        m_indices = indices;

        m_vBuffer = new LVertexBuffer();
        m_vBuffer->setData( sizeof( LVec3 ) * vertices.size(),
                            3, (GLfloat*) vertices.data() );

        m_nBuffer = new LVertexBuffer();
        m_nBuffer->setData( sizeof( LVec3 ) * normals.size(),
                            3, (GLfloat*) normals.data() );

        m_tBuffer = NULL;// no textures by default

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( m_vBuffer, 0 );
        m_vertexArray->addBuffer( m_nBuffer, 1 );

        m_indexBuffer = new LIndexBuffer();
        m_indexBuffer->setData( sizeof( LInd3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );

        // Create a default material
        m_material = new LMaterial();

        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LMesh::~LMesh()
    {
        m_vBuffer = NULL;
        m_nBuffer = NULL;
        m_tBuffer = NULL;

        delete m_vertexArray;

        if ( m_indexBuffer != NULL )
        {
            delete m_indexBuffer;
        }
    }

    /* @TODO: Implement another way of using the textures. Currently we ...
              are binding each texture for each render call :( */
    void LMesh::render()
    {
        if ( m_drawAsWireframe )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }

        if ( m_usesIndices )
        {
            for ( LTexture* _texture : m_textures )
            {
                _texture->bind();
            }
            m_vertexArray->bind();
            m_indexBuffer->bind();

            glDrawElements( GL_TRIANGLES, 
                            m_indexBuffer->getCount(), 
                            GL_UNSIGNED_INT, 0 );

            m_indexBuffer->unbind();
            m_vertexArray->unbind();
            for ( LTexture* _texture : m_textures )
            {
                _texture->unbind();
            }
        }
        else
        {
            for ( LTexture* _texture : m_textures )
            {
                _texture->bind();
            }
            m_vertexArray->bind();

            glDrawArrays( GL_TRIANGLES, 
                          0, 
                          m_vertices.size() );

            m_vertexArray->unbind();
            for ( LTexture* _texture : m_textures )
            {
                _texture->unbind();
            }
        }

        if ( m_drawAsWireframe )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

    }

}