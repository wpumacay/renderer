
#include <CEngine.h>

engine::CVec3 g_lightDirPosition    = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightPointPosition  = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightSpotPosition   = { 0.0f, 5.0f, 0.0f };

// engine::CVec3 g_lightDirPosition    = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPointPosition  = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightSpotPosition   = { -2.0f, 4.0f, -1.0f };

engine::CVec3 g_lightDirDirection   = engine::CVec3::normalize( -g_lightDirPosition );
engine::CVec3 g_lightPointDirection = engine::CVec3::normalize( -g_lightPointPosition );
engine::CVec3 g_lightSpotDirection  = engine::CVec3::normalize( -g_lightSpotPosition );

engine::CVec3 g_focusPoint = { 0.0f, 0.0f, 0.0f };
const engine::CVec3 g_worldUp = { 0.0f, 1.0f, 0.0f };

bool g_useAutofixToCamera = false;

float g_widthDir = 20.0f;
float g_heightDir = 20.0f;
float g_depthDir = 10.0f;

float g_znearPoint = 0.1f;
float g_zfarPoint = 15.0f;
float g_fovPoint = 120.0f;

float g_znearSpot = 0.1f;
float g_zfarSpot = 15.0f;
float g_fovSpot = 120.0f;

float g_extraWidth = 1.0f;
float g_extraHeight = 1.0f;
float g_extraDepth = 1.0f;

class ApplicationUi : public engine::CImguiUi
{

public :

    ApplicationUi( engine::COpenGLContext* context ) 
        : engine::CImguiUi( context ) {}

    ~ApplicationUi() {}

    engine::CILight* selectedLight() const { return m_lights[m_lightSelectedIndex]; }

protected :

    void _initInternal() override
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
        m_lightPointDirection   = g_lightPointDirection;
        m_lightSpotDirection    = g_lightSpotDirection;

