
#include <utils/CImguiUiDemo.h>

namespace engine
{

    CImguiUiDemo::CImguiUiDemo( COpenGLContext* context )
        : CImguiUi( context )
    {
        m_currentCachedTextureName = "";
        m_currentCachedTexture = nullptr;

        m_textureToShow = nullptr;
    }

    CImguiUiDemo::~CImguiUiDemo()
    {

    }

    void CImguiUiDemo::showTexture( CTexture* texture,
                                    const CVec2& uv0, const CVec2& uv1 )
    {
        m_textureToShow = texture;
        m_textureToShowUv0 = uv0;
        m_textureToShowUv1 = uv1;
    }

    void CImguiUiDemo::_initInternal()
    {

    }

    void CImguiUiDemo::_renderInternal()
    {
        ImGui::Begin( "Demo-ui" );
        auto _pos = ImGui::GetCursorPos();
        auto _posScreen = ImGui::GetCursorScreenPos();
        auto _cursor = ImGui::GetMousePos();
        ImGui::Text( "pos : (%.3f,%.3f)", _pos.x, _pos.y );
        ImGui::Text( "pos-screen: (%.3f,%.3f)", _posScreen.x, _posScreen.y );
        ImGui::Text( "cursor: (%.3f,%.3f)", _cursor.x, _cursor.y );
        ImGui::End();

        _renderUserTextureMenu();
        _renderCachedTexturesMenu();
    }

    void CImguiUiDemo::_renderCachedTexturesMenu()
    {
        ImGui::Begin( "Texture-manager" );

        auto _cachedTexturesList = CTextureManager::GetAllCachedTextures();

        if ( ImGui::BeginCombo( "Cached-textures", m_currentCachedTextureName.c_str() ) )
        {
            for ( size_t i = 0; i < _cachedTexturesList.size(); i++ )
            {
                auto _cachedTexture = _cachedTexturesList[i];
                bool _isSelected = ( _cachedTexture->name() == m_currentCachedTextureName );

                if ( ImGui::Selectable( _cachedTexture->name().c_str(), _isSelected ) )
                {
                    m_currentCachedTexture = _cachedTexture;
                    m_currentCachedTextureName = _cachedTexture->name();
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( m_currentCachedTexture )
        {
            ImGui::Image( (void*)(intptr_t) m_currentCachedTexture->openglId(),
                          ImVec2( m_currentCachedTexture->width() / 2.0, 
                                  m_currentCachedTexture->height() / 2.0 ) );
        }

        ImGui::End();
    }

    void CImguiUiDemo::_renderUserTextureMenu()
    {
        if ( !m_textureToShow )
            return;

        ImGui::Begin( "User-texture" );

        ImGui::Image( (void*)(intptr_t) m_textureToShow->openglId(),
                      ImVec2( m_textureToShow->width() / 2.0, m_textureToShow->height() / 2.0 ),
                      ImVec2( m_textureToShowUv0.x, m_textureToShowUv0.y ),
                      ImVec2( m_textureToShowUv1.x, m_textureToShowUv1.y ) );
        
        ImGui::End();
    }

}