
#include <CEngine.h>

engine::CVec3 g_lightDirPosition    = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightPointPosition  = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightSpotPosition   = { 0.0f, 5.0f, 0.0f };
// engine::CVec3 g_lightDirPosition    = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPointPosition  = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightSpotPosition   = { -2.0f, 4.0f, -1.0f };

engine::CVec3 g_lightDirDirection   = ( -g_lightDirPosition ).normalized();
engine::CVec3 g_lightSpotDirection  = ( -g_lightSpotPosition ).normalized();

const engine::CVec3 g_worldUp = { 0.0f, 1.0f, 0.0f };

class FogUtilslLayer : public engine::CImGuiLayer
{

public :

    FogUtilslLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        m_lightSelectedIndex = -1;
        m_lightSelectedName = "none";

        m_lightDirDirection     = g_lightDirDirection;
        m_lightSpotDirection    = g_lightSpotDirection;

        m_lightPointPosition    = g_lightPointPosition;
        m_lightSpotPosition     = g_lightSpotPosition;

        m_fogRef = nullptr;

        m_wantsToCaptureMouse = false;
    }

    ~FogUtilslLayer() {}

    void addLightRef( engine::CILight* lightRef )
    {
        m_lightsRefs.push_back( lightRef );
        m_lightsNames.push_back( lightRef->name() );
        if ( m_lightSelectedIndex == -1 )
            m_lightSelectedIndex = 0;
    }

    void setFogReference( engine::CFog* fogRef )
    {
        m_fogRef = fogRef;
    }

    engine::CILight* selectedLight() const
    {
        if ( m_lightSelectedIndex == -1 )
            return nullptr;
        return m_lightsRefs[m_lightSelectedIndex];
    }


    void render() override
    {
        m_wantsToCaptureMouse = false;

        _menuUiLights();
        _menuUiFog();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;

        return false;
    }

