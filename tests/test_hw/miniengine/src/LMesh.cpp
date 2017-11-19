

#include "../include/LMesh.h"
#include "../include/LShaderManager.h"

#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
//#include <glm/ext.hpp>

namespace miniengine
{



    LMesh::LMesh( const vector<LVec3>& vertices,
                  const vector<LInd3>& indices )
    {

        //#ifdef USE_MODERN_OPENGL

        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;

        m_numVertices = vertices.size();
        m_numIndices  = 3 * indices.size();
        m_vertices = new LVec3[vertices.size()];
        m_indices  = new LInd3[indices.size()];

        memcpy( m_vertices, vertices.data(), sizeof( LVec3 ) * vertices.size() );
        memcpy( m_indices, indices.data(), sizeof( LInd3 ) * indices.size() );

        m_lightingEnabled = false;

        //#else

        //#endif

    }

    LMesh::~LMesh()
    {
        if ( m_vertices != NULL )
        {
            delete[] m_vertices;
            m_vertices = NULL;
        }

        if ( m_indices != NULL )
        {
            delete[] m_indices;
            m_indices = NULL;
        }
    }

    void LMesh::init()
    {
        cout << ":D" << endl;

        glGenBuffers( 1, &m_vbo );
        glGenBuffers( 1, &m_ebo );
        glGenVertexArrays( 1, &m_vao );

        glBindVertexArray( m_vao );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, 
                      sizeof( LVec3 ) * m_numVertices,
                      m_vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      sizeof( LInd3 ) * m_numIndices,
                      m_indices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
        glEnableVertexAttribArray( 0 );

        glBindVertexArray( 0 );

    }

    void LMesh::render( const LRenderInfo& rInfo )
    {
        if ( m_lightingEnabled )
        {
            m_programIndx = LShaderManager::instance->shaderTable[shaders::BASE_SHADER_3D_WITH_LIGHTING];
        }
        else
        {
            m_programIndx = LShaderManager::instance->shaderTable[shaders::BASE_SHADER_3D_NO_LIGHTING];
        }

        LProgram& _program = LShaderManager::instance->getProgram( m_programIndx );

        glUseProgram( _program.id );

        glBindVertexArray( m_vao );

        if ( _program.id != 0 )
        {

            // Set mvp transformations
            GLuint u_tModel = glGetUniformLocation( _program.id, "u_tModel" );
            GLuint u_tView  = glGetUniformLocation( _program.id, "u_tView" );
            GLuint u_tProj  = glGetUniformLocation( _program.id, "u_tProj" );

            glm::mat4 _matModel = glm::mat4( 1.0f );
            _matModel = glm::scale( _matModel, glm::vec3( scale.x, scale.y, scale.z ) );
            _matModel = glm::rotate( _matModel, rot.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
            _matModel = glm::rotate( _matModel, rot.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
            _matModel = glm::rotate( _matModel, rot.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
            _matModel = glm::translate( _matModel, glm::vec3( pos.x, pos.y, pos.z ) );

            glUniformMatrix4fv( u_tModel, 1, GL_FALSE, glm::value_ptr( _matModel ) );
            glUniformMatrix4fv( u_tView, 1, GL_FALSE, glm::value_ptr( rInfo.viewMat ) );
            glUniformMatrix4fv( u_tProj, 1, GL_FALSE, glm::value_ptr( rInfo.projMat ) );

            if ( m_lightingEnabled )
            {
                // TODO: Set material params
            }
            else
            {
                // Just set plain color from ambient component
                GLuint u_color  = glGetUniformLocation( _program.id, "u_color" );

                glUniform4fv( u_color, 1, glm::value_ptr( glm::vec4( m_material.ambient.x,
                                                                     m_material.ambient.y,
                                                                     m_material.ambient.z,
                                                                     1.0f ) ) );
            }

            draw();

            //dump();
        }


        glBindVertexArray( 0 );

        glUseProgram( 0 );
    }

    void LMesh::draw()
    {
        glDrawElements( GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0 );
    }

}