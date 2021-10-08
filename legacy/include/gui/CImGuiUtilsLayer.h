#pragma once

#include <gui/CImGuiLayer.h>

#include <utils/CDebugDrawer.h>
#include <input/CInputManager.h>
#include <renderers/CMainRenderer.h>
#include <graphics/CScene.h>
#include <gui/CImGuiManager.h>
#include <gl/CIWindow.h>

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
                if ( _vect_elm->name() == current_name_str ) current_index_int = _index;                \
            }                                                                                           \
            ImGui::EndCombo();                                                                          \
        }

namespace engine
{

    const float32 GUI_UTILS_DRAGFLOAT_POSITION_SPEED = 0.1f;

    class CImGuiUtilsLayer : public CImGuiLayer
    {

    public :

        CImGuiUtilsLayer( const std::string& name,
                          CScene* scene,
                          CMainRenderer* mainRenderer,
                          CRenderOptions* renderOptions,
                          CImGuiManager* imguiManager,
                          CIWindow* window );
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
        void _menuScene();
        void _submenuSceneMeshes();
        void _submenuSceneModels();
        void _submenuSceneLights();
        void _submenuSceneCameras();

        void _submenuMesh( CMesh* mesh, bool refresh );
        void _submenuModel( CModel* model, bool refresh );

        void _submenuTreeTransform( CVec3& position, CMat3& rotation, CVec3& scale );
        void _submenuTreeMaterial( CMaterial* material, bool refresh );

        void _submenuLight( CILight* light, bool refresh );
        void _submenuCamera( CICamera* camera, bool refresh );

        template< typename T >
        std::vector< T* > _collectTypedRenderables( std::vector< CIRenderable* > renderables ) const
        {
            std::vector< T* > _typedRenderables;
            for ( auto renderable : renderables )
                if ( renderable->type() == T::GetStaticType() )
                    _typedRenderables.push_back( dynamic_cast< T* >( renderable ) );

            return _typedRenderables;
        }

    private :

        CScene*             m_scene;
        CMainRenderer*      m_mainRenderer;
        CRenderOptions*     m_renderOptions;
        CMeshRenderer*      m_meshRenderer;
        CSkyboxRenderer*    m_skyboxRenderer;
        CImGuiManager*      m_imguiManager;
        CIWindow*           m_window;

        bool m_wantsToCaptureMouse;
        bool m_cursorDisabledByFpsCamera;
    };

}
