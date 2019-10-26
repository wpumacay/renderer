
#include <gui/CImGuiUtilsLayer.h>

namespace engine
{

    CImGuiUtilsLayer::CImGuiUtilsLayer( const std::string& name,
                                        CScene* scene,
                                        CMainRenderer* mainRenderer,
                                        CMeshRenderer* meshRenderer,
                                        CSkyboxRenderer* skyboxRenderer )
        : CImGuiLayer( name )
    {
        m_scene = scene;
        m_mainRenderer = mainRenderer;
        m_meshRenderer = meshRenderer;
        m_skyboxRenderer = skyboxRenderer;
    }

    CImGuiUtilsLayer::~CImGuiUtilsLayer()
    {
        m_scene = nullptr;
        m_mainRenderer = nullptr;
        m_meshRenderer = nullptr;
        m_skyboxRenderer = nullptr;
    }

    void CImGuiUtilsLayer::update()
    {

    }

    void CImGuiUtilsLayer::render()
    {
        m_wantsToCaptureMouse = false;

        /* show some stats (fps, num-drawables, etc.) */
        _menuStatistics();
        /* render menus related to the scene */
        _menuSceneMeshes();
        _menuSceneModels();
        _menuSceneLights();
        _menuSceneCameras();
        /* render menus related to renderers' information */

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool CImGuiUtilsLayer::onEvent( const CInputEvent& event )
    {
        if ( event.type() == eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;

        return false;
    }

    void CImGuiUtilsLayer::_menuStatistics()
    {

    }

    void CImGuiUtilsLayer::_menuSceneMeshes()
    {

    }

    void CImGuiUtilsLayer::_menuSceneModels()
    {

    }

    void CImGuiUtilsLayer::_menuSceneLights()
    {

    }

    void CImGuiUtilsLayer::_menuSceneCameras()
    {

    }

}