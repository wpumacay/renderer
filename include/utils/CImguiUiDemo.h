#pragma once

#include <utils/CImguiUi.h>
#include <core/CTexture.h>
#include <assets/CTextureManager.h>

namespace engine
{

    class CImguiUiDemo : public CImguiUi
    {
    public :
        CImguiUiDemo( COpenGLContext* context );
        ~CImguiUiDemo();

        void showTexture( std::shared_ptr< CTexture > texture,
                          const CVec2& uv0 = { 0.0f, 0.0f }, 
                          const CVec2& uv1 = { 1.0f, 1.0f } );

    protected :

        void _initInternal() override;
        void _renderInternal() override;

    private :

        void _renderCachedTexturesMenu();

        std::string                 m_currentCachedTextureName;
        std::shared_ptr< CTexture > m_currentCachedTexture;

        void _renderUserTextureMenu();

        std::shared_ptr< CTexture > m_textureToShow;
        CVec2 m_textureToShowUv0;
        CVec2 m_textureToShowUv1;
    };

}