        m_lightPointPosition    = g_lightPointPosition;
        m_lightSpotPosition     = g_lightSpotPosition;
    }

    void _renderInternal() override
    {
        _menuUiRendererStats();
        _menuUiLights();
    }

    void _menuUiRendererStats()
    {
        ImGui::Begin( "statistics" );
        ImGui::Text( "fps           : %.5f", engine::COpenGLApp::GetInstance()->fps() );
        ImGui::Text( "frame-time    : %.5f", engine::COpenGLApp::GetInstance()->frametime() );
        ImGui::End();
    }

    void _menuUiLights()
    {
        ImGui::Begin( "Lights shadowmapping configuration" );

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

        _menuUiLightShadowProps();

        ImGui::Spacing();
        ImGui::Text( m_lights[ m_lightSelectedIndex ]->toString().c_str() );
        ImGui::End();
    }

    void _menuUiLightShadowProps()
    {
        auto _lightPtr = m_lights[m_lightSelectedIndex];
        if ( _lightPtr->type() == engine::eLightType::DIRECTIONAL )
        {
            /* shadow frustum transform in world-space (view) */
            float _direction[3] = { m_lightDirDirection.x, m_lightDirDirection.y, m_lightDirDirection.z };
            ImGui::SliderFloat3( "direction", _direction, -1.0f, 1.0f );
            m_lightDirDirection = { _direction[0], _direction[1], _direction[2] };

            g_lightDirDirection = engine::CVec3::normalize( m_lightDirDirection );
            _lightPtr->direction = g_lightDirDirection;

            // whether or not to use autofix-to-camera for directional lights shadowmapping
            ImGui::Checkbox( "autofix-to-camera", &g_useAutofixToCamera );

            /* shadow-frustum size properties (proj) */
            if ( g_useAutofixToCamera )
            {
                ImGui::SliderFloat( "extra-width", &g_extraWidth, 0.0f, 5.0f );
                ImGui::SliderFloat( "extra-height", &g_extraHeight, 0.0f, 5.0f );
                ImGui::SliderFloat( "extra-depth", &g_extraDepth, 0.0f, 5.0f );
            }
            else
            {
                ImGui::SliderFloat( "depth", &g_depthDir, 0.1f, 20.0f );
                ImGui::SliderFloat( "width", &g_widthDir, 20.0f, 40.0f );
                ImGui::SliderFloat( "height", &g_heightDir, 20.0f, 40.0f );

                float _focusp[3] = { g_focusPoint.x, g_focusPoint.y, g_focusPoint.z };
                ImGui::SliderFloat3( "focus", _focusp, -10.0f, 10.0f );
                g_focusPoint = { _focusp[0], _focusp[1], _focusp[2] };
            }
        }
        else if ( _lightPtr->type() == engine::eLightType::POINT )
        {
            float _position[3] = { m_lightPointPosition.x, m_lightPointPosition.y, m_lightPointPosition.z };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightPointPosition = { _position[0], _position[1], _position[2] };

            g_lightPointDirection = engine::CVec3::normalize( m_lightPointDirection );
            g_lightPointPosition = m_lightPointPosition;
            _lightPtr->position = g_lightPointPosition;

            ImGui::SliderFloat( "znear", &g_znearPoint, 0.1f, 5.0f );
            ImGui::SliderFloat( "zfar", &g_zfarPoint, g_znearPoint, 20.0f );
            ImGui::SliderFloat( "fov", &g_fovPoint, 20.0f, 150.0f );

            float _focusp[3] = { g_focusPoint.x, g_focusPoint.y, g_focusPoint.z };
            ImGui::SliderFloat3( "focus", _focusp, -10.0f, 10.0f );
            g_focusPoint = { _focusp[0], _focusp[1], _focusp[2] };
        }
        else if ( _lightPtr->type() == engine::eLightType::SPOT )
        {
            float _direction[3] = { m_lightSpotDirection.x, m_lightSpotDirection.y, m_lightSpotDirection.z };
            ImGui::SliderFloat3( "direction", _direction, -1.0f, 1.0f );
            m_lightSpotDirection = { _direction[0], _direction[1], _direction[2] };

            float _position[3] = { m_lightSpotPosition.x, m_lightSpotPosition.y, m_lightSpotPosition.z };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightSpotPosition = { _position[0], _position[1], _position[2] };

            g_lightSpotDirection = engine::CVec3::normalize( m_lightSpotDirection );
            g_lightSpotPosition = m_lightSpotPosition;

            _lightPtr->direction = g_lightSpotDirection;
            _lightPtr->position = g_lightSpotPosition;

            ImGui::SliderFloat( "znear", &g_znearSpot, 0.1f, 5.0f );
            ImGui::SliderFloat( "zfar", &g_zfarSpot, g_znearSpot, 20.0f );
            ImGui::SliderFloat( "fov", &g_fovSpot, 20.0f, 150.0f );
        }
    }

    std::vector< engine::CILight* > m_lights;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;

    engine::CVec3 m_lightDirDirection;
    engine::CVec3 m_lightPointDirection;
    engine::CVec3 m_lightSpotDirection;

    engine::CVec3 m_lightPointPosition;
    engine::CVec3 m_lightSpotPosition;
};

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CICamera* cameraPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::CIRenderable* floor,
                        std::vector< engine::CIRenderable* > cubes );

void renderSceneWithShadows( engine::CILight* lightPtr, 
                             engine::CICamera* cameraPtr, 
                             engine::CShadowMap* shadowMapPtr,
                             engine::CShader* shaderPtr,
                             engine::CPhongMaterial* floorMaterialPtr,
                             engine::CPhongMaterial* cubeMaterialPtr,
                             engine::CIRenderable* floor,
                             std::vector< engine::CIRenderable* > cubes );

void renderShadowMapVisualization( engine::CILight* lightPtr,
                                   engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr );

