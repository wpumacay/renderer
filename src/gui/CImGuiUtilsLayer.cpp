
#include <gui/CImGuiUtilsLayer.h>

namespace engine
{

    CImGuiUtilsLayer::CImGuiUtilsLayer( const std::string& name,
                                        CScene* scene,
                                        CMainRenderer* mainRenderer,
                                        CRenderOptions* renderOptions,
                                        CImGuiManager* imguiManager,
                                        CIWindow* window )
        : CImGuiLayer( name )
    {
        m_scene = scene;
        m_mainRenderer = mainRenderer;
        m_renderOptions = renderOptions;
        m_meshRenderer = mainRenderer->meshRenderer();
        m_skyboxRenderer = mainRenderer->skyboxRenderer();
        m_imguiManager = imguiManager;
        m_window = window;

        m_wantsToCaptureMouse = false;
        m_cursorDisabledByFpsCamera = false;
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
        if ( !m_scene )
            return;

        if ( m_scene->HasCurrentCamera() )
        {
            auto _currentCamera = m_scene->GetCurrentCamera();
            auto _cameraType = _currentCamera->type();
            m_cursorDisabledByFpsCamera = ( _cameraType == eCameraType::FPS ) ? _currentCamera->active() : false;

            // restore cursor if disabled by fps camera
            if ( m_cursorDisabledByFpsCamera && CInputManager::IsKeyDown( engine::Keys::KEY_SPACE ) )
            {
                m_window->enableCursor();
                m_cursorDisabledByFpsCamera = false;
                _currentCamera->SetActiveMode( false );
            }
        }

        // draw some gizmos for the lights
        auto _lights = m_scene->GetLightsList();
        for ( auto _light : _lights )
            if ( _light->type() != eLightType::DIRECTIONAL )
                CDebugDrawer::DrawBox( { 0.1f, 0.1f, 0.1f }, engine::translation( _light->position ), { 1.0f, 1.0f, 1.0f } );

        // draw some gizmos for the cameras
        auto _cameras = m_scene->GetCamerasList();
        for ( auto _camera : _cameras )
        {
            auto _projData = _camera->proj_data();
            if ( _camera->name() == m_scene->GetCurrentCamera()->name() )
                continue;

            if ( _camera->proj_data().projection == eCameraProjection::PERSPECTIVE )
                CDebugDrawer::DrawClipVolume( engine::perspective( _projData.fov, _projData.aspect, 0.01f, 0.2f ) * _camera->mat_view(), CVec3( 0.0f, 1.0f, 1.0f ) );
            else
                CDebugDrawer::DrawBox( CVec3( 0.1f, 0.1f, 0.1f ), tinymath::inverse( _camera->mat_view() ), CVec3( 0.0f, 1.0f, 1.0f ) );
        }
    }

    void CImGuiUtilsLayer::render()
    {
        m_wantsToCaptureMouse = false;

        /* general options available to the user (docking, offscreen rendering, etc.) */
        _menuGeneralOptions();
        /* show some stats (fps, num-drawables, etc.) */
        _menuStatistics();
        /* show configuration|information of the scene */
        _menuScene();

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

            bool _usesPassthrough = m_imguiManager->usesDockingSpacePassthrough();
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
            ImGui::Text( "Fps-avg       : %.2f", tinyutils::Clock::GetAvgFps() );
            ImGui::Text( "Frametime-avg : %.5f", tinyutils::Clock::GetAvgTimeStep() );
            ImGui::PlotLines( "Fps-avg", 
                              tinyutils::Clock::GetFpsBuffer().data(), 
                              NUM_FRAMES_FOR_AVG, 
                              tinyutils::Clock::GetTimeIndex(),
                              ( std::string( "average: " ) + std::to_string( tinyutils::Clock::GetAvgFps() ) ).c_str(),
                              0.0f, FLT_MAX, ImVec2( 0, 100 ) );
        }

