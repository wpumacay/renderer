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

        void showTexture( std::shared_ptr< CTexture > texture );

    protected :

        void _initInternal() override;
        void _renderInternal() override;

    private :

        void _renderCachedTexturesMenu();

        std::string                 m_currentCachedTextureName;
        std::shared_ptr< CTexture > m_currentCachedTexture;

        void _renderUserTextureMenu();

        std::shared_ptr< CTexture > m_textureToShow;
    };

}