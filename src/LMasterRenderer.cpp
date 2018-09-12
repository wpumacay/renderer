
#include <LMasterRenderer.h>


namespace engine
{

    LMasterRenderer* LMasterRenderer::_INSTANCE = NULL;

    LMasterRenderer* LMasterRenderer::GetInstance()
    {
        if ( LMasterRenderer::_INSTANCE == NULL )
        {
            LMasterRenderer::_INSTANCE = new LMasterRenderer();
        }

        return LMasterRenderer::_INSTANCE;
    }

    LMasterRenderer::LMasterRenderer()
    {
        m_meshRenderer = new LMeshRenderer();
    }

    LMasterRenderer::~LMasterRenderer()
    {
        if ( m_meshRenderer != NULL )
        {
            delete m_meshRenderer;
            m_meshRenderer = NULL;
        }

        LMasterRenderer::_INSTANCE = NULL;
    }

    void LMasterRenderer::render( LScene* pScene )
    {
        if ( m_meshRenderer != NULL )
        {
            m_meshRenderer->begin( pScene );
            m_meshRenderer->renderScene( pScene );
            m_meshRenderer->end( pScene );
        }
    }
}