void showDirectionalLightVolume( engine::CICamera* cameraPtr,
                                 engine::CDirectionalLight* dirLightPtr,
                                 engine::CShadowMap* shadowMapPtr );

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _ui = new ApplicationUi( engine::COpenGLApp::GetWindow()->context() );
    _ui->init();

    _app->setUi( std::unique_ptr< ApplicationUi >( _ui ) );

    /* load the shader used to render the scene normally (single-light for now) */
    std::string _baseNamePhongWithShadows = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/phong_with_shadows";
    auto _shaderPhongWithShadows = engine::CShaderManager::CreateShaderFromFiles( "phong_with_shadows_shader",
                                                                                  _baseNamePhongWithShadows + "_vs.glsl",
                                                                                  _baseNamePhongWithShadows + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongWithShadows, "Could not load phong-with-shadows shader to render the scene using the shadowmap T_T" );

    /* load the shader used for shadow mapping */
    std::string _baseNameShadowMapProjection = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_projection";
    auto _shaderShadowMapProj = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_projection_shader",
                                                                               _baseNameShadowMapProjection + "_vs.glsl",
                                                                               _baseNameShadowMapProjection + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapProj, "Couldn't load shadow-mapping shader use to create the depth-map T_T" );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapViz = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                             _baseNameShadowMapViz + "_vs.glsl",
                                                                             _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapViz, "Couldn't load the visualization shader to check the depth-map T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

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

    /* create a dummy camera to visualize the clipping volume */
    auto _cameraProjDataTest = engine::CCameraProjData();
    _cameraProjDataTest.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjDataTest.fov         = 45.0f;
    _cameraProjDataTest.aspect      = engine::COpenGLApp::GetWindow()->aspect();
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
    _cube3->rotation = engine::CMat4::rotation( engine::toRadians( 60.0f ), { 1.0f, 0.0f, 1.0f } );
    _cube3->scale = { 0.5f, 0.5f, 0.5f };

    auto _floorTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );

    ENGINE_ASSERT( _floorTexture, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTexture, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = new engine::CPhongMaterial( "floor_material",
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      64.0f,
                                                      _floorTexture,
                                                      _floorTexture );

    auto _cubeMaterial = new engine::CPhongMaterial( "cube_material",
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     64.0f,
                                                     _floorTexture,
                                                     _floorTexture );

    auto _lightTest = new engine::CDirectionalLight( "directional_test",
                                                     { 0.3f, 0.3f, 0.3f },
                                                     { 0.3f, 0.3f, 0.3f },
                                                     { 0.3f, 0.3f, 0.3f },
                                                     engine::CVec3::normalize( engine::CVec3( 0.0f, 0.0f, 0.0f ) - g_lightDirPosition ) );

    /**********************************************************************************************/

    auto _currentLight = _ui->selectedLight();
    auto _shadowmap = new engine::CShadowMap( 4096, 4096 );

    engine::float32 _quad_buffData[] = {
     /*|  positions |     uvs    |*/
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    engine::uint32 _quad_indices[] = {
        0, 1, 2, 
        0, 2, 3
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "uv", engine::eElementType::Float2, false } };

    auto _quad_vbuffer = new engine::CVertexBuffer( _layout,
                                                    engine::eBufferUsage::STATIC,
                                                    sizeof( _quad_buffData ),
                                                    _quad_buffData );;

    auto _quad_ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                                   6, _quad_indices );

    auto _quad_varray = new engine::CVertexArray();
    _quad_varray->addVertexBuffer( _quad_vbuffer );
    _quad_varray->setIndexBuffer( _quad_ibuffer );

    while( _app->active() )
    {
        if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
            _camera->setActiveMode( true );
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_F ) )
            g_useAutofixToCamera = !g_useAutofixToCamera;

        if ( _camera->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _camera->active() )
                engine::COpenGLApp::GetWindow()->disableCursor();
            else
                engine::COpenGLApp::GetWindow()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        engine::CDebugDrawer::DrawNormals( _cube3, { 0.0f, 0.0f, 1.0f } );

        /* use the light selected by the user */
        _currentLight = _ui->selectedLight();

        float _t = glfwGetTime();
        float _scaler = 1.0f;
        float _ctheta = std::cos( _t * _scaler );
        float _stheta = std::sin( _t * _scaler );
        float _cphi = std::cos( _t * _scaler );
        float _sphi = std::sin( _t * _scaler );

        engine::CVec3 _testDirection = { _sphi * _ctheta, _sphi * _stheta, _cphi };

        showDirectionalLightVolume( _cameraTest, _lightTest, _shadowmap );

        _app->beginRendering();
        _camera->update();

        /* do our thing here ************************/

        // render to shadow map first
        renderToShadowMap( _currentLight, _camera, _shadowmap, _shaderShadowMapProj, _floor, { _cube1, _cube2, _cube3 } );

        // render the scene normally
        renderSceneWithShadows( _currentLight, _camera, _shadowmap, _shaderPhongWithShadows, _floorMaterial, _cubeMaterial, _floor, { _cube1, _cube2, _cube3 } );

        // render the shadowmap to a quad
        renderShadowMapVisualization( _currentLight, _quad_varray, _shaderShadowMapViz, _shadowmap );

        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        if ( !_camera->active() )
            _app->renderUi();

        _app->endRendering();
    }

    return 0;
}

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CICamera* cameraPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::CIRenderable* floor,
                        std::vector< engine::CIRenderable* > cubes )
{
    // glCullFace( GL_FRONT );
    shaderPtr->bind();
    shadowMapPtr->bind();
    engine::CMat4 _lightViewMat, _lightProjMat;

    engine::CShadowMapRangeConfig _config;
    if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
        _config.dirLightPtr = dynamic_cast< engine::CDirectionalLight* >( lightPtr );
    if ( lightPtr->type() == engine::eLightType::POINT )
        _config.pointLightPtr = dynamic_cast< engine::CPointLight* >( lightPtr );
    if ( lightPtr->type() == engine::eLightType::SPOT )
        _config.spotLightPtr = dynamic_cast< engine::CSpotLight* >( lightPtr );

    if ( g_useAutofixToCamera && lightPtr->type() == engine::eLightType::DIRECTIONAL )
    {
        _config.type        = engine::eShadowRangeType::AUTOFIX_CAMERA;
        _config.worldUp     = g_worldUp;
        _config.cameraPtr   = cameraPtr;
        _config.extraWidth  = g_extraWidth;
        _config.extraHeight = g_extraHeight;
        _config.extraDepth  = g_extraDepth;
    }
    else
    {
        _config.type            = engine::eShadowRangeType::FIXED_USER;
        _config.worldUp         = g_worldUp;
        _config.focusPoint      = g_focusPoint;
        if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
        {
            _config.clipSpaceWidth  = g_widthDir;
            _config.clipSpaceHeight = g_heightDir;
            _config.clipSpaceDepth  = g_depthDir;
        }
        else if ( lightPtr->type() == engine::eLightType::POINT )
        {
            _config.clipSpaceZNear  = g_znearPoint;
            _config.clipSpaceZFar   = g_zfarPoint;
            _config.clipSpaceFov    = g_fovPoint;
        }
        else if ( lightPtr->type() == engine::eLightType::SPOT )
        {
            _config.clipSpaceZNear  = g_znearSpot;
            _config.clipSpaceZFar   = g_zfarSpot;
            _config.clipSpaceFov    = g_fovSpot;
        }
    }

    shadowMapPtr->setup( _config );

    _lightViewMat = shadowMapPtr->lightSpaceMatView();
    _lightProjMat = shadowMapPtr->lightSpaceMatProj();

    engine::CDebugDrawer::DrawClipVolume( cameraPtr->matProj() * cameraPtr->matView(), { 1.0f, 1.0f, 0.0f } );
    engine::CDebugDrawer::DrawClipVolume( _lightProjMat * _lightViewMat, { 0.7f, 0.5f, 0.3f } );

    shaderPtr->setMat4( "u_lightSpaceViewProjMatrix", _lightProjMat * _lightViewMat );

    {
        shaderPtr->setMat4( "u_modelMatrix", floor->matModel() );
        floor->render();
    }

    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        shaderPtr->setMat4( "u_modelMatrix", cubes[i]->matModel() );
        cubes[i]->render();
    }

    shadowMapPtr->unbind();
    shaderPtr->unbind();
    // glCullFace( GL_BACK );
}