private :

    void _menuUiLights()
    {
        ImGui::Begin( "Lights configuration" );

        ImGui::Spacing();

        if ( ImGui::BeginCombo( "Lights", m_lightSelectedName.c_str() ) )
        {
            for ( size_t i = 0; i < m_lightsRefs.size(); i++ )
            {
                std::string _lightName = m_lightsNames[i];
                bool _isSelected = ( _lightName == m_lightSelectedName );

                if ( ImGui::Selectable( _lightName.c_str(), _isSelected ) )
                {
                    m_lightSelectedName = _lightName;
                    m_lightSelectedIndex = i;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::Spacing();

        if ( m_lightSelectedIndex != -1 )
        {
            _menuUiLightProps();
            ImGui::Spacing();
            ImGui::Text( m_lightsRefs[ m_lightSelectedIndex ]->toString().c_str() );
            ImGui::End();
        }

    }

    void _menuUiLightProps()
    {
        auto _lightRef = m_lightsRefs[m_lightSelectedIndex];
        if ( _lightRef->type() == engine::eLightType::DIRECTIONAL )
        {
            float _direction[3] = { m_lightDirDirection.x(), m_lightDirDirection.y(), m_lightDirDirection.z() };
            ImGui::SliderFloat3( "direction", _direction, -10.0f, 10.0f );
            m_lightDirDirection = { _direction[0], _direction[1], _direction[2] };

            g_lightDirDirection = m_lightDirDirection.normalized();
            _lightRef->direction = g_lightDirDirection;
        }
        else if ( _lightRef->type() == engine::eLightType::POINT )
        {
            float _position[3] = { m_lightPointPosition.x(), m_lightPointPosition.y(), m_lightPointPosition.z() };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightPointPosition = { _position[0], _position[1], _position[2] };

            g_lightPointPosition = m_lightPointPosition;
            _lightRef->position = g_lightPointPosition;
        }
        else if ( _lightRef->type() == engine::eLightType::SPOT )
        {
            float _direction[3] = { m_lightSpotDirection.x(), m_lightSpotDirection.y(), m_lightSpotDirection.z() };
            ImGui::SliderFloat3( "direction", _direction, -10.0f, 10.0f );
            m_lightSpotDirection = { _direction[0], _direction[1], _direction[2] };

            float _position[3] = { m_lightSpotPosition.x(), m_lightSpotPosition.y(), m_lightSpotPosition.z() };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightSpotPosition = { _position[0], _position[1], _position[2] };

            g_lightSpotDirection = m_lightSpotDirection.normalized();
            g_lightSpotPosition = m_lightSpotPosition;

            _lightRef->direction = g_lightSpotDirection;
            _lightRef->position = g_lightSpotPosition;
        }
    }

    void _menuUiFog()
    {
        if ( !m_fogRef )
            return;

        ImGui::Begin( "Fog configuration" );

        std::vector< std::string > _types = { engine::toString( engine::eFogType::LINEAR ),
                                              engine::toString( engine::eFogType::EXPONENTIAL ) };
        std::string _currentType = engine::toString( m_fogRef->type() );

        if ( ImGui::BeginCombo( "type", _currentType.c_str() ) )
        {
            for ( size_t i = 0; i < _types.size(); i++ )
            {
                std::string _type =_types[i];
                bool _isSelected = ( _currentType == _type );

                if ( ImGui::Selectable( _type.c_str(), _isSelected ) )
                    _currentType = _type;

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( _currentType == engine::toString( engine::eFogType::LINEAR ) ) m_fogRef->setType( engine::eFogType::LINEAR );
        if ( _currentType == engine::toString( engine::eFogType::EXPONENTIAL ) ) m_fogRef->setType( engine::eFogType::EXPONENTIAL );

        ImGui::Spacing();

        if ( m_fogRef->type() == engine::eFogType::LINEAR )
        {
            ImGui::SliderFloat( "dist-start", &m_fogRef->distStart, 0.0f, 10.0f );
            ImGui::SliderFloat( "dist-end", &m_fogRef->distEnd, m_fogRef->distStart, 40.0f );
        }
        else if ( m_fogRef->type() == engine::eFogType::EXPONENTIAL )
        {
            ImGui::SliderFloat( "density", &m_fogRef->density, 0.001f, 0.1f );
            ImGui::SliderFloat( "gradient", &m_fogRef->gradient, 1.0f, 10.0f );
            ImGui::SliderFloat( "dist-start", &m_fogRef->distStart, 0.0f, 10.0f );
        }

        float _fogColor[3] = { m_fogRef->color.x(), m_fogRef->color.y(), m_fogRef->color.z() };
        ImGui::ColorEdit3( "color", _fogColor );
        m_fogRef->color = { _fogColor[0], _fogColor[1], _fogColor[2] };

        ImGui::End();
    }

    std::vector< engine::CILight* > m_lightsRefs;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;

    engine::CVec3 m_lightDirDirection;
    engine::CVec3 m_lightSpotDirection;

    engine::CVec3 m_lightPointPosition;
    engine::CVec3 m_lightSpotPosition;

    engine::CFog* m_fogRef;

    bool m_wantsToCaptureMouse;
};

void renderSceneWithFog( engine::CILight* lightRef, 
                         engine::CICamera* cameraRef, 
                         engine::CFog* fogRef,
                         engine::CShader* shaderRef,
                         engine::CMaterial* floorMaterialRef,
                         engine::CMaterial* cubeMaterialRef,
                         engine::CIRenderable* floor,
                         std::vector< engine::CIRenderable* > cubes );

int main()
{
    auto _app = std::make_unique<engine::CApplication>();
    auto _uiLayer = std::make_unique<FogUtilslLayer>( "Fog-utils" );
    auto _uiLayerRef = dynamic_cast<FogUtilslLayer*>( _app->addGuiLayer( std::move( _uiLayer ) ) );

    /* load the shader used to render the scene with fog */
    std::string _baseNamePhongWithFog = std::string( ENGINE_EXAMPLES_PATH ) + "fog/shaders/phong_with_fog";
    auto _shaderPhongWithFogRef = engine::CShaderManager::CreateShaderFromFiles( "phong_with_fog_shader",
                                                                              _baseNamePhongWithFog + "_vs.glsl",
                                                                              _baseNamePhongWithFog + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongWithFogRef, "Could not load phong-with-fog shader to render the scene using fog T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _dirlight = std::make_unique<engine::CDirectionalLight>( "directional",
                                                                  engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                                  engine::CVec3( 0.4f, 0.4f, 0.4f ),
                                                                  engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                  g_lightDirDirection );
    _uiLayerRef->addLightRef( _dirlight.get() );

    auto _pointlight = std::make_unique<engine::CPointLight>( "point",
                                                              engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                              engine::CVec3( 0.5f, 0.5f, 0.5f ),
                                                              engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                              g_lightPointPosition,
                                                              1.0f, 0.05f, 0.005f );
    _uiLayerRef->addLightRef( _pointlight.get() );

    auto _spotlight = std::make_unique<engine::CSpotLight>( "spot",
                                                            engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                            engine::CVec3( 0.5f, 0.5f, 0.5f ),
                                                            engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                            g_lightSpotPosition,
                                                            g_lightSpotDirection,
                                                            1.0f, 0.05f, 0.005f,
                                                            ENGINE_PI / 4.0f,
                                                            ENGINE_PI / 3.0f );
    _uiLayerRef->addLightRef( _spotlight.get() );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 0.0f, 0.0f, 3.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Y,
                                                           _cameraProjData,
                                                           _app->window()->width(),
                                                           _app->window()->height() );

////     const float _cameraSensitivity  = 0.25f;
////     const float _cameraSpeed        = 250.0f;
////     const float _cameraMaxDelta     = 10.0f;
//// 
////     auto _camera = std::make_unique<engine::CFpsCamera>( "fps",
////                                                          engine::CVec3( 2.0f, 2.0f, 2.0f ),
////                                                          engine::CVec3( 0.0f, 0.0f, 0.0f ),
////                                                          engine::eAxis::Y,
////                                                          _cameraProjData,
////                                                          _cameraSensitivity,
////                                                          _cameraSpeed,
////                                                          _cameraMaxDelta );

    auto _cameraRef = _app->scene()->AddCamera( std::move( _camera ) );

    auto _floor = engine::CMeshBuilder::createPlane( 30.0f, 30.0f, engine::eAxis::Y );
    _floor->position = { 0.0f, 0.0f, 0.0f };

    auto _cube1 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube1->position = { 0.0f, 2.0f, 0.0f };

    auto _cube2 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube2->position = { 2.0f, 0.5f, 1.0f };

    auto _cube3 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube3->position = { -1.0f, 0.5f, 2.0f };
    _cube3->rotation = tinymath::rotation( engine::CVec3( 1.0f, 0.0f, 1.0f ), engine::toRadians( 60.0f ) );
    _cube3->scale = { 0.5f, 0.5f, 0.5f };

    auto _floorTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );

    ENGINE_ASSERT( _floorTexture, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTexture, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = std::make_unique<engine::CMaterial>( "floor_material",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTexture,
                                                               _floorTexture );

    auto _cubeMaterial = std::make_unique<engine::CMaterial>( "cube_material",
                                                              engine::eMaterialType::PHONG,
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              64.0f,
                                                              _floorTexture,
                                                              _floorTexture );

    auto _fog = std::make_unique<engine::CFog>( engine::eFogType::EXPONENTIAL,
                                                engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                0.05f, 1.5f,
                                                0.0f, 10.0f );

    /**********************************************************************************************/

    auto _currentLightRef = _uiLayerRef->selectedLight();
    _uiLayerRef->setFogReference( _fog.get() );

    while( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _cameraRef->SetActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _cameraRef->SetActiveMode( true );

        if ( _cameraRef->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _cameraRef->active() )
                _app->window()->disableCursor();
            else
                _app->window()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        /* use the light selected by the user */
        _currentLightRef = _uiLayerRef->selectedLight();

        _app->update();
        _app->begin();

        /* do our thing here ************************/

        // render the scene using fog
        renderSceneWithFog( _currentLightRef, _cameraRef, _fog.get(), _shaderPhongWithFogRef, 
                            _floorMaterial.get(), _cubeMaterial.get(), 
                            _floor.get(), { _cube1.get(), _cube2.get(), _cube3.get() } );

        /********************************************/

        _app->render();
        _app->end();
        tinyutils::Clock::Tock();
    }

    return 0;
}

void renderSceneWithFog( engine::CILight* lightRef, 
                         engine::CICamera* cameraRef, 
                         engine::CFog* fogRef,
                         engine::CShader* shaderRef,
                         engine::CMaterial* floorMaterialRef,
                         engine::CMaterial* cubeMaterialRef,
                         engine::CIRenderable* floor,
                         std::vector< engine::CIRenderable* > cubes )
{
    if ( !lightRef || !cameraRef || !shaderRef )
        return;

    shaderRef->bind();
    shaderRef->setInt( "u_spotLight.enabled", 0 );
    shaderRef->setInt( "u_pointLight.enabled", 0 );
    shaderRef->setInt( "u_directionalLight.enabled", 0 );

    /* setup the light according to its type */
    if ( lightRef->type() == engine::eLightType::DIRECTIONAL )
    {
        shaderRef->setInt( "u_directionalLight.enabled", 1 );
        shaderRef->setVec3( "u_directionalLight.ambient", lightRef->ambient );
        shaderRef->setVec3( "u_directionalLight.diffuse", lightRef->diffuse );
        shaderRef->setVec3( "u_directionalLight.specular", lightRef->specular );
        shaderRef->setFloat( "u_directionalLight.intensity", lightRef->intensity );
        shaderRef->setVec3( "u_directionalLight.direction", lightRef->direction );
    }
    else if ( lightRef->type() == engine::eLightType::POINT )
    {
        shaderRef->setInt( "u_pointLight.enabled", 1 );
        shaderRef->setVec3( "u_pointLight.ambient", lightRef->ambient );
        shaderRef->setVec3( "u_pointLight.diffuse", lightRef->diffuse );
        shaderRef->setVec3( "u_pointLight.specular", lightRef->specular );
        shaderRef->setFloat( "u_pointLight.intensity", lightRef->intensity );
        shaderRef->setVec3( "u_pointLight.position", lightRef->position );
        shaderRef->setFloat( "u_pointLight.attnk0", lightRef->atnConstant );
        shaderRef->setFloat( "u_pointLight.attnk1", lightRef->atnLinear );
        shaderRef->setFloat( "u_pointLight.attnk2", lightRef->atnQuadratic );
    }
    else if ( lightRef->type() == engine::eLightType::SPOT )
    {
        shaderRef->setInt( "u_spotLight.enabled", 1 );
        shaderRef->setVec3( "u_spotLight.ambient", lightRef->ambient );
        shaderRef->setVec3( "u_spotLight.diffuse", lightRef->diffuse );
        shaderRef->setVec3( "u_spotLight.specular", lightRef->specular );
        shaderRef->setFloat( "u_spotLight.intensity", lightRef->intensity );
        shaderRef->setVec3( "u_spotLight.position", lightRef->position );
        shaderRef->setFloat( "u_spotLight.attnk0", lightRef->atnConstant );
        shaderRef->setFloat( "u_spotLight.attnk1", lightRef->atnLinear );
        shaderRef->setFloat( "u_spotLight.attnk2", lightRef->atnQuadratic );
        shaderRef->setVec3( "u_spotLight.direction", lightRef->direction );
        shaderRef->setFloat( "u_spotLight.innerCutoffCos", std::cos( lightRef->innerCutoff ) );
        shaderRef->setFloat( "u_spotLight.outerCutoffCos", std::cos( lightRef->outerCutoff ) );
    }

    /* setup the view and proj matrices */
    shaderRef->setMat4( "u_viewProjMatrix", cameraRef->mat_proj() * cameraRef->mat_view() );
    shaderRef->setMat4( "u_viewMatrix", cameraRef->mat_view() );// used for pos w.r.t. camera calculations
    shaderRef->setVec3( "u_viewerPosition", cameraRef->position() );

    /* setup the fog properties */
    fogRef->bind( shaderRef );

    /* render the floor */
    floorMaterialRef->bind( shaderRef );
    {
        auto _modelMat = floor->matModel();
        shaderRef->setMat4( "u_modelMatrix", _modelMat );
        shaderRef->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        floor->render();
    }
    floorMaterialRef->unbind();

    /* render the cubes (all use the same material) */
    cubeMaterialRef->bind( shaderRef );
    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        auto _modelMat = cubes[i]->matModel();
        shaderRef->setMat4( "u_modelMatrix", _modelMat );
        shaderRef->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        cubes[i]->render();
    }
    cubeMaterialRef->unbind();

    fogRef->unbind();
    shaderRef->unbind();
}
