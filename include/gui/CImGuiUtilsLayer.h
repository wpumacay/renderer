#pragma once

#include <gui/CImGuiLayer.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <renderers/CMainRenderer.h>
#include <graphics/CScene.h>
#include <gui/CImGuiManager.h>

namespace engine
{

    class CImGuiUtilsLayer : public CImGuiLayer
    {

    public :

        CImGuiUtilsLayer( const std::string& name,
                          CScene* scene,
                          CMainRenderer* mainRenderer,
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

        void _menuMesh( CMesh* mesh );
        void _menuModel( CModel* model );

    private :

        CScene*             m_scene;
        CMainRenderer*      m_mainRenderer;
        CMeshRenderer*      m_meshRenderer;
        CSkyboxRenderer*    m_skyboxRenderer;
        CImGuiManager*      m_imguiManager;

        bool m_wantsToCaptureMouse;

    };

}