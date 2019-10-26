#pragma once

#include <gui/CImGuiLayer.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <renderers/CMainRenderer.h>
#include <graphics/CScene.h>

namespace engine
{

    class CImGuiUtilsLayer : public CImGuiLayer
    {

    public :

        CImGuiUtilsLayer( const std::string& name,
                          CScene* scene,
                          CMainRenderer* mainRenderer,
                          CMeshRenderer* meshRenderer,
                          CSkyboxRenderer* skyboxRenderer );
        ~CImGuiUtilsLayer();

        void update() override;
        void render() override;
        bool onEvent( const CInputEvent& event ) override;

    private :

        void _menuStatistics();
        void _menuSceneMeshes();
        void _menuSceneModels();
        void _menuSceneLights();
        void _menuSceneCameras();

    private :

        CScene*             m_scene;
        CMainRenderer*      m_mainRenderer;
        CMeshRenderer*      m_meshRenderer;
        CSkyboxRenderer*    m_skyboxRenderer;

        bool m_wantsToCaptureMouse;

    };

}