void renderSceneWithShadows( engine::CILight* lightPtr, 
                             engine::CICamera* cameraPtr, 
                             engine::CShadowMap* shadowMapPtr,
                             engine::CShader* shaderPtr,
                             engine::CPhongMaterial* floorMaterialPtr,
                             engine::CPhongMaterial* cubeMaterialPtr,
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
    shaderPtr->setVec3( "u_viewerPosition", cameraPtr->position() );

    /* setup the light-clip-space transform */
    engine::CMat4 _lightViewMat, _lightProjMat;

    engine::CShadowMapRangeConfig _config;
    if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
        _config.dirLightPtr = dynamic_cast< engine::CDirectionalLight* >( lightPtr );
    if ( lightPtr->type() == engine::eLightType::POINT )
        _config.pointLightPtr = dynamic_cast< engine::CPointLight* >( lightPtr );
    if ( lightPtr->type() == engine::eLightType::SPOT )
        _config.spotLightPtr = dynamic_cast< engine::CSpotLight* >( lightPtr );

    if ( g_useAutofixToCamera && lightPtr->type() == engine::eLightType::DIRECTIONAL )
    {
        _config.type        = engine::eShadowRangeType::AUTOFIX_CAMERA;
        _config.worldUp     = g_worldUp;
        _config.cameraPtr   = cameraPtr;
        _config.extraWidth  = g_extraWidth;
        _config.extraHeight = g_extraHeight;
        _config.extraDepth  = g_extraDepth;
    }
    else
    {
        _config.type            = engine::eShadowRangeType::FIXED_USER;
        _config.worldUp         = g_worldUp;
        _config.focusPoint      = g_focusPoint;
        if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
        {
            _config.clipSpaceWidth  = g_widthDir;
            _config.clipSpaceHeight = g_heightDir;
            _config.clipSpaceDepth  = g_depthDir;
        }
        else if ( lightPtr->type() == engine::eLightType::POINT )
        {
            _config.clipSpaceZNear  = g_znearPoint;
            _config.clipSpaceZFar   = g_zfarPoint;
            _config.clipSpaceFov    = g_fovPoint;
        }
        else if ( lightPtr->type() == engine::eLightType::SPOT )
        {
            _config.clipSpaceZNear  = g_znearSpot;
            _config.clipSpaceZFar   = g_zfarSpot;
            _config.clipSpaceFov    = g_fovSpot;
        }
    }

    shadowMapPtr->setup( _config );

    _lightViewMat = shadowMapPtr->lightSpaceMatView();
    _lightProjMat = shadowMapPtr->lightSpaceMatProj();

    shaderPtr->setMat4( "u_viewProjLightSpaceMatrix", _lightProjMat * _lightViewMat );

    /* configure the texture unit for our shadowmap's depth texture (slot 3 in the shader) */
    shaderPtr->setInt( "u_depthmapTexture", 3 );
    glActiveTexture( GL_TEXTURE3 );
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();

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

    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->unbind();
    shaderPtr->unbind();
}

