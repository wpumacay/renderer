
#include <gui/CImGuiUtilsLayer.h>

namespace engine
{

    CImGuiUtilsLayer::CImGuiUtilsLayer( const std::string& name,
                                        CScene* scene,
                                        CMainRenderer* mainRenderer,
                                        CRenderOptions* renderOptions,
                                        CImGuiManager* imguiManager )
        : CImGuiLayer( name )
    {
        m_scene = scene;
        m_mainRenderer = mainRenderer;
        m_renderOptions = renderOptions;
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

        if ( ImGui::CollapsingHeader( "Render-options" ) )
        {
            ImGui::Checkbox( "Use-fog", &m_renderOptions->useFog );
            ImGui::Checkbox( "Use-skybox", &m_renderOptions->useSkybox );
            ImGui::Checkbox( "Use-shadows", &m_renderOptions->useShadowMapping );
            ImGui::Checkbox( "Use-blending", &m_renderOptions->useBlending );
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
        auto _meshes = m_scene->collectTypedRenderables< CMesh >();

        ImGui::Begin( "Meshes" );

        if ( _meshes.size() < 1 )
        {
            ImGui::Text( "No meshes in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _currentMeshName = "";
        IMGUI_COMBO_CONSTRUCT( "meshes", _currentMeshName, _meshes );

        if ( m_scene->hasRenderable( _currentMeshName ) )
            _menuMesh( dynamic_cast< CMesh* >( m_scene->getRenderable( _currentMeshName ) ) );

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuMesh( CMesh* mesh )
    {
        auto _visible = mesh->visible();
        ImGui::Checkbox( "Visible", &_visible );
        mesh->setVisibility( _visible );

        if ( !_visible )
            return;

        // position, rotation and scale
        if ( ImGui::TreeNode( "World-transform" ) )
        {
            float32 _vposition[3] = { mesh->position.x, mesh->position.y, mesh->position.z };
            ImGui::InputFloat3( "position", _vposition );
            mesh->position = { _vposition[0], _vposition[1], _vposition[2] };

            auto _euler = CMat4::toEuler( mesh->rotation );
            ImGui::SliderFloat( "euler-x", &_euler.x, -ENGINE_PI, ENGINE_PI );
            ImGui::SliderFloat( "euler-y", &_euler.y, -ENGINE_PI / 2 + 0.001f, ENGINE_PI / 2 - 0.001f );
            ImGui::SliderFloat( "euler-z", &_euler.z, -ENGINE_PI, ENGINE_PI );
            mesh->rotation = CMat4::fromEuler( _euler );

            float32 _vscale[3] = { mesh->scale.x, mesh->scale.y, mesh->scale.z };
            ImGui::SliderFloat3( "scale", _vscale, 0.1f, 10.0f );
            mesh->scale = { _vscale[0], _vscale[1], _vscale[2] };

            ImGui::TreePop();
        }

        // material properties
        if ( ImGui::TreeNode( "Material" ) )
        {
            auto _material = mesh->material();

            /* material type */
            int32 _materialType = 0;
            if ( _material->type() == eMaterialType::LAMBERT ) _materialType = 0;
            else if ( _material->type() == eMaterialType::PHONG ) _materialType = 1;
            else if ( _material->type() == eMaterialType::BLINN_PHONG ) _materialType = 2;

            ImGui::RadioButton( "Lambert", &_materialType, 0 ); ImGui::SameLine();
            ImGui::RadioButton( "Phong", &_materialType, 1 ); ImGui::SameLine();
            ImGui::RadioButton( "Blinn-Phong", &_materialType, 2 ); ImGui::Spacing();

            eMaterialType _mtype = eMaterialType::LAMBERT;
            if ( _materialType == 0 ) _mtype = eMaterialType::LAMBERT;
            else if ( _materialType == 1 ) _mtype = eMaterialType::PHONG;
            else if ( _materialType == 2 ) _mtype = eMaterialType::BLINN_PHONG;

            _material->setType( _mtype );

            /* material color-components */
            float32 _ambient[3] = { _material->ambient.x, _material->ambient.y, _material->ambient.z };
            float32 _diffuse[3] = { _material->diffuse.x, _material->diffuse.y, _material->diffuse.z };
            ImGui::ColorEdit3( "Ambient", _ambient );
            ImGui::ColorEdit3( "Diffuse", _diffuse );
            _material->ambient = { _ambient[0], _ambient[1], _ambient[2] };
            _material->diffuse = { _diffuse[0], _diffuse[1], _diffuse[2] };

            if ( _mtype == eMaterialType::PHONG || _mtype == eMaterialType::BLINN_PHONG )
            {
                float32 _specular[3] = { _material->specular.x, _material->specular.y, _material->specular.z };
                ImGui::ColorEdit3( "Specular", _specular );
                _material->specular = { _specular[0], _specular[1], _specular[2] };

                ImGui::SliderFloat( "Shininess", &_material->shininess, 1.0f, 256.0f );
            }

            /* material textures */
            auto _allTextures = CTextureManager::GetAllCachedTextures();

            // albedo-map
            static std::string _albedoMapName = _material->albedoMap() ? _material->albedoMap()->name() : "none";
            IMGUI_COMBO_CONSTRUCT( "albedo-map", _albedoMapName, _allTextures );

            if ( CTextureManager::HasCachedTexture( _albedoMapName ) )
                _material->setAlbedoMap( CTextureManager::GetCachedTexture( _albedoMapName ) );

            if ( ImGui::Button( "Clear-albedomap" ) )
            {
                _albedoMapName = "none";
                _material->setAlbedoMap( nullptr );
            }

            // specular map
            static std::string _specularMapName  = _material->specularMap() ? _material->specularMap()->name() : "none";
            IMGUI_COMBO_CONSTRUCT( "specular-map", _specularMapName, _allTextures );

            if ( CTextureManager::HasCachedTexture( _specularMapName ) )
                _material->setSpecularMap( CTextureManager::GetCachedTexture( _specularMapName ) );

            if ( ImGui::Button( "Clear-specularmap" ) )
            {
                _specularMapName = "none";
                _material->setSpecularMap( nullptr );
            }

            /* transparency */
            ImGui::Checkbox( "Use-transparency", &_material->transparent );
            if ( _material->transparent )
                ImGui::SliderFloat( "Alpha", &_material->alpha, 0.0f, 1.0f );

            ImGui::TreePop();
        }
    }

    void CImGuiUtilsLayer::_menuSceneModels()
    {
        auto _models = m_scene->collectTypedRenderables< CModel >();

        ImGui::Begin( "Models" );

        if ( _models.size() < 1 )
        {
            ImGui::Text( "No models in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _currentModelName = "";
        IMGUI_COMBO_CONSTRUCT( "models", _currentModelName, _models );

        if ( m_scene->hasRenderable( _currentModelName ) )
            _menuModel( dynamic_cast< CModel* >( m_scene->getRenderable( _currentModelName ) ) );

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuModel( CModel* model )
    {
        // position, rotation and scale (root of the model)
        float32 _vposition[3] = { model->position.x, model->position.y, model->position.z };
        ImGui::InputFloat3( "position", _vposition );
        model->position = { _vposition[0], _vposition[1], _vposition[2] };

        auto _euler = CMat4::toEuler( model->rotation );
        ImGui::SliderFloat( "euler-x", &_euler.x, -ENGINE_PI, ENGINE_PI );
        ImGui::SliderFloat( "euler-y", &_euler.y, -ENGINE_PI / 2 + 0.001f, ENGINE_PI / 2 - 0.001f );
        ImGui::SliderFloat( "euler-z", &_euler.z, -ENGINE_PI, ENGINE_PI );
        model->rotation = CMat4::fromEuler( _euler );

        float32 _vscale[3] = { model->scale.x, model->scale.y, model->scale.z };
        ImGui::SliderFloat3( "scale", _vscale, 0.1f, 10.0f );
        model->scale = { _vscale[0], _vscale[1], _vscale[2] };
    }

    void CImGuiUtilsLayer::_menuSceneLights()
    {
        auto _lights = m_scene->lights();

        ImGui::Begin( "Lights" );

        if ( _lights.size() < 1 )
        {
            ImGui::Text( "No lights in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _currentLightName = "";
        IMGUI_COMBO_CONSTRUCT( "lights", _currentLightName, _lights );

        if ( m_scene->hasLight( _currentLightName ) )
        {
            auto _light = m_scene->getLight( _currentLightName );

            if ( _light->type() == eLightType::DIRECTIONAL )
                _menuLightDirectional( dynamic_cast< CDirectionalLight* >( _light ) );

            else if ( _light->type() == eLightType::POINT )
                _menuLightPoint( dynamic_cast< CPointLight* >( _light ) );

            else if ( _light->type() == eLightType::SPOT )
                _menuLightSpot( dynamic_cast< CSpotLight* >( _light ) );
        }
    }

    void CImGuiUtilsLayer::_menuLightDirectional( CDirectionalLight* light )
    {

    }

    void CImGuiUtilsLayer::_menuLightPoint( CPointLight* light )
    {

    }

    void CImGuiUtilsLayer::_menuLightSpot( CSpotLight* light )
    {

    }


    void CImGuiUtilsLayer::_menuSceneCameras()
    {
        auto _cameras = m_scene->cameras();

        ImGui::Begin( "Cameras" );

        if ( _cameras.size() < 1 )
        {
            ImGui::Text( "No cameras in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _currentCameraName = "";
        IMGUI_COMBO_CONSTRUCT( "cameras", _currentCameraName, _cameras );
    }

}