#pragma once

#include <gui/CImGuiLayer.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <renderers/CMainRenderer.h>
#include <graphics/CScene.h>
#include <gui/CImGuiManager.h>

#define IMGUI_COMBO_CONSTRUCT( combo_name, current_name_str, vect_container )       \
        if ( ImGui::BeginCombo( combo_name, current_name_str.c_str() ) )            \
        {                                                                           \
            for ( auto _vect_elm : vect_container )                                 \
            {                                                                       \
                bool _isSelected = ( _vect_elm->name() == current_name_str );       \
                                                                                    \
                if ( ImGui::Selectable( _vect_elm->name().c_str(), _isSelected ) )  \
                    current_name_str = _vect_elm->name();                           \
                                                                                    \
                if ( _isSelected )                                                  \
                    ImGui::SetItemDefaultFocus();                                   \
            }                                                                       \
            ImGui::EndCombo();                                                      \
        }

#define IMGUI_COMBO_CONSTRUCT_EXT( combo_name, current_name_str, current_index_int, vect_container )    \
        int _index = -1;                                                                                \
        if ( ImGui::BeginCombo( combo_name, current_name_str.c_str() ) )                                \
        {                                                                                               \
            for ( auto _vect_elm : vect_container )                                                     \
            {                                                                                           \
                bool _isSelected = ( _vect_elm->name() == current_name_str );                           \
                                                                                                        \
                if ( ImGui::Selectable( _vect_elm->name().c_str(), _isSelected ) )                      \
                    current_name_str = _vect_elm->name();                                               \
                                                                                                        \
                if ( _isSelected )                                                                      \
                    ImGui::SetItemDefaultFocus();                                                       \
                _index++;                                                                               \
                                                                                                        \
                if ( _vect_elm->name() == current_name_str ) current_index_int = _index;                                          \
            }                                                                                           \
            ImGui::EndCombo();                                                                          \
        }

namespace engine
{

    class CImGuiUtilsLayer : public CImGuiLayer
    {

    public :

        CImGuiUtilsLayer( const std::string& name,
                          CScene* scene,
                          CMainRenderer* mainRenderer,
                          CRenderOptions* renderOptions,
                          CImGuiManager* imguiManager );
        ~CImGuiUtilsLayer();

        void setScene( CScene* scene ) { m_scene = scene; }
        void setMainRenderer( CMainRenderer* renderer ) { m_mainRenderer = renderer; }
        void setImguiManager( CImGuiManager* imguiManager ) { m_imguiManager = imguiManager; }

        void update() override;
        void render() override;
        bool onEvent( const CInputEvent& event ) override;

    private :

        void _menuGeneralOptions();
        void _menuStatistics();
        void _menuSceneMeshes();
        void _menuSceneModels();
        void _menuSceneLights();
        void _menuSceneCameras();

        void _menuMesh( CMesh* mesh, bool refresh );
        void _menuModel( CModel* model, bool refresh );

        void _submenuTransform( CVec3& position, CMat4& rotation, CVec3& scale );
        void _submenuMaterial( CMaterial* material, bool refresh );

        void _menuLightDirectional( CDirectionalLight* light );
        void _menuLightPoint( CPointLight* light );
        void _menuLightSpot( CSpotLight* light );


    private :

        CScene*             m_scene;
        CMainRenderer*      m_mainRenderer;
        CRenderOptions*     m_renderOptions;
        CMeshRenderer*      m_meshRenderer;
        CSkyboxRenderer*    m_skyboxRenderer;
        CImGuiManager*      m_imguiManager;

        bool m_wantsToCaptureMouse;

    };

}