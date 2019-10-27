
#include <gui/CImGuiUtilsLayer.h>

namespace engine
{

    CImGuiUtilsLayer::CImGuiUtilsLayer( const std::string& name,
                                        CScene* scene,
                                        CMainRenderer* mainRenderer,
                                        CImGuiManager* imguiManager )
        : CImGuiLayer( name )
    {
        m_scene = scene;
        m_mainRenderer = mainRenderer;
        m_meshRenderer = mainRenderer->meshRenderer();
        m_skyboxRenderer = mainRenderer->skyboxRenderer();
        m_imguiManager = imguiManager;
    }

    CImGuiUtilsLayer::~CImGuiUtilsLayer()
    {
        m_scene = nullptr;
        m_mainRenderer = nullptr;
        m_meshRenderer = nullptr;
        m_skyboxRenderer = nullptr;
        m_imguiManager = nullptr;
    }

    void CImGuiUtilsLayer::update()
    {

    }

    void CImGuiUtilsLayer::render()
    {
        m_wantsToCaptureMouse = false;

        /* general options available to the user (docking, offscreen rendering, etc.) */
        _menuGeneralOptions();
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

    void CImGuiUtilsLayer::_menuGeneralOptions()
    {
        ImGui::Begin( "General" );

        if ( ImGui::CollapsingHeader( "Gui" ) )
        {
            if ( ImGui::Button( "Close utils" ) )
                m_active = false;

            bool _usesDockingSpace = m_imguiManager->usesDockingSpace();
            ImGui::Checkbox( "DockingSpace", &_usesDockingSpace );
            m_imguiManager->setDockingSpace( _usesDockingSpace );

            bool _usesPassthrough = m_imguiManager->usesDockingPassthrough();
            ImGui::Checkbox( "DockingSpace-passthrough", &_usesPassthrough );
            m_imguiManager->setDockingSpacePassthrough( _usesPassthrough );
        }

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuStatistics()
    {
        ImGui::Begin( "Stats" );

        if ( ImGui::CollapsingHeader( "Performance" ) )
        {
            ImGui::Text( "Fps-avg       : %.2f", CTime::GetAvgFps() );
            ImGui::Text( "Frametime-avg : %.5f", CTime::GetAvgTimeStep() );
            ImGui::PlotLines( "Fps-avg", 
                              engine::CTime::GetFpsAvgs(), 
                              engine::CTime::GetNumFramesForAvg(), 
                              engine::CTime::GetFrameTimeIndex(),
                              ( std::string( "average: " ) + std::to_string( engine::CTime::GetAvgFps() ) ).c_str(),
                              0.0f, FLT_MAX, ImVec2( 0, 100 ) );
        }

        if ( ImGui::CollapsingHeader( "Scene-summary" ) )
        {
            ENGINE_CORE_ASSERT( m_scene, "Must provide a scene to gui-utils" );

            ImGui::Text( "num-cameras       : %lu", m_scene->cameras().size() );
            ImGui::Text( "num-lights        : %lu", m_scene->lights().size() );
            ImGui::Text( "\tpoint-lights    : %lu", m_scene->pointLights().size() );
            ImGui::Text( "\tdir-light       : %lu", m_scene->directionalLights().size() );
            ImGui::Text( "\tspot-light      : %lu", m_scene->spotLights().size() );
            ImGui::Text( "num-renderables   : %lu", m_scene->renderables().size() );
            ImGui::Text( "skybox            : %s", m_scene->skybox() ? "true" : "false" );
            ImGui::Text( "fog               : %s", m_scene->fog() ? "true" : "false" );
        }

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuSceneMeshes()
    {
        ImGui::Begin( "Meshes" );

        auto _meshes = m_scene->collectTypedRenderables< CMesh >();

        static std::string _currentMeshName = "";
        if ( ImGui::BeginCombo( "meshes", _currentMeshName.c_str() ) )
        {
            for ( auto _mesh : _meshes )
            {
                bool _isSelected = ( _mesh->name() == _currentMeshName );

                if ( ImGui::Selectable( _mesh->name().c_str(), _isSelected ) )
                    _currentMeshName = _mesh->name();

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
        }

        if ( m_scene->hasRenderable( _currentMeshName ) )
            _menuMesh( dynamic_cast< CMesh* >( m_scene->getRenderable( _currentMeshName ) ) );

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuMesh( CMesh* mesh )
    {
        
    }

    void CImGuiUtilsLayer::_menuModel( CModel* model )
    {

    }

    void CImGuiUtilsLayer::_menuSceneModels()
    {
        auto _models = m_scene->collectTypedRenderables< CModel >();
    }

    void CImGuiUtilsLayer::_menuSceneLights()
    {
        auto _lights = m_scene->lights();
    }

    void CImGuiUtilsLayer::_menuSceneCameras()
    {
        auto _cameras = m_scene->cameras();
    }

}