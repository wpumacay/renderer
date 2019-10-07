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

        void showTexture( CTexture* texture,
                          const CVec2& uv0 = { 0.0f, 0.0f }, 
                          const CVec2& uv1 = { 1.0f, 1.0f } );

    protected :

        void _initInternal() override;
        void _renderInternal() override;

    private :

        void _renderCachedTexturesMenu();

        CTexture*   m_currentCachedTexture;
        std::string m_currentCachedTextureName;

        void _renderUserTextureMenu();

        CTexture* m_textureToShow;
        CVec2 m_textureToShowUv0;
        CVec2 m_textureToShowUv1;
    };

}