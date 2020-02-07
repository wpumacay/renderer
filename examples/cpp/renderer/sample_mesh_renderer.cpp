
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

class MeshRendererGuiLayer : public engine::CImGuiLayer
{

public :

    MeshRendererGuiLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        m_rendererPtr = nullptr;
        m_meshRendererPtr = nullptr;

        auto _dirlight = std::make_unique<engine::CDirectionalLight>( "directional",
                                                                      engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                                      engine::CVec3( 0.4f, 0.4f, 0.4f ),
                                                                      engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                      g_lightDirDirection );

        auto _pointlight = std::make_unique<engine::CPointLight>( "point",
                                                                  engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                                  engine::CVec3( 0.5f, 0.5f, 0.5f ),
                                                                  engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                  g_lightPointPosition,
                                                                  1.0f, 0.05f, 0.005f );

        auto _spotlight = std::make_unique<engine::CSpotLight>( "spot",
                                                                engine::CVec3( 0.2f, 0.2f, 0.2f ),
                                                                engine::CVec3( 0.5f, 0.5f, 0.5f ),
                                                                engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                g_lightSpotPosition,
                                                                g_lightSpotDirection,
                                                                1.0f, 0.05f, 0.005f,
                                                                ENGINE_PI / 4.0f,
                                                                ENGINE_PI / 3.0f );

        m_lights.push_back( std::move( _dirlight ) ); m_lightsNames.push_back( "directional" );
        m_lights.push_back( std::move( _pointlight ) ); m_lightsNames.push_back( "point" );
        m_lights.push_back( std::move( _spotlight ) ); m_lightsNames.push_back( "spot" );

        m_lightSelectedIndex = 2;
        m_lightSelectedName = m_lightsNames[m_lightSelectedIndex];

        m_lightDirDirection     = g_lightDirDirection;
        m_lightPointDirection   = g_lightPointDirection;
        m_lightSpotDirection    = g_lightSpotDirection;

        m_lightPointPosition    = g_lightPointPosition;
        m_lightSpotPosition     = g_lightSpotPosition;

        m_wantsToCaptureMouse = false;
    }

    ~MeshRendererGuiLayer() {}

    void setRenderer( engine::CMainRenderer* rendererPtr )
    {
        m_rendererPtr = rendererPtr;
    }

    void setMeshRenderer( engine::CMeshRenderer* meshRendererPtr )
    {
        m_meshRendererPtr = meshRendererPtr;
    }

    void setMeshesScene0( const std::vector< engine::CMesh* >& meshes )
    {
        m_meshesScene0 = meshes;
    }

    void setMeshesScene1( const std::vector< engine::CMesh* >& meshes )
    {
        m_meshesScene1 = meshes;
    }

    engine::CILight* selectedLight() const { return m_lights[m_lightSelectedIndex].get(); }

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
                          engine::CTime::GetFpsAvgs().data(), 
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

        ImGui::RadioButton( "scene-0", &g_sceneId, 0 );
        ImGui::SameLine();
        ImGui::RadioButton( "scene-1", &g_sceneId, 1 );
        ImGui::Spacing();

        for ( auto meshPtr : m_meshesScene0 )
            meshPtr->setVisibility( ( g_sceneId == 0 ) );

        for ( auto meshPtr : m_meshesScene1 )
            meshPtr->setVisibility( ( g_sceneId == 1 ) );

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
        auto& _lightPtr = m_lights[m_lightSelectedIndex];
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

    std::vector< std::unique_ptr<engine::CILight> > m_lights;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;

    engine::CVec3 m_lightDirDirection;
    engine::CVec3 m_lightPointDirection;
    engine::CVec3 m_lightSpotDirection;

    engine::CVec3 m_lightPointPosition;
    engine::CVec3 m_lightSpotPosition;

    std::vector< engine::CMesh* > m_meshesScene0;
    std::vector< engine::CMesh* > m_meshesScene1;

    bool m_wantsToCaptureMouse;
};

