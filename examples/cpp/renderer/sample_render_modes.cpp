
#include <CEngine.h>

engine::CRenderOptions g_renderOptions;

engine::CVec3 g_lightDirPosition    = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightPointPosition  = { 0.0f, 5.0f, 0.0f };
engine::CVec3 g_lightSpotPosition   = { 0.0f, 5.0f, 0.0f };

// engine::CVec3 g_lightDirPosition    = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPointPosition  = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightSpotPosition   = { -2.0f, 4.0f, -1.0f };

engine::CVec3 g_lightDirDirection   = ( -g_lightDirPosition ).normalized();
engine::CVec3 g_lightPointDirection = ( -g_lightPointPosition ).normalized();
engine::CVec3 g_lightSpotDirection  = ( -g_lightSpotPosition ).normalized();

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

int g_sceneId = 0;
int g_renderMode = 0;
int g_numRenderables = 0;

class RenderModesGuiLayer : public engine::CImGuiLayer
{

public :

    RenderModesGuiLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        m_rendererPtr = nullptr;
        m_meshRendererPtr = nullptr;

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

        m_wantsToCaptureMouse = false;
    }

    ~RenderModesGuiLayer() {}

    void setRenderer( engine::CMainRenderer* rendererPtr )
    {
        m_rendererPtr = rendererPtr;
    }

    void setMeshRenderer( engine::CMeshRenderer* meshRendererPtr )
    {
        m_meshRendererPtr = meshRendererPtr;
    }

    void setRenderablesScene0( const std::vector< engine::CIRenderable* >& renderables )
    {
        m_renderablesScene0 = renderables;
    }

    void setRenderablesScene1( const std::vector< engine::CIRenderable* >& renderables )
    {
        m_renderablesScene1 = renderables;
    }

    engine::CILight* selectedLight() const { return m_lights[m_lightSelectedIndex]; }

    void render() override
    {
        m_wantsToCaptureMouse = false;

        _menuUiRendererStats();
        _menuUiLights();
        _menuUiRendererScene();
        _menuUiRendererShadowMap();

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

    void _menuUiRendererStats()
    {
        ImGui::Begin( "statistics" );
        ImGui::Text( "fps-avg       : %.2f", 1.0f / engine::CTime::GetAvgTimeStep() );
        ImGui::Text( "frame-time-avg: %.5f", engine::CTime::GetAvgTimeStep() );
        ImGui::PlotLines( "fps-avg", 
                          engine::CTime::GetFpsAvgs(), 
                          engine::CTime::GetNumFramesForAvg(), 
                          engine::CTime::GetFrameTimeIndex(),
                          ( std::string( "average: " ) + std::to_string( 1.0f / engine::CTime::GetAvgTimeStep() ) ).c_str(),
                          0.0f, FLT_MAX, ImVec2( 0, 120 ) );
        ImGui::End();
    }

    void _menuUiRendererScene()
    {
        if ( !m_rendererPtr )
            return;

        ImGui::Begin( "Scene" );

        ImGui::TextColored( ImVec4( 1.0f, 1.0f, 0.0f, 1.0f ), "Scene:" );
        ImGui::RadioButton( "scene-0", &g_sceneId, 0 ); ImGui::SameLine();
        ImGui::RadioButton( "scene-1", &g_sceneId, 1 ); ImGui::Spacing();

        ImGui::TextColored( ImVec4( 0.0f, 1.0f, 1.0f, 1.0f ), "Render-mode:" );
        ImGui::RadioButton( "NORMAL", &g_renderMode, 0 ); ImGui::SameLine();
        ImGui::RadioButton( "DEPTH_ONLY", &g_renderMode, 1 ); ImGui::SameLine();
        ImGui::RadioButton( "SEMANTIC_ONLY", &g_renderMode, 2 ); ImGui::SameLine();
        ImGui::RadioButton( "NO_SUBMIT", &g_renderMode, 3 ); ImGui::Spacing();

        if ( g_renderMode == 0 )
            g_renderOptions.mode = engine::eRenderMode::NORMAL;
        else if ( g_renderMode == 1 )
            g_renderOptions.mode = engine::eRenderMode::DEPTH_ONLY;
        else if ( g_renderMode == 2 )
            g_renderOptions.mode = engine::eRenderMode::SEMANTIC_ONLY;
        else if ( g_renderMode == 3 )
            g_renderOptions.mode = engine::eRenderMode::NO_SUBMIT;

        for ( auto renderablePtr : m_renderablesScene0 )
            renderablePtr->setVisibility( ( g_sceneId == 0 ) );

        for ( auto renderablePtr : m_renderablesScene1 )
            renderablePtr->setVisibility( ( g_sceneId == 1 ) );

        ImGui::Checkbox( "use-frustum-culling", &g_renderOptions.useFrustumCulling );
        if ( g_renderOptions.useFrustumCulling )
        {
            int _cullingGeomOption = ( g_renderOptions.cullingGeom == engine::eCullingGeom::BOUNDING_BOX ) ? 0 : 1;
            ImGui::RadioButton( "Box", &_cullingGeomOption, 0 );
            ImGui::SameLine();
            ImGui::RadioButton( "Sphere", &_cullingGeomOption, 1 );
            g_renderOptions.cullingGeom = ( _cullingGeomOption == 0 ) ? engine::eCullingGeom::BOUNDING_BOX : engine::eCullingGeom::BOUNDING_SPHERE;
        }

        ImGui::Spacing();
        ImGui::Text( m_rendererPtr->status().c_str() );
        ImGui::End();
    }

    void _menuUiRendererShadowMap()
    {
        if ( !m_rendererPtr )
            return;

        // grab depth-map texture from shadowmap
        auto _depthMapTexture = m_rendererPtr->shadowMap()->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" );

        if ( !_depthMapTexture )
            return;

        ImGui::Begin( "shadow-map" );

        ImGui::Image( (void*)(intptr_t) _depthMapTexture->openglId(),
                      ImVec2( 200.0, 200.0 ),
                      ImVec2( 0.0, 0.0 ), ImVec2( 1.0, 1.0 ) );

        ImGui::End();
    }

    void _menuUiLights()
    {
        ImGui::Begin( "Lights" );

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
            float _direction[3] = { m_lightDirDirection.x(), m_lightDirDirection.y(), m_lightDirDirection.z() };
            ImGui::SliderFloat3( "direction", _direction, -10.0f, 10.0f );
            m_lightDirDirection = { _direction[0], _direction[1], _direction[2] };

            g_lightDirDirection = m_lightDirDirection.normalized();
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

                float _focusp[3] = { g_focusPoint.x(), g_focusPoint.y(), g_focusPoint.z() };
                ImGui::SliderFloat3( "focus", _focusp, -10.0f, 10.0f );
                g_focusPoint = { _focusp[0], _focusp[1], _focusp[2] };
            }
        }
        else if ( _lightPtr->type() == engine::eLightType::POINT )
        {
            float _position[3] = { m_lightPointPosition.x(), m_lightPointPosition.y(), m_lightPointPosition.z() };
            ImGui::SliderFloat3( "position", _position, -10.0f, 10.0f );
            m_lightPointPosition = { _position[0], _position[1], _position[2] };

            g_lightPointDirection = m_lightPointDirection.normalized();
            g_lightPointPosition = m_lightPointPosition;
            _lightPtr->position = g_lightPointPosition;

            ImGui::SliderFloat( "znear", &g_znearPoint, 0.1f, 5.0f );
            ImGui::SliderFloat( "zfar", &g_zfarPoint, g_znearPoint, 20.0f );
            ImGui::SliderFloat( "fov", &g_fovPoint, 20.0f, 150.0f );

            float _focusp[3] = { g_focusPoint.x(), g_focusPoint.y(), g_focusPoint.z() };
            ImGui::SliderFloat3( "focus", _focusp, -10.0f, 10.0f );
            g_focusPoint = { _focusp[0], _focusp[1], _focusp[2] };
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

            ImGui::SliderFloat( "znear", &g_znearSpot, 0.1f, 5.0f );
            ImGui::SliderFloat( "zfar", &g_zfarSpot, g_znearSpot, 20.0f );
            ImGui::SliderFloat( "fov", &g_fovSpot, 20.0f, 150.0f );
        }
    }

    engine::CMainRenderer* m_rendererPtr;
    engine::CMeshRenderer* m_meshRendererPtr;

    std::vector< engine::CILight* > m_lights;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;

    engine::CVec3 m_lightDirDirection;
    engine::CVec3 m_lightPointDirection;
    engine::CVec3 m_lightSpotDirection;

    engine::CVec3 m_lightPointPosition;
    engine::CVec3 m_lightSpotPosition;

    std::vector< engine::CIRenderable* > m_renderablesScene0;
    std::vector< engine::CIRenderable* > m_renderablesScene1;

    bool m_wantsToCaptureMouse;
};

