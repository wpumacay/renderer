

#include "../../include/renderer/LSceneRenderer.h"
#include "../../include/shader/LShaderManager.h"
#include "../../include/entity/LEntity.h"
#include "../../include/component/LMeshComponent.h"
#include "../../include/graphics/LMesh.h"
#include "../../include/light/LLightDirectional.h"
#include "../../include/light/LLightPoint.h"
#include "../../include/light/LLightSpot.h"

using namespace std;

namespace engine
{


    LSceneRenderer::LSceneRenderer()
    {
        disableLighting();
    }

    LSceneRenderer::~LSceneRenderer()
    {

    }

    void LSceneRenderer::enableLighting()
    {
        m_lightingEnabled = true;
        LShaderManager::INSTANCE->currentShader = LShaderManager::INSTANCE->programs["basic3d_lighting"];
        m_shader = LShaderManager::INSTANCE->currentShader;

        m_uniformProj = glGetUniformLocation( m_shader, "u_tProj" );
        m_uniformView = glGetUniformLocation( m_shader, "u_tView" );
        m_uniformModel = glGetUniformLocation( m_shader, "u_tModel" );

        m_uniformNumDirectionalLights = glGetUniformLocation( m_shader, "u_numDirectionalLights" );
        m_uniformNumPointLights       = glGetUniformLocation( m_shader, "u_numPointLights" );
        m_uniformNumSpotLights        = glGetUniformLocation( m_shader, "u_numSpotLights" );;

        m_uniformViewPos = glGetUniformLocation( m_shader, "u_viewPos" );
        m_uniformGlobalLight = glGetUniformLocation( m_shader, "u_globalAmbientLight" );
    }

    void LSceneRenderer::disableLighting()
    {
        m_lightingEnabled = false;
        LShaderManager::INSTANCE->currentShader = LShaderManager::INSTANCE->programs["basic3d"];
        m_shader = LShaderManager::INSTANCE->currentShader;

        m_uniformProj = glGetUniformLocation( m_shader, "u_tProj" );
        m_uniformView = glGetUniformLocation( m_shader, "u_tView" );
        m_uniformModel = glGetUniformLocation( m_shader, "u_tModel" );
    }

    void LSceneRenderer::begin( LScene* pScene )
    {
        glUseProgram( m_shader );

        glUniformMatrix4fv( m_uniformProj, 1, GL_FALSE, glm::value_ptr( pScene->getProjMatrix() ) );
        glUniformMatrix4fv( m_uniformView, 1, GL_FALSE, glm::value_ptr( pScene->getCurrentCamera()->getViewMatrix() ) );

        if ( m_lightingEnabled )
        {
            auto _viewPos = pScene->getCurrentCamera()->getPosition();
            //cout << "_viewPos: " << _viewPos.toString() << endl;
            glUniform3f( m_uniformViewPos, _viewPos.x, _viewPos.y, _viewPos.z );
            glUniform3f( m_uniformGlobalLight, 1.0f, 1.0f, 1.0f );

            glUniform1i( m_uniformNumDirectionalLights, LLightDirectional::s_count );
            glUniform1i( m_uniformNumPointLights, LLightPoint::s_count );
            glUniform1i( m_uniformNumSpotLights, LLightSpot::s_count );

            auto _lights = pScene->getLights();

            for ( LILight* _light : _lights )
            {
                _light->bind();
            }
        }

    }

    void LSceneRenderer::renderScene( LScene* pScene )
    {
        auto _entities = pScene->getEntities();
        for ( LEntity* _entity : _entities )
        {
            LMeshComponent* _meshComponent = _entity->getComponent<LMeshComponent>();
            LMesh* _mesh = _meshComponent->getMesh();
            glUniformMatrix4fv( m_uniformModel, 1, GL_FALSE, glm::value_ptr( _mesh->getModelMatrix() ) );
            render( _mesh );
        }
    }

    void LSceneRenderer::render( LIRenderable* pRenderable )
    {
        pRenderable->render();
    }

    void LSceneRenderer::end( LScene* pScene )
    {
        if ( m_lightingEnabled )
        {
            auto _lights = pScene->getLights();
            for ( LILight* _light : _lights )
            {
                _light->unbind();
            }
        }        

        glUseProgram( 0 );
    }

}