void renderDepthMap( engine::CILight* lightPtr,
                     engine::CVertexArray* quadVAO,
                     engine::CShader* shaderPtr,
                     engine::CShadowMap* shadowMapPtr );

std::vector< engine::CMesh* > _createScene0();

std::vector< engine::CMesh* > _createScene1();

int main()
{
    auto _app = std::make_unique<engine::CApplication>();
    auto _ui = std::make_unique<MeshRendererGuiLayer>( "Mesh-renderer-utils" );
    auto _uiRef = dynamic_cast<MeshRendererGuiLayer*>( _app->addGuiLayer( std::move( _ui ) ) );
    _uiRef->setRenderer( _app->renderer() );
    _uiRef->setMeshRenderer( _app->renderer()->meshRenderer() );

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

    auto _cameraRef = _app->scene()->AddCamera( std::move( _camera ) );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapVizRef = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                                 _baseNameShadowMapViz + "_vs.glsl",
                                                                                 _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapVizRef, "Couldn't load the visualization shader to check the depth-map T_T" );

    auto _meshesScene0 = _createScene0();
    auto _meshesScene1 = _createScene1();

    _uiRef->setMeshesScene0( _meshesScene0 );
    _uiRef->setMeshesScene1( _meshesScene1 );

    for ( auto meshPtr : _meshesScene0 )
        meshPtr->setVisibility( ( g_sceneId == 0 ) );

    for ( auto meshPtr : _meshesScene1 )
        meshPtr->setVisibility( ( g_sceneId == 1 ) );

    // configure render options
    g_renderOptions.mode = engine::eRenderMode::NORMAL;
    g_renderOptions.useFrustumCulling = true;
    g_renderOptions.cullingGeom = engine::eCullingGeom::BOUNDING_BOX;
    g_renderOptions.useFaceCulling = false;
    g_renderOptions.useShadowMapping = true;
    g_renderOptions.viewportWidth = _app->window()->width();
    g_renderOptions.viewportHeight = _app->window()->height();
    g_renderOptions.cameraPtr = _cameraRef;
    g_renderOptions.lightPtr = _uiRef->selectedLight();
    g_renderOptions.shadowMapPtr = _app->renderer()->shadowMap();
    g_renderOptions.renderTargetPtr = nullptr;

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

    auto _quad_vbuffer = std::make_unique<engine::CVertexBuffer>( _layout, engine::eBufferUsage::STATIC,
                                                                  sizeof( _quad_buffData ), _quad_buffData );

    auto _quad_ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6, _quad_indices );

    auto _quad_varray = std::make_unique<engine::CVertexArray>();
    _quad_varray->addVertexBuffer( std::move( _quad_vbuffer ) );
    _quad_varray->setIndexBuffer( std::move( _quad_ibuffer ) );

    while( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _cameraRef->setActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _cameraRef->setActiveMode( true );

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

        // use user-selected light
        auto lightPtr = g_renderOptions.lightPtr = _uiRef->selectedLight();

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
            _config.cameraPtr   = _cameraRef;
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
        _app->renderer()->submit( _app->scene()->GetRenderablesList() );
        _app->render();

        // visualize the shadow-map
        renderDepthMap( lightPtr, _quad_varray.get(), _shaderShadowMapVizRef, _app->renderer()->shadowMap() );

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

std::vector< engine::CMesh* > _createScene0()
{
    auto _meshes = std::vector< std::unique_ptr<engine::CMesh> >();
    auto _meshesRefs = std::vector< engine::CMesh* >();

    _meshes.push_back( std::move( engine::CMeshBuilder::createPlane( 20.0f, 20.0f, engine::eAxis::Y ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createSphere( 0.5f ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createEllipsoid( 0.2f, 0.4f, 0.6f ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::X ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Y ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Z ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::X ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Y ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Z ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::X ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Y ) ) );
    _meshes.push_back( std::move( engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Z ) ) );

    _meshes[1]->position = { -1.0f, 1.0f, -1.0f };
    _meshes[2]->position = { -1.0f, 1.0f,  1.0f };
    _meshes[3]->position = {  1.0f, 1.0f,  1.0f };
    _meshes[4]->position = {  1.0f, 1.0f, -1.0f };

    _meshes[5]->position = { -1.0f, 2.0f, -1.0f };
    _meshes[6]->position = { -1.0f, 2.0f,  1.0f };
    _meshes[7]->position = {  1.0f, 2.0f,  1.0f };
    _meshes[8]->position = {  1.0f, 2.0f, -1.0f };

    _meshes[9]->position  = { -1.0f, 3.0f, -1.0f };
    _meshes[10]->position = { -1.0f, 3.0f,  1.0f };
    _meshes[11]->position = {  1.0f, 3.0f,  1.0f };
    _meshes[12]->position = {  1.0f, 3.0f, -1.0f };

    auto _sceneRef = engine::CApplication::GetInstance()->scene();
    for ( auto& meshPtr : _meshes )
        _meshesRefs.push_back( dynamic_cast<engine::CMesh*>( _sceneRef->AddRenderable( std::move( meshPtr ) ) ) );

    auto _meshTextureRef = engine::CTextureManager::GetCachedTexture( "img_grid" );

    // give the renderables a little rotation and scale
    std::default_random_engine _randomGenerator;
    std::uniform_real_distribution< float > _randomDistribution( 0.5f, 1.0f );
    for ( auto meshPtr : _meshesRefs )
    {
        if ( meshPtr == _meshesRefs.front() ) // leave plane unchanged
        {
            meshPtr->material()->setAlbedoMap( _meshTextureRef );
            continue;
        }

        meshPtr->rotation = tinymath::rotation( engine::CVec3( _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                               _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                               _randomDistribution( _randomGenerator ) * (float) ENGINE_PI ) );
        float _scale = _randomDistribution( _randomGenerator );
        meshPtr->scale = { _scale, _scale, _scale };

        meshPtr->material()->setAlbedoMap( _meshTextureRef );
    }

    return _meshesRefs;
}

std::vector< engine::CMesh* > _createScene1()
{
    auto _meshes = std::vector< std::unique_ptr<engine::CMesh> >();
    auto _meshesRefs = std::vector< engine::CMesh* >();

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

    auto _floorTextureRef = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTextureRef = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );

    ENGINE_ASSERT( _floorTextureRef, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTextureRef, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = std::make_unique<engine::CMaterial>( "floor_material",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTextureRef,
                                                               _floorTextureRef );

    auto _cube1Material = std::make_unique<engine::CMaterial>( "cube_material1",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTextureRef,
                                                               _floorTextureRef );

    auto _cube2Material = std::make_unique<engine::CMaterial>( "cube_material2",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTextureRef,
                                                               _floorTextureRef );

    auto _cube3Material = std::make_unique<engine::CMaterial>( "cube_material3",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTextureRef,
                                                               _floorTextureRef );

    _floor->setMaterial( std::move( _floorMaterial ) );
    _cube1->setMaterial( std::move( _cube1Material ) );
    _cube2->setMaterial( std::move( _cube2Material ) );
    _cube3->setMaterial( std::move( _cube3Material ) );

    _meshes.push_back( std::move( _floor ) );
    _meshes.push_back( std::move( _cube1 ) );
    _meshes.push_back( std::move( _cube2 ) );
    _meshes.push_back( std::move( _cube3 ) );

    auto _sceneRef = engine::CApplication::GetInstance()->scene();
    for ( auto& meshPtr : _meshes )
        _meshesRefs.push_back( dynamic_cast<engine::CMesh*>( _sceneRef->AddRenderable( std::move( meshPtr ) ) ) );

    return _meshesRefs;
}