        if ( m_scene && ImGui::CollapsingHeader( "Scene-summary" ) )
        {
            ENGINE_CORE_ASSERT( m_scene, "Must provide a scene to gui-utils" );

            ImGui::Text( "num-cameras       : %lu", m_scene->GetNumCameras() );
            ImGui::Text( "num-lights        : %lu", m_scene->GetNumLights() );
            ImGui::Text( "num-renderables   : %lu", m_scene->GetNumRenderables() );
            ImGui::Text( "skybox            : %s", m_scene->HasCurrentSkybox() ? "true" : "false" );
            ImGui::Text( "fog               : %s", m_scene->HasCurrentFog() ? "true" : "false" );
        }

        ImGui::End();
    }

    void CImGuiUtilsLayer::_menuScene()
    {
        if ( !m_scene )
            return;

        ImGui::Begin( "Scene" );

        _submenuSceneMeshes();
        _submenuSceneModels();
        _submenuSceneLights();
        _submenuSceneCameras();

        ImGui::End();
    }

    void CImGuiUtilsLayer::_submenuSceneMeshes()
    {
        if ( !ImGui::CollapsingHeader( "Meshes" ) )
            return;

        auto _meshes = _collectTypedRenderables< CMesh >( m_scene->GetRenderablesList() );

        if ( _meshes.size() < 1 )
        {
            ImGui::Text( "No meshes in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _lastMeshName = "";
        static std::string _currentMeshName = "";
        IMGUI_COMBO_CONSTRUCT( "meshes", _currentMeshName, _meshes );
        bool _refresh = ( _currentMeshName != _lastMeshName );
        _lastMeshName = _currentMeshName;

        if ( m_scene->HasRenderableNamed( _currentMeshName ) )
            _submenuMesh( dynamic_cast< CMesh* >( m_scene->GetRenderableByName( _currentMeshName ) ), _refresh );
    }

    void CImGuiUtilsLayer::_submenuMesh( CMesh* mesh, bool refresh )
    {
        auto _visible = mesh->visible();
        ImGui::Checkbox( "Visible", &_visible );
        mesh->setVisibility( _visible );

        if ( !_visible )
            return;

        _submenuTreeTransform( mesh->position, mesh->rotation, mesh->scale );
        _submenuTreeMaterial( mesh->material(), refresh );
    }

    void CImGuiUtilsLayer::_submenuTreeTransform( CVec3& position, CMat3& rotation, CVec3& scale )
    {
        // position, rotation and scale
        if ( ImGui::TreeNode( "World-transform" ) )
        {
            float32 _vposition[3] = { position.x(), position.y(), position.z() };
            ImGui::DragFloat3( "position", _vposition, GUI_UTILS_DRAGFLOAT_POSITION_SPEED );
            position = { _vposition[0], _vposition[1], _vposition[2] };

            auto _euler = tinymath::euler( rotation );
            ImGui::SliderFloat( "euler-x", &_euler.x(), -ENGINE_PI, ENGINE_PI );
            ImGui::SliderFloat( "euler-y", &_euler.y(), -ENGINE_PI / 2 + 0.001f, ENGINE_PI / 2 - 0.001f );
            ImGui::SliderFloat( "euler-z", &_euler.z(), -ENGINE_PI, ENGINE_PI );
            rotation = tinymath::rotation( _euler );

            float32 _vscale[3] = { scale.x(), scale.y(), scale.z() };
            ImGui::SliderFloat3( "scale", _vscale, 0.1f, 10.0f );
            scale = { _vscale[0], _vscale[1], _vscale[2] };

            ImGui::TreePop();
        }
    }

    void CImGuiUtilsLayer::_submenuTreeMaterial( CMaterial* material, bool refresh )
    {
        // material properties
        if ( ImGui::TreeNode( "Material" ) )
        {
            /* material type */
            int32 _materialType = 0;
            if ( material->type() == eMaterialType::LAMBERT ) _materialType = 0;
            else if ( material->type() == eMaterialType::PHONG ) _materialType = 1;
            else if ( material->type() == eMaterialType::BLINN_PHONG ) _materialType = 2;

            ImGui::RadioButton( "Lambert", &_materialType, 0 ); ImGui::SameLine();
            ImGui::RadioButton( "Phong", &_materialType, 1 ); ImGui::SameLine();
            ImGui::RadioButton( "Blinn-Phong", &_materialType, 2 ); ImGui::Spacing();

            eMaterialType _mtype = eMaterialType::LAMBERT;
            if ( _materialType == 0 ) _mtype = eMaterialType::LAMBERT;
            else if ( _materialType == 1 ) _mtype = eMaterialType::PHONG;
            else if ( _materialType == 2 ) _mtype = eMaterialType::BLINN_PHONG;

            material->setType( _mtype );

            /* material color-components */
            float32 _ambient[3] = { material->ambient.x(), material->ambient.y(), material->ambient.z() };
            float32 _diffuse[3] = { material->diffuse.x(), material->diffuse.y(), material->diffuse.z() };
            ImGui::ColorEdit3( "Ambient", _ambient );
            ImGui::ColorEdit3( "Diffuse", _diffuse );
            material->ambient = { _ambient[0], _ambient[1], _ambient[2] };
            material->diffuse = { _diffuse[0], _diffuse[1], _diffuse[2] };

            if ( _mtype == eMaterialType::PHONG || _mtype == eMaterialType::BLINN_PHONG )
            {
                float32 _specular[3] = { material->specular.x(), material->specular.y(), material->specular.z() };
                ImGui::ColorEdit3( "Specular", _specular );
                material->specular = { _specular[0], _specular[1], _specular[2] };

                ImGui::SliderFloat( "Shininess", &material->shininess, 1.0f, 256.0f );
            }

            /* material textures */
            auto _allTextures = CTextureManager::GetAllCachedTextures();

            // albedo-map
            static std::string _albedoMapName = material->albedoMap() ? material->albedoMap()->name() : "none";
            // in case the user selected a different renderable
            if ( refresh ) _albedoMapName = material->albedoMap() ? material->albedoMap()->name() : "none";
            IMGUI_COMBO_CONSTRUCT( "albedo-map", _albedoMapName, _allTextures );

            if ( CTextureManager::HasCachedTexture( _albedoMapName ) )
                material->setAlbedoMap( CTextureManager::GetCachedTexture( _albedoMapName ) );

            if ( ImGui::Button( "Clear-albedomap" ) )
            {
                _albedoMapName = "none";
                material->setAlbedoMap( nullptr );
            }

            // specular map
            static std::string _specularMapName  = material->specularMap() ? material->specularMap()->name() : "none";
            // in case the user selected a different renderable
            if ( refresh ) _specularMapName = material->specularMap() ? material->specularMap()->name() : "none";
            IMGUI_COMBO_CONSTRUCT( "specular-map", _specularMapName, _allTextures );

            if ( CTextureManager::HasCachedTexture( _specularMapName ) )
                material->setSpecularMap( CTextureManager::GetCachedTexture( _specularMapName ) );

            if ( ImGui::Button( "Clear-specularmap" ) )
            {
                _specularMapName = "none";
                material->setSpecularMap( nullptr );
            }

            /* transparency */
            ImGui::Checkbox( "Use-transparency", &material->transparent );
            if ( material->transparent )
                ImGui::SliderFloat( "Alpha", &material->alpha, 0.0f, 1.0f );

            ImGui::TreePop();
        }
    }

    void CImGuiUtilsLayer::_submenuSceneModels()
    {
        if ( !ImGui::CollapsingHeader( "Models" ) )
            return;

        auto _models = _collectTypedRenderables< CModel >( m_scene->GetRenderablesList() );

        if ( _models.size() < 1 )
        {
            ImGui::Text( "No models in the scene for now" );
            ImGui::End();
            return;
        }

        static std::string _lastModelName = "";
        static std::string _currentModelName = "";
        IMGUI_COMBO_CONSTRUCT( "models", _currentModelName, _models );
        bool _refresh = ( _currentModelName != _lastModelName );
        _lastModelName = _currentModelName;

        if ( m_scene->HasRenderableNamed( _currentModelName ) )
            _submenuModel( dynamic_cast< CModel* >( m_scene->GetRenderableByName( _currentModelName ) ), _refresh );
    }

    void CImGuiUtilsLayer::_submenuModel( CModel* model, bool refresh )
    {
        auto _visible = model->visible();
        ImGui::Checkbox( "Visible", &_visible );
        model->setVisibility( _visible );

        if ( !_visible )
            return;

        // position, rotation and scale (root of the model)
        _submenuTreeTransform( model->position, model->rotation, model->scale );

        // grab all submeshes and give access to their properties
        auto _submeshes = model->meshes();
        auto& _submeshesLocalTf = model->localTransforms();
        if ( ImGui::TreeNode( "Submeshes" ) )
        {
            static std::string _lastSubmeshName = "";
            static std::string _currentSubmeshName = "";
            static int _currentSubmeshIndex = -1;
            if ( refresh )
            {
                _lastSubmeshName = "";
                _currentSubmeshName = "";
                _currentSubmeshIndex = -1;
            }
            IMGUI_COMBO_CONSTRUCT_EXT( "submesh", _currentSubmeshName, _currentSubmeshIndex, _submeshes );
            bool _refreshSubmesh = ( _currentSubmeshName != _lastSubmeshName );
            _lastSubmeshName = _currentSubmeshName;

            if ( _currentSubmeshIndex == -1 )
            {
                ImGui::TreePop();
                return;
            }

            // local transform w.r.t. root
            auto _position = CVec3( _submeshesLocalTf[_currentSubmeshIndex].col( 3 ) );
            auto _rotation = CMat3( _submeshesLocalTf[_currentSubmeshIndex] );
            auto _scale = _submeshes[_currentSubmeshIndex]->scale;
            _submenuTreeTransform( _position, _rotation, _scale );
            _submeshesLocalTf[_currentSubmeshIndex].set( _position, 3 );
            _submeshesLocalTf[_currentSubmeshIndex].set( _rotation );
            _submeshes[_currentSubmeshIndex]->scale = _scale;

            // submesh's material
            _submenuTreeMaterial( _submeshes[_currentSubmeshIndex]->material(), _refreshSubmesh );

            ImGui::TreePop();
        }

    }

    void CImGuiUtilsLayer::_submenuSceneLights()
    {
        if ( !ImGui::CollapsingHeader( "Lights" ) )
            return;

        auto _lights = m_scene->GetLightsList();

        if ( _lights.size() < 1 )
        {
            ImGui::Text( "No lights in the scene for now" );
            return;
        }

        // show the main-light in the scene
        if ( m_scene->HasCurrentLight() )
        {
            auto _currentLight = m_scene->GetCurrentLight();
            ImGui::Text( "Main-light: " ); 
            ImGui::SameLine();
            ImGui::TextColored( ImVec4( 0.2f, 0.4f, 0.8f, 1.0f ), _currentLight->name().c_str() );
        }

        static std::string _lastLightName = "";
        static std::string _currentLightName = "";
        IMGUI_COMBO_CONSTRUCT( "lights", _currentLightName, _lights );
        bool _refresh = ( _currentLightName != _lastLightName );
        _lastLightName = _currentLightName;

        if ( m_scene->HasLightNamed( _currentLightName ) )
        {
            // give an option to set this as current light
            if ( ImGui::Button( "Set as main light" ) )
                m_scene->ChangeCurrentLight( _currentLightName );

            auto _light = m_scene->GetLightByName( _currentLightName );
            _submenuLight( _light, _refresh );
        }
    }

    void CImGuiUtilsLayer::_submenuLight( CILight* light, bool refresh )
    {
        float32 _cAmbient[3] = { light->ambient.x(), light->ambient.y(), light->ambient.z() };
        float32 _cDiffuse[3] = { light->diffuse.x(), light->diffuse.y(), light->diffuse.z() };
        float32 _cSpecular[3] = { light->specular.x(), light->specular.y(), light->specular.z() };
        ImGui::ColorEdit3( "Ambient-color", _cAmbient );
        ImGui::ColorEdit3( "Diffuse-color", _cDiffuse );
        ImGui::ColorEdit3( "Specular-color", _cSpecular );
        light->ambient = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
        light->diffuse = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
        light->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };

        ImGui::SliderFloat( "Intensity", &light->intensity, 0.0f, 1.0f );

        if ( light->type() == eLightType::DIRECTIONAL )
        {
            static CVec3 _direction = light->direction;
            if ( refresh ) _direction = light->direction;
            float32 _vdir[3] = { _direction.x(), _direction.y(), _direction.z() };
            ImGui::SliderFloat3( "Direction", _vdir, -1.0f, 1.0f );
            _direction = { _vdir[0], _vdir[1], _vdir[2] };
            light->direction = _direction.normalized();
        }
        else if ( light->type() == eLightType::POINT )
        {
            float32 _vpos[3] = { light->position.x(), light->position.y(), light->position.z() };
            ImGui::DragFloat3( "Position", _vpos, GUI_UTILS_DRAGFLOAT_POSITION_SPEED );
            light->position = { _vpos[0], _vpos[1], _vpos[2] };

            ImGui::SliderFloat( "Attn-constant", &light->atnConstant, 1.0f, 2.0f );
            ImGui::SliderFloat( "Attn-linear", &light->atnLinear, 0.0f, 0.5f );
            ImGui::SliderFloat( "Attn-quadratic", &light->atnQuadratic, 0.0f, 0.25f );
        }
        else if ( light->type() == eLightType::SPOT )
        {
            static CVec3 _direction = light->direction;
            if ( refresh ) _direction = light->direction;
            float32 _vdir[3] = { _direction.x(), _direction.y(), _direction.z() };
            ImGui::SliderFloat3( "Direction", _vdir, -1.0f, 1.0f );
            _direction = { _vdir[0], _vdir[1], _vdir[2] };
            light->direction = _direction.normalized();

            float32 _vpos[3] = { light->position.x(), light->position.y(), light->position.z() };
            ImGui::DragFloat3( "Position", _vpos, GUI_UTILS_DRAGFLOAT_POSITION_SPEED );
            light->position = { _vpos[0], _vpos[1], _vpos[2] };

            ImGui::SliderFloat( "Attn-constant", &light->atnConstant, 1.0f, 2.0f );
            ImGui::SliderFloat( "Attn-linear", &light->atnLinear, 0.0f, 0.5f );
            ImGui::SliderFloat( "Attn-quadratic", &light->atnQuadratic, 0.0f, 0.25f );
            ImGui::SliderFloat( "Inner-cutoff", &light->innerCutoff, ENGINE_PI / 10.0f, ENGINE_PI / 2 );
            ImGui::SliderFloat( "Outer-cutoff", &light->outerCutoff, ENGINE_PI / 10.0f, ENGINE_PI / 2 );
        }
    }

    void CImGuiUtilsLayer::_submenuSceneCameras()
    {
        if ( !ImGui::CollapsingHeader( "Cameras" ) )
            return;

        auto _cameras = m_scene->GetCamerasList();

        if ( _cameras.size() < 1 )
        {
            ImGui::Text( "No cameras in the scene for now" );
            ImGui::End();
            return;
        }

        // show the current-camera in the scene
        if ( m_scene->HasCurrentCamera() )
        {
            auto _currentCamera = m_scene->GetCurrentCamera();
            ImGui::Text( "Current-camera: " ); 
            ImGui::SameLine();
            ImGui::TextColored( ImVec4( 0.2f, 0.4f, 0.8f, 1.0f ), _currentCamera->name().c_str() );
        }

        static std::string _lastCameraName = "";
        static std::string _currentCameraName = "";
        IMGUI_COMBO_CONSTRUCT( "cameras", _currentCameraName, _cameras );
        bool _refresh = ( _currentCameraName != _lastCameraName );
        _lastCameraName = _currentCameraName;

        if ( m_scene->HasCameraNamed( _currentCameraName ) )
        {
            auto _camera = m_scene->GetCameraByName( _currentCameraName );
            auto _cameraType = _camera->type();

            // give an option to set this as current camera
            if ( ImGui::Button( ( _cameraType != eCameraType::FPS ) ? "Set current" : "Set current (press SPACE to toggle cursor)" ) )
            {
                m_scene->ChangeCurrentCamera( _currentCameraName );
                if ( m_scene->GetCurrentCamera()->type() == eCameraType::FPS )
                {
                    m_cursorDisabledByFpsCamera = true;
                    _camera->SetActiveMode( true );
                    m_window->disableCursor();
                }
            }

            _submenuCamera( _camera, _refresh );
        }
    }

    void CImGuiUtilsLayer::_submenuCamera( CICamera* camera, bool refresh )
    {
        /* common properties of all camera types */

        // position
        float32 _vposition[3] = { camera->position().x(), camera->position().y(), camera->position().z() };
        if ( ImGui::DragFloat3( "Position", _vposition, GUI_UTILS_DRAGFLOAT_POSITION_SPEED ) )
        {
            camera->SetActiveMode( false );
            camera->SetPosition( CVec3( _vposition[0], _vposition[1], _vposition[2] ) );
        }
        else
        {
            if ( camera->type() == eCameraType::FPS )
                camera->SetActiveMode( m_cursorDisabledByFpsCamera );
            else
                camera->SetActiveMode( true );
        }

        // projection-data (fov, ...)
        auto _projData = camera->proj_data();
        int _projType = ( _projData.projection == eCameraProjection::PERSPECTIVE ) ? 0 : 1;
        ImGui::RadioButton( "Perspective", &_projType, 0 ); ImGui::SameLine();
        ImGui::RadioButton( "Orthographic", &_projType, 1 ); ImGui::Spacing();
        _projData.projection = ( _projType == 0 ) ? eCameraProjection::PERSPECTIVE : eCameraProjection::ORTHOGRAPHIC;
        if ( _projData.projection == eCameraProjection::PERSPECTIVE )
        {
            ImGui::SliderFloat( "fov", &_projData.fov, 30.0f, 120.0f );
        }
        else
        {
            ImGui::SliderFloat( "width", &_projData.width, 1.0f, 10.0f );
            ImGui::SliderFloat( "height", &_projData.height, 1.0f, 10.0f );
        }
        ImGui::SliderFloat( "znear", &_projData.zNear, 0.1f, 1.0f );
        ImGui::SliderFloat( "zfar", &_projData.zFar, 10.0f, 100.0f );

        camera->SetProjectionData( _projData );

        /* type-specific properties */

        // target-point
        if ( camera->type() == eCameraType::FIXED || camera->type() == eCameraType::ORBIT )
        {
            float32 _vtargetPoint[3] = { camera->target_point().x(), camera->target_point().y(), camera->target_point().z() };
            if ( ImGui::DragFloat3( "Target-point", _vtargetPoint, GUI_UTILS_DRAGFLOAT_POSITION_SPEED ) )
            {
                camera->SetActiveMode( false );
                camera->SetTargetPoint( { _vtargetPoint[0], _vtargetPoint[1], _vtargetPoint[2] } );
            }
            else
            {
                camera->SetActiveMode( true );
            }
        }

        if ( camera->type() == eCameraType::ORBIT )
        {
            auto _orbitCamera = dynamic_cast< COrbitCamera* >( camera );
            float _moveSensitivity = _orbitCamera->move_sensitivity();
            float _zoomSensitivity = _orbitCamera->zoom_sensitivity();
            if ( ImGui::SliderFloat( "Drag-sens.", &_moveSensitivity, 0.000f, 0.100f ) )
            {
                camera->SetActiveMode( false );
                _orbitCamera->SetMoveSensitivity( _moveSensitivity );
            }
            else
            {
                camera->SetActiveMode( true );
            }
            if ( ImGui::SliderFloat( "Zoom-sens.", &_zoomSensitivity, 0.000f, 4.000f ) )
            {
                camera->SetActiveMode( false );
                _orbitCamera->SetZoomSensitivity( _zoomSensitivity );
            }
            else
            {
                camera->SetActiveMode( true );
            }
        }
        else if ( camera->type() == eCameraType::FPS )
        {
            auto _fpsCamera = dynamic_cast< CFpsCamera* >( camera );
            float _mouseSensitivity = _fpsCamera->sensitivity();
            float _camSpeed = _fpsCamera->speed();
            ImGui::SliderFloat( "Mouse-sens.", &_mouseSensitivity, 0.000f, 1.000f );
            ImGui::SliderFloat( "Camera-speed", &_camSpeed, 0.0f, 500.0f );
            _fpsCamera->SetSensitivity( _mouseSensitivity );
            _fpsCamera->SetSpeed( _camSpeed );
        }

        ImGui::TextColored( ImVec4( 0.8f, 0.8f, 0.2f, 1.0f ), "Information:" );
        ImGui::Text( camera->ToString().c_str() );
    }

}