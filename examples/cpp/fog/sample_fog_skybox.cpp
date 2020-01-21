
#include <CEngine.h>

engine::CVec3 g_lightDirPosition    = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightPointPosition  = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightSpotPosition   = { 0.0f, 5.0f, 0.0f };
// engine::CVec3 g_lightDirPosition    = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPointPosition  = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightSpotPosition   = { -2.0f, 4.0f, -1.0f };

engine::CVec3 g_lightDirDirection   = -g_lightDirPosition.normalized();
engine::CVec3 g_lightSpotDirection  = -g_lightSpotPosition.normalized();

const engine::CVec3 g_worldUp = { 0.0f, 1.0f, 0.0f };

float g_lowerLimit = 0.0f;
float g_upperLimit = 0.25f;

class FogSkyboxLayer : public engine::CImGuiLayer
{

public :

    FogSkyboxLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        auto _dirlight = new engine::CDirectionalLight( "directional",
                                                        { 0.2f, 0.2f, 0.2f },
                                                        { 0.4f, 0.4f, 0.4f },
                                                        { 0.8f, 0.8f, 0.8f },
                                                        g_lightDirDirection );

        auto _pointlight = new engine::CPointLight( "point",
                                                    { 0.2f, 0.2f, 0.2f },
                                                    { 0.5f, 0.5f, 0.5f },
                                                    { 0.8f, 0.8f, 0.8f },
                                                    g_lightPointPosition,
                                                    1.0f, 0.05f, 0.005f );

        auto _spotLight = new engine::CSpotLight( "spot",
                                                  { 0.2f, 0.2f, 0.2f },
                                                  { 0.5f, 0.5f, 0.5f },
                                                  { 0.8f, 0.8f, 0.8f },
                                                  g_lightSpotPosition,
                                                  g_lightSpotDirection,
                                                  1.0f, 0.05f, 0.005f,
                                                  ENGINE_PI / 4.0f,
                                                  ENGINE_PI / 3.0f );

        m_lights = { _dirlight, _pointlight, _spotLight };
        m_lightsNames = { "directional", "point", "spot" };
        m_lightSelectedIndex = 2;
        m_lightSelectedName = m_lightsNames[m_lightSelectedIndex];

        m_lightDirDirection     = g_lightDirDirection;
        m_lightSpotDirection    = g_lightSpotDirection;

        m_lightPointPosition    = g_lightPointPosition;
        m_lightSpotPosition     = g_lightSpotPosition;

        m_fogPtr = nullptr;
        m_skyboxPtr = nullptr;

        auto _cubemapsPtrs = engine::CTextureManager::GetAllCachedTexturesCube();
        for ( auto& _cubemapPtr : _cubemapsPtrs )
        {
            m_cubemaps.push_back( _cubemapPtr );
            m_cubemapsNames.push_back( _cubemapPtr->name() );
        }

        m_cubemapSelectedIndex = 0;
        m_cubemapSelectedName = ( m_cubemapsNames.size() > 0 ) ? m_cubemapsNames[m_cubemapSelectedIndex] : "undefined";

        m_wantsToCaptureMouse = false;
    }

    ~FogSkyboxLayer() {}

    void setSkyboxReference( engine::CSkybox* skyboxPtr ) { m_skyboxPtr = skyboxPtr; }
    void setFogReference( engine::CFog* fogPtr ) { m_fogPtr = fogPtr; }

    engine::CILight* selectedLight() const { return m_lights[m_lightSelectedIndex]; }

    void render() override
    {
        m_wantsToCaptureMouse = false;

        _menuUiLights();
        _menuUiFog();
        _menuUiSkybox();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;

        return false;
    }

protected :

    void _menuUiLights()
    {
        ImGui::Begin( "Lights configuration" );

        ImGui::Spacing();

        if ( ImGui::BeginCombo( "Lights", m_lightSelectedName.c_str() ) )
        {
            for ( size_t i = 0; i < m_lights.size(); i++ )
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

        _menuUiLightProps();

        ImGui::Spacing();
        ImGui::Text( m_lights[ m_lightSelectedIndex ]->toString().c_str() );
        ImGui::End();
    }

    void _menuUiLightProps()
    {
        auto _lightPtr = m_lights[m_lightSelectedIndex];
        if ( _lightPtr->type() == engine::eLightType::DIRECTIONAL )
        {
            float _direction[3] = { m_lightDirDirection.x(), m_lightDirDirection.y(), m_lightDirDirection.z() };
            ImGui::SliderFloat3( "direction", _direction, -10.0f, 10.0f );
            m_lightDirDirection = { _direction[0], _direction[1], _direction[2] };

            g_lightDirDirection = m_lightDirDirection.normalized();
            _lightPtr->direction = g_lightDirDirection;
        }
        else if ( _lightPtr->type() == engine::eLightType::POINT )
        {
            float _position[3] = { m_lightPointPosition.x(), m_lightPointPosition.y(), m_lightPointPosition.z() };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightPointPosition = { _position[0], _position[1], _position[2] };

            g_lightPointPosition = m_lightPointPosition;
            _lightPtr->position = g_lightPointPosition;
        }
        else if ( _lightPtr->type() == engine::eLightType::SPOT )
        {
            float _direction[3] = { m_lightSpotDirection.x(), m_lightSpotDirection.y(), m_lightSpotDirection.z() };
            ImGui::SliderFloat3( "direction", _direction, -10.0f, 10.0f );
            m_lightSpotDirection = { _direction[0], _direction[1], _direction[2] };

            float _position[3] = { m_lightSpotPosition.x(), m_lightSpotPosition.y(), m_lightSpotPosition.z() };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightSpotPosition = { _position[0], _position[1], _position[2] };

            g_lightSpotDirection = m_lightSpotDirection.normalized();
            g_lightSpotPosition = m_lightSpotPosition;

            _lightPtr->direction = g_lightSpotDirection;
            _lightPtr->position = g_lightSpotPosition;
        }
    }

    void _menuUiFog()
    {
        if ( !m_fogPtr )
            return;

        ImGui::Begin( "Fog configuration" );

        bool _enabled = m_fogPtr->active();
        ImGui::Checkbox( "enabled", &_enabled );
        m_fogPtr->setActive( _enabled );

        std::vector< std::string > _types = { engine::toString( engine::eFogType::LINEAR ),
                                              engine::toString( engine::eFogType::EXPONENTIAL ) };
        std::string _currentType = engine::toString( m_fogPtr->type() );

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

        if ( _currentType == engine::toString( engine::eFogType::LINEAR ) ) m_fogPtr->setType( engine::eFogType::LINEAR );
        if ( _currentType == engine::toString( engine::eFogType::EXPONENTIAL ) ) m_fogPtr->setType( engine::eFogType::EXPONENTIAL );

        ImGui::Spacing();

        if ( m_fogPtr->type() == engine::eFogType::LINEAR )
        {
            ImGui::SliderFloat( "dist-start", &m_fogPtr->distStart, 0.0f, 10.0f );
            ImGui::SliderFloat( "dist-end", &m_fogPtr->distEnd, m_fogPtr->distStart, 40.0f );
        }
        else if ( m_fogPtr->type() == engine::eFogType::EXPONENTIAL )
        {
            ImGui::SliderFloat( "density", &m_fogPtr->density, 0.001f, 0.1f );
            ImGui::SliderFloat( "gradient", &m_fogPtr->gradient, 1.0f, 10.0f );
            ImGui::SliderFloat( "dist-start", &m_fogPtr->distStart, 0.0f, 10.0f );
        }

        float _fogColor[3] = { m_fogPtr->color.x(), m_fogPtr->color.y(), m_fogPtr->color.z() };
        ImGui::ColorEdit3( "color", _fogColor );
        m_fogPtr->color = { _fogColor[0], _fogColor[1], _fogColor[2] };

        ImGui::SliderFloat( "lower-limit", &g_lowerLimit, 0.0f, g_upperLimit );
        ImGui::SliderFloat( "upper-limit", &g_upperLimit, g_lowerLimit, 1.0f );

        ImGui::End();
    }

    void _menuUiSkybox()
    {
        if ( !m_skyboxPtr )
            return;

        ImGui::Begin( "skybox-configuration" );

        if ( ImGui::BeginCombo( "Cubemaps", m_cubemapSelectedName.c_str() ) )
        {
            for ( size_t i = 0; i < m_cubemaps.size(); i++ )
            {
                std::string _cubemapName = m_cubemapsNames[i];
                bool _isSelected = ( _cubemapName == m_cubemapSelectedName );

                if ( ImGui::Selectable( _cubemapName.c_str(), _isSelected ) )
                {
                    m_cubemapSelectedName = _cubemapName;
                    m_cubemapSelectedIndex = i;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::Spacing();

        m_skyboxPtr->setCubemap( m_cubemaps[m_cubemapSelectedIndex] );

        ImGui::End();
    }

    std::vector< engine::CILight* > m_lights;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;

    engine::CVec3 m_lightDirDirection;
    engine::CVec3 m_lightSpotDirection;

    engine::CVec3 m_lightPointPosition;
    engine::CVec3 m_lightSpotPosition;

    engine::CFog* m_fogPtr;
    engine::CSkybox* m_skyboxPtr;

    std::vector< engine::CTextureCube* > m_cubemaps;
    std::vector< std::string > m_cubemapsNames;
    std::string m_cubemapSelectedName;
    int m_cubemapSelectedIndex;

    bool m_wantsToCaptureMouse;
};

void renderSceneWithFog( engine::CILight* lightPtr, 
                         engine::CICamera* cameraPtr, 
                         engine::CFog* fogPtr,
                         engine::CShader* shaderPtr,
                         engine::CMaterial* floorMaterialPtr,
                         engine::CMaterial* cubeMaterialPtr,
                         engine::CIRenderable* floor,
                         std::vector< engine::CIRenderable* > cubes );

void renderSkyboxWithFog( engine::CICamera* cameraPtr,
                          engine::CSkybox* skyboxPtr,
                          engine::CFog* fogPtr,
                          engine::CShader* shaderPtr );

engine::CMat4 computeSkyboxCorrectionMat( const engine::eAxis& axis );

int main()
{
    auto _app = new engine::CApplication();
    auto _uiLayer = new FogSkyboxLayer( "FogSkybox-utils" );
    _app->addGuiLayer( std::unique_ptr< FogSkyboxLayer >( _uiLayer ) );

    /* load the shader used to render the scene with fog */
    std::string _baseNamePhongWithFog = std::string( ENGINE_EXAMPLES_PATH ) + "fog/shaders/phong_with_fog";
    auto _shaderPhongWithFog = engine::CShaderManager::CreateShaderFromFiles( "phong_with_fog_shader",
                                                                              _baseNamePhongWithFog + "_vs.glsl",
                                                                              _baseNamePhongWithFog + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongWithFog, "Could not load phong-with-fog shader to render the scene using fog T_T" );

    /* load the shader used to render the skybox */
    std::string _baseNameSkybox = std::string( ENGINE_EXAMPLES_PATH ) + "fog/shaders/skybox_with_fog";
    auto _shaderSkybox = engine::CShaderManager::CreateShaderFromFiles( "skybox_with_fog_shader",
                                                                        _baseNameSkybox + "_vs.glsl",
                                                                        _baseNameSkybox + "_fs.glsl" );
    ENGINE_ASSERT( _shaderSkybox, "Could not load skybox-with-fog shader to render the skybox using fog T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             _app->window()->width(),
                                             _app->window()->height() );

//     const float _cameraSensitivity  = 0.25f;
//     const float _cameraSpeed        = 250.0f;
//     const float _cameraMaxDelta     = 10.0f;
//     
//     auto _camera = new engine::CFpsCamera( "fps",
//                                            { 2.0f, 2.0f, 2.0f },
//                                            { 0.0f, 0.0f, 0.0f },
//                                            engine::eAxis::Y,
//                                            _cameraProjData,
//                                            _cameraSensitivity,
//                                            _cameraSpeed,
//                                            _cameraMaxDelta );

    _app->scene()->addCamera( std::unique_ptr< engine::CICamera >( _camera ) );

    /* create a dummy camera to visualize the clipping volume */
    auto _cameraProjDataTest = engine::CCameraProjData();
    _cameraProjDataTest.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjDataTest.fov         = 45.0f;
    _cameraProjDataTest.aspect      = _app->window()->aspect();
    _cameraProjDataTest.zNear       = 1.0f;
    _cameraProjDataTest.zFar        = 3.0f;

    auto _cameraTest = new engine::CFixedCamera( "fixed",
                                                  engine::CVec3( 3.0f, 3.0f, 3.0f ),
                                                  engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                  engine::eAxis::Y,
                                                  _cameraProjDataTest );

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

    auto _floorMaterial = new engine::CMaterial( "floor_material",
                                                 engine::eMaterialType::PHONG,
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 64.0f,
                                                 _floorTexture,
                                                 _floorTexture );

    auto _cubeMaterial = new engine::CMaterial( "cube_material",
                                                engine::eMaterialType::PHONG,
                                                { 1.0f, 1.0f, 1.0f },
                                                { 1.0f, 1.0f, 1.0f },
                                                { 1.0f, 1.0f, 1.0f },
                                                64.0f,
                                                _floorTexture,
                                                _floorTexture );

    auto _fog = new engine::CFog( engine::eFogType::EXPONENTIAL,
                                  { 0.2f, 0.2f, 0.2f },
                                  0.05f, 1.5f,
                                  0.0f, 10.0f );

    auto _skybox = new engine::CSkybox();
    _skybox->setCubemap( engine::CTextureManager::GetCachedTextureCube( "cloudtop" ) );

    /**********************************************************************************************/

    auto _currentLight = _uiLayer->selectedLight();
    _uiLayer->setFogReference( _fog );
    _uiLayer->setSkyboxReference( _skybox );

    while( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _camera->setActiveMode( true );

        if ( _camera->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _camera->active() )
                _app->window()->disableCursor();
            else
                _app->window()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        /* use the light selected by the user */
        _currentLight = _uiLayer->selectedLight();

        _app->update();
        _app->begin();

        /* do our thing here ************************/

        // render the scene using fog
        renderSceneWithFog( _currentLight, _camera, _fog, _shaderPhongWithFog, _floorMaterial, _cubeMaterial, _floor, { _cube1, _cube2, _cube3 } );

        // render the skybox at last
        renderSkyboxWithFog( _camera, _skybox, _fog, _shaderSkybox );

        /********************************************/

        _app->render();
        _app->end();
    }

    return 0;
}

void renderSceneWithFog( engine::CILight* lightPtr, 
                         engine::CICamera* cameraPtr, 
                         engine::CFog* fogPtr,
                         engine::CShader* shaderPtr,
                         engine::CMaterial* floorMaterialPtr,
                         engine::CMaterial* cubeMaterialPtr,
                         engine::CIRenderable* floor,
                         std::vector< engine::CIRenderable* > cubes )
{
    if ( !lightPtr || !cameraPtr || !shaderPtr )
        return;

    shaderPtr->bind();
    shaderPtr->setInt( "u_spotLight.enabled", 0 );
    shaderPtr->setInt( "u_pointLight.enabled", 0 );
    shaderPtr->setInt( "u_directionalLight.enabled", 0 );

    /* setup the light according to its type */
    if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
    {
        shaderPtr->setInt( "u_directionalLight.enabled", 1 );
        shaderPtr->setVec3( "u_directionalLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_directionalLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_directionalLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_directionalLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_directionalLight.direction", lightPtr->direction );
    }
    else if ( lightPtr->type() == engine::eLightType::POINT )
    {
        shaderPtr->setInt( "u_pointLight.enabled", 1 );
        shaderPtr->setVec3( "u_pointLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_pointLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_pointLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_pointLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_pointLight.position", lightPtr->position );
        shaderPtr->setFloat( "u_pointLight.attnk0", lightPtr->atnConstant );
        shaderPtr->setFloat( "u_pointLight.attnk1", lightPtr->atnLinear );
        shaderPtr->setFloat( "u_pointLight.attnk2", lightPtr->atnQuadratic );
    }
    else if ( lightPtr->type() == engine::eLightType::SPOT )
    {
        shaderPtr->setInt( "u_spotLight.enabled", 1 );
        shaderPtr->setVec3( "u_spotLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_spotLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_spotLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_spotLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_spotLight.position", lightPtr->position );
        shaderPtr->setFloat( "u_spotLight.attnk0", lightPtr->atnConstant );
        shaderPtr->setFloat( "u_spotLight.attnk1", lightPtr->atnLinear );
        shaderPtr->setFloat( "u_spotLight.attnk2", lightPtr->atnQuadratic );
        shaderPtr->setVec3( "u_spotLight.direction", lightPtr->direction );
        shaderPtr->setFloat( "u_spotLight.innerCutoffCos", std::cos( lightPtr->innerCutoff ) );
        shaderPtr->setFloat( "u_spotLight.outerCutoffCos", std::cos( lightPtr->outerCutoff ) );
    }

    /* setup the view and proj matrices */
    shaderPtr->setMat4( "u_viewProjMatrix", cameraPtr->matProj() * cameraPtr->matView() );
    shaderPtr->setMat4( "u_viewMatrix", cameraPtr->matView() );// used for pos w.r.t. camera calculations
    shaderPtr->setVec3( "u_viewerPosition", cameraPtr->position() );

    /* setup the fog properties */
    fogPtr->bind( shaderPtr );

    /* render the floor */
    floorMaterialPtr->bind( shaderPtr );
    {
        auto _modelMat = floor->matModel();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        floor->render();
    }
    floorMaterialPtr->unbind();

    /* render the cubes (all use the same material) */
    cubeMaterialPtr->bind( shaderPtr );
    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        auto _modelMat = cubes[i]->matModel();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        cubes[i]->render();
    }
    cubeMaterialPtr->unbind();

    fogPtr->unbind();
    shaderPtr->unbind();
}

void renderSkyboxWithFog( engine::CICamera* cameraPtr,
                          engine::CSkybox* skyboxPtr,
                          engine::CFog* fogPtr,
                          engine::CShader* shaderPtr )
{
    /* compute rotation to correct for up axis */
    auto _correctionMat = computeSkyboxCorrectionMat( cameraPtr->upAxis() );

    /* render the skybox (as it last rendererd, we take advantage of early depth testing) */

    shaderPtr->bind();
    shaderPtr->setMat4( "u_viewProjMatrix", cameraPtr->matProj() * engine::CMat4( engine::CMat3( cameraPtr->matView() ) ) * _correctionMat );
    shaderPtr->setInt( "u_fog.enabled", ( fogPtr->active() ) ? 1 : 0 );
    shaderPtr->setVec3( "u_fog.color", fogPtr->color );
    shaderPtr->setFloat( "u_fog.lowerLimit", g_lowerLimit );
    shaderPtr->setFloat( "u_fog.upperLimit", g_upperLimit );

    skyboxPtr->render();
    shaderPtr->unbind();

}

engine::CMat4 computeSkyboxCorrectionMat( const engine::eAxis& axis )
{
    if ( axis == engine::eAxis::X ) return engine::rotationZ( -ENGINE_PI / 2.0f );
    else if ( axis == engine::eAxis::Z ) return engine::rotationX( ENGINE_PI / 2.0f );

    return engine::CMat4();
}
