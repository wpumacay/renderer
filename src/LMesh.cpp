

#include "LMesh.h"
#include "LMeshBuilder.h"

using namespace std;

namespace engine
{


    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals )
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
        rotation = glm::mat4( 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LVec2>& texCoords )
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

        rotation = glm::mat4( 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LInd3>& indices,
                  const vector<LVec2>& texCoords )
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

        rotation = glm::mat4( 1.0f );
    }

    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LInd3>& indices )
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

        rotation = glm::mat4( 1.0f );
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

    glm::mat4 LMesh::getModelMatrix()
    {
        glm::mat4 _model = glm::mat4( 1.0f );

        _model = glm::scale( glm::vec3( scale.x, scale.y, scale.z ) ) * _model;
        _model = rotation * _model;
        _model = glm::translate( glm::vec3( pos.x, pos.y, pos.z ) ) * _model;

        return _model;
    }

    void LMesh::render()
    {
        if ( m_usesIndices )
        {
            m_vertexArray->bind();
            m_indexBuffer->bind();

            glDrawElements( GL_TRIANGLES, 
                            m_indexBuffer->getCount(), 
                            GL_UNSIGNED_INT, 0 );

            m_indexBuffer->unbind();
            m_vertexArray->unbind();
        }
        else
        {
            // cout << "drawing?? " << m_vertices.size() << endl;

            m_vertexArray->bind();

            glDrawArrays( GL_TRIANGLES, 
                          0, 
                          m_vertices.size() );

            m_vertexArray->unbind();
        }

    }

}