

#include "../../include/graphics/LMesh.h"
#include "../../include/graphics/LMeshBuilder.h"

using namespace std;

namespace engine
{


    LMesh::LMesh( const vector<LVec3>& vertices, 
                  const vector<LVec3>& normals,
                  const vector<LInd3>& indices )
    {
        type = "base";

        auto _vBuffer = new LVertexBuffer();
        _vBuffer->setData( sizeof( LVec3 ) * vertices.size(),
                           3, (GLfloat*) vertices.data() );

        auto _nBuffer = new LVertexBuffer();
        _nBuffer->setData( sizeof( LVec3 ) * normals.size(),
                           3, (GLfloat*) normals.data() );

        m_vertexArray = new LVertexArray();
        m_vertexArray->addBuffer( _vBuffer, 0 );
        m_vertexArray->addBuffer( _nBuffer, 1 );

        m_indexBuffer = new LIndexBuffer();
        m_indexBuffer->setData( sizeof( LInd3 ) * indices.size(), 
                                3 * indices.size(), (GLuint*) indices.data() );

        // Create a default material
        m_material = new LMaterial();

        scale = LVec3( 1.0f, 1.0f, 1.0f );

        drawAsWireframe = false;

        rotation = glm::mat4( 1.0f );
    }

    LMesh::~LMesh()
    {
        delete m_vertexArray;
        delete m_indexBuffer;
        delete m_material;
    }

    void LMesh::setMaterial( LMaterial* pMaterial )
    { 
        delete m_material;
        m_material = pMaterial;
    }

    void LMesh::setTexCoords( const vector<LVec2>& texCoords )
    {
        auto _tBuffer = new LVertexBuffer();
        _tBuffer->setData( sizeof( LVec2 ) * texCoords.size(),
                           2, ( GLfloat* ) texCoords.data() );

        m_vertexArray->addBuffer( _tBuffer, 2 );
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

        if ( drawAsWireframe )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }

        m_vertexArray->bind();
        m_indexBuffer->bind();

        m_material->bind();

        glDrawElements( GL_TRIANGLES, 
                        m_indexBuffer->getCount(), 
                        GL_UNSIGNED_INT, 0 );

        m_material->unbind();

        m_indexBuffer->unbind();
        m_vertexArray->unbind();

        if ( drawAsWireframe )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

    }

}