void renderDepthMap( engine::CILight* lightPtr,
                     engine::CVertexArray* quadVAO,
                     engine::CShader* shaderPtr,
                     engine::CShadowMap* shadowMapPtr );

std::vector< engine::CIRenderable* > _createScene0();
std::vector< engine::CIRenderable* > _createScene1();

int main()
{
    auto _app = new engine::CApplication();
    auto _ui = new RenderModesGuiLayer( "RenderModes-utils" );
    _app->addGuiLayer( std::unique_ptr< RenderModesGuiLayer >( _ui ) );
    _ui->setRenderer( _app->renderer() );
    _ui->setMeshRenderer( _app->renderer()->meshRenderer() );

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

    _app->scene()->addCamera( std::unique_ptr< engine::CICamera >( _camera ) );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapViz = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                             _baseNameShadowMapViz + "_vs.glsl",
                                                                             _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapViz, "Couldn't load the visualization shader to check the depth-map T_T" );

    auto _renderablesScene0 = _createScene0();
    auto _renderablesScene1 = _createScene1();

    _ui->setRenderablesScene0( _renderablesScene0 );
    _ui->setRenderablesScene1( _renderablesScene1 );

    for ( auto renderablePtr : _renderablesScene0 )
        renderablePtr->setVisibility( ( g_sceneId == 0 ) );

    for ( auto renderablePtr : _renderablesScene1 )
        renderablePtr->setVisibility( ( g_sceneId == 1 ) );

    // configure render options
    g_renderOptions.mode = engine::eRenderMode::NORMAL;
    g_renderOptions.useFrustumCulling = true;
    g_renderOptions.cullingGeom = engine::eCullingGeom::BOUNDING_BOX;
    g_renderOptions.useFaceCulling = false;
    g_renderOptions.useShadowMapping = true;
    g_renderOptions.viewportWidth = _app->window()->width();
    g_renderOptions.viewportHeight = _app->window()->height();
    g_renderOptions.cameraPtr = _camera;
    g_renderOptions.lightPtr = _ui->selectedLight();
    g_renderOptions.shadowMapPtr = _app->renderer()->shadowMap();
    g_renderOptions.renderTargetPtr = nullptr;

    g_renderOptions.depthViewZmin = 0.0f;
    g_renderOptions.depthViewZmax = 20.0f;
    g_renderOptions.depthViewZminColor = { 1.0f, 1.0f, 1.0f };
    g_renderOptions.depthViewZmaxColor = { 0.0f, 0.0f, 0.0f };

    g_renderOptions.semanticViewIdMap = { { 0, { 0.5f, 0.7f, 0.3f } },
                                          { 1, { 0.3f, 0.5f, 0.7f } },
                                          { 2, { 0.7f, 0.3f, 0.5f } } };

    /* vao used for shadow-mapping visualization */
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

        // use user-selected light
        auto lightPtr = g_renderOptions.lightPtr = _ui->selectedLight();

        // configure shadow-map range
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
            _config.cameraPtr   = _camera;
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
        g_renderOptions.shadowMapRangeConfig = _config;

        _app->update();
        _app->begin();

        /****************************************************/
        // render our scene
        _app->renderer()->begin( g_renderOptions );
        _app->renderer()->submit( _app->scene()->renderables() );
        _app->render();

        // visualize the shadow-map
        renderDepthMap( lightPtr, _quad_varray, _shaderShadowMapViz, _app->renderer()->shadowMap() );

        /****************************************************/

        _app->end();
    }

    return 0;
}

void renderDepthMap( engine::CILight* lightPtr,
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
    glViewport( 0, 0, engine::CApplication::GetInstance()->window()->width(), engine::CApplication::GetInstance()->window()->height() );
}

std::vector< engine::CIRenderable* > _createScene0()
{
    auto _renderables = std::vector< engine::CIRenderable* >();

    _renderables.push_back( engine::CMeshBuilder::createPlane( 20.0f, 20.0f, engine::eAxis::Y ) );
    _renderables.push_back( engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f ) );
    _renderables.push_back( engine::CMeshBuilder::createSphere( 0.5f ) );
    _renderables.push_back( engine::CMeshBuilder::createEllipsoid( 0.2f, 0.4f, 0.6f ) );
    _renderables.push_back( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::X ) );
    _renderables.push_back( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Y ) );
    _renderables.push_back( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Z ) );
    _renderables.push_back( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::X ) );
    _renderables.push_back( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Y ) );
    _renderables.push_back( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Z ) );
    _renderables.push_back( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::X ) );
    _renderables.push_back( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Y ) );
    _renderables.push_back( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Z ) );
    _renderables.push_back( engine::CMeshBuilder::createAxes( 0.5f ) );
    _renderables.push_back( engine::CMeshBuilder::createModelFromFile( ENGINE_RESOURCES_PATH + std::string( "models/chassis.stl" ) ) );

    _renderables[1]->position = { -1.0f, 1.0f, -1.0f };
    _renderables[2]->position = { -1.0f, 1.0f,  1.0f };
    _renderables[3]->position = {  1.0f, 1.0f,  1.0f };
    _renderables[4]->position = {  1.0f, 1.0f, -1.0f };

    _renderables[5]->position = { -1.0f, 2.0f, -1.0f };
    _renderables[6]->position = { -1.0f, 2.0f,  1.0f };
    _renderables[7]->position = {  1.0f, 2.0f,  1.0f };
    _renderables[8]->position = {  1.0f, 2.0f, -1.0f };

    _renderables[9]->position  = { -1.0f, 3.0f, -1.0f };
    _renderables[10]->position = { -1.0f, 3.0f,  1.0f };
    _renderables[11]->position = {  1.0f, 3.0f,  1.0f };
    _renderables[12]->position = {  1.0f, 3.0f, -1.0f };

    _renderables[13]->position = { 0.0f, 1.0f, 0.0f };
    _renderables[14]->position = { 0.0f, 2.0f, 0.0f };

    for ( auto renderablePtr : _renderables )
    {
        renderablePtr->setMaskId( g_numRenderables );
        renderablePtr->setObjectId( g_numRenderables );
        g_numRenderables++;

        engine::CApplication::GetInstance()->scene()->addRenderable( std::unique_ptr< engine::CIRenderable >( renderablePtr ) );
    }

    auto _renderableTexture = engine::CTextureManager::GetCachedTexture( "img_grid" );

    // give the renderables a little rotation and scale
    std::default_random_engine _randomGenerator;
    std::uniform_real_distribution< float > _randomDistribution( 0.5f, 1.0f );
    for ( auto renderablePtr : _renderables )
    {
        if ( renderablePtr == _renderables.front() ) // leave plane unchanged
        {
            renderablePtr->material()->setAlbedoMap( _renderableTexture );
            continue;
        }

        renderablePtr->rotation = tinymath::rotation( engine::CVec3( _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                     _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                     _randomDistribution( _randomGenerator ) * (float) ENGINE_PI ) );
        float _scale = _randomDistribution( _randomGenerator );
        renderablePtr->scale = { _scale, _scale, _scale };

        renderablePtr->material()->setAlbedoMap( _renderableTexture );
        if ( renderablePtr->type() == engine::eRenderableType::MODEL )
        {
            auto _submeshes = dynamic_cast< engine::CModel* >( renderablePtr )->meshes();
            for ( auto submeshPtr : _submeshes )
                submeshPtr->material()->setAlbedoMap( _renderableTexture );
        }
    }

    return _renderables;
}

std::vector< engine::CIRenderable* > _createScene1()
{
    auto _renderables = std::vector< engine::CIRenderable* >();

    auto _floor = engine::CMeshBuilder::createPlane( 30.0f, 30.0f, engine::eAxis::Y );
    _floor->position = { 0.0f, 0.0f, 0.0f };

    auto _cube1 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube1->position = { 0.0f, 2.0f, 0.0f };

    auto _cube2 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube2->position = { 2.0f, 0.5f, 1.0f };

    auto _cube3 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube3->position = { -1.0f, 0.5f, 2.0f };
    _cube3->rotation = tinymath::rotation( engine::CVec3( 1.0f, 0.0f, 1.0f ), engine::toRadians( 30.0f ) );
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

    auto _cube1Material = new engine::CMaterial( "cube_material1",
                                                 engine::eMaterialType::PHONG,
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 64.0f,
                                                 _floorTexture,
                                                 _floorTexture );

    auto _cube2Material = new engine::CMaterial( "cube_material2",
                                                 engine::eMaterialType::PHONG,
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 64.0f,
                                                 _floorTexture,
                                                 _floorTexture );

    auto _cube3Material = new engine::CMaterial( "cube_material3",
                                                 engine::eMaterialType::PHONG,
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 { 1.0f, 1.0f, 1.0f },
                                                 64.0f,
                                                 _floorTexture,
                                                 _floorTexture );

    _floor->setMaterial( std::unique_ptr< engine::CMaterial >( _floorMaterial ) );
    _cube1->setMaterial( std::unique_ptr< engine::CMaterial >( _cube1Material ) );
    _cube2->setMaterial( std::unique_ptr< engine::CMaterial >( _cube2Material ) );
    _cube3->setMaterial( std::unique_ptr< engine::CMaterial >( _cube3Material ) );

    _renderables = { _floor, _cube1, _cube2, _cube3 };

    for ( auto renderablePtr : _renderables )
    {
        renderablePtr->setMaskId( g_numRenderables );
        renderablePtr->setObjectId( g_numRenderables );
        g_numRenderables++;

        engine::CApplication::GetInstance()->scene()->addRenderable( std::unique_ptr< engine::CIRenderable >( renderablePtr ) );
    }

    return _renderables;
}