void renderShadowMapVisualization( engine::CILight* lightPtr,
                                   engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr )
{
    glDisable( GL_DEPTH_TEST );
    glViewport( 0, 0, 256, 256 );
    shaderPtr->bind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();
    quadVAO->bind();

    if ( lightPtr->type() == engine::eLightType::POINT || lightPtr->type() == engine::eLightType::SPOT )
    {
        shaderPtr->setInt( "u_linearizeDepth", 1 );
        shaderPtr->setFloat( "u_znear", g_znearPoint );
        shaderPtr->setFloat( "u_zfar", g_zfarPoint );
    }
    else
    {
        shaderPtr->setInt( "u_linearizeDepth", 0 );
    }

    glDrawElements( GL_TRIANGLES, quadVAO->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

    quadVAO->unbind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->unbind();
    shaderPtr->unbind();
    glEnable( GL_DEPTH_TEST );
    glViewport( 0, 0, engine::COpenGLApp::GetWindow()->width(), engine::COpenGLApp::GetWindow()->height() );
}

void showDirectionalLightVolume( engine::CICamera* cameraPtr,
                                 engine::CDirectionalLight* dirLightPtr,
                                 engine::CShadowMap* shadowMapPtr )
{
    engine::CShadowMapRangeConfig _config;
    _config.type        = engine::eShadowRangeType::AUTOFIX_CAMERA;
    _config.worldUp     = g_worldUp;
    _config.dirLightPtr = dirLightPtr;
    _config.cameraPtr   = cameraPtr;
    _config.extraWidth  = 0.0f;
    _config.extraHeight = 0.0f;
    _config.extraDepth  = 0.0f;

    shadowMapPtr->setup( _config );

    engine::CDebugDrawer::DrawClipVolume( cameraPtr->matProj() * cameraPtr->matView(), { 1.0f, 1.0f, 0.0f } );
    engine::CDebugDrawer::DrawClipVolume( shadowMapPtr->lightSpaceMatProj() * shadowMapPtr->lightSpaceMatView(), { 0.7f, 0.5f, 0.3f } );
}