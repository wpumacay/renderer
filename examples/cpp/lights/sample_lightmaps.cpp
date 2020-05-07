
#include <CEngine.h>

class GuiLightmapsLayer : public engine::CImGuiLayer
{

public :

    GuiLightmapsLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        m_lightRef = nullptr;
        m_materialRef = nullptr;
        m_lightsAnimated = false;

        auto _box       = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
        auto _sphere    = engine::CMeshBuilder::createSphere( 1.5f );
        auto _ellipsoid = engine::CMeshBuilder::createEllipsoid( 1.0f, 1.25f, 1.5f );
        auto _cylinderX = engine::CMeshBuilder::createCylinder( 0.5f, 1.0f, engine::eAxis::X );
        auto _cylinderY = engine::CMeshBuilder::createCylinder( 0.5f, 1.0f, engine::eAxis::Y );
        auto _cylinderZ = engine::CMeshBuilder::createCylinder( 0.5f, 1.0f, engine::eAxis::Z );
        auto _capsuleX  = engine::CMeshBuilder::createCapsule( 0.5f, 1.0f, engine::eAxis::X );
        auto _capsuleY  = engine::CMeshBuilder::createCapsule( 0.5f, 1.0f, engine::eAxis::Y );
        auto _capsuleZ  = engine::CMeshBuilder::createCapsule( 0.5f, 1.0f, engine::eAxis::Z );
        auto _arrowX    = engine::CMeshBuilder::createArrow( 1.0f, engine::eAxis::X );
        auto _arrowY    = engine::CMeshBuilder::createArrow( 1.0f, engine::eAxis::Y );
        auto _arrowZ    = engine::CMeshBuilder::createArrow( 1.0f, engine::eAxis::Z );
        auto _axes      = engine::CMeshBuilder::createAxes( 1.0f );

        m_meshes.push_back( std::move( _box ) ); m_meshesNames.push_back( "box" );
        m_meshes.push_back( std::move( _sphere ) ); m_meshesNames.push_back( "sphere" );
        m_meshes.push_back( std::move( _ellipsoid ) ); m_meshesNames.push_back( "ellipsoid" );
        m_meshes.push_back( std::move( _cylinderX ) ); m_meshesNames.push_back( "cylinderX" );
        m_meshes.push_back( std::move( _cylinderY ) ); m_meshesNames.push_back( "cylinderY" );
        m_meshes.push_back( std::move( _cylinderZ ) ); m_meshesNames.push_back( "cylinderZ" );
        m_meshes.push_back( std::move( _capsuleX ) ); m_meshesNames.push_back( "capsuleX" );
        m_meshes.push_back( std::move( _capsuleY ) ); m_meshesNames.push_back( "capsuleY" );
        m_meshes.push_back( std::move( _capsuleZ ) ); m_meshesNames.push_back( "capsuleZ" );
        m_meshes.push_back( std::move( _arrowX ) ); m_meshesNames.push_back( "arrowX" );
        m_meshes.push_back( std::move( _arrowY ) ); m_meshesNames.push_back( "arrowY" );
        m_meshes.push_back( std::move( _arrowZ ) ); m_meshesNames.push_back( "arrowZ" );
        m_meshes.push_back( std::move( _axes ) ); m_meshesNames.push_back( "axes" );

        m_meshSelectedName = "box";
        m_meshSelectedIndex = 0;
        m_meshRotationAngle = 0.0f;

        m_cachedTextures = engine::CTextureManager::GetAllCachedTextures();
        m_cachedTextures.push_back( nullptr );
        m_currentAlbedoMapName = "none";
        m_currentSpecularMapName = "none";
        m_currentAlbedoMap = nullptr;
        m_currentSpecularMap = nullptr;

        m_anyMenuHovered = false;
        m_wantsToCaptureMouse = false;
    }

    ~GuiLightmapsLayer() 
    {
        m_meshes.clear();
        m_cachedTextures.clear();
    }

    void setMaterial( engine::CMaterial* material )
    {
        m_materialRef = material;
    }

    void setLight( engine::CILight* light )
    {
        m_lightRef = light;
    }

    engine::CIRenderable* selectedMesh()
    {
        return m_meshes[ m_meshSelectedIndex ].get();
    }

    void render() override
    {
        m_anyMenuHovered = false;
        m_wantsToCaptureMouse = false;

        _menuUiMaterial();
        _menuUiLight();
        _menuUiGeometry();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_anyMenuHovered || m_wantsToCaptureMouse;

        return false;
    }

private :

    void _menuUiMaterial()
    {
        if ( !m_materialRef )
            return;

        ImGui::Begin( "Material-properties" );

        /* diffuse properties (phong and lambert materials) */
        if ( m_materialRef->type() == engine::eMaterialType::PHONG || m_materialRef->type() == engine::eMaterialType::LAMBERT )
        {
            if ( ImGui::BeginCombo( "Albedo-map", m_currentAlbedoMapName.c_str() ) )
            {
                for ( auto _cachedTexture : m_cachedTextures )
                {
                    std::string _textureName = ( _cachedTexture ) ? _cachedTexture->name() : "none";
                    bool _isSelected = ( _textureName == m_currentAlbedoMapName );
    
                    if ( ImGui::Selectable( _textureName.c_str(), _isSelected ) )
                    {
                        m_currentAlbedoMap = _cachedTexture;
                        m_currentAlbedoMapName = _textureName;
                    }
    
                    if ( _isSelected )
                        ImGui::SetItemDefaultFocus();
                }
    
                ImGui::EndCombo();
            }

            m_materialRef->setAlbedoMap( m_currentAlbedoMap );

            if ( m_currentAlbedoMap )
                ImGui::Image( (void*)(intptr_t) m_currentAlbedoMap->openglId(), ImVec2( 64, 64 ) );

            _menuUiDiffuseProps();
        }

        /* specular properties (phong material only) */
        if ( m_materialRef->type() == engine::eMaterialType::PHONG )
        {
            if ( ImGui::BeginCombo( "Specular-map", m_currentSpecularMapName.c_str() ) )
            {
                for ( auto _cachedTexture : m_cachedTextures )
                {
                    std::string _textureName = ( _cachedTexture ) ? _cachedTexture->name() : "none";
                    bool _isSelected = ( _textureName == m_currentSpecularMapName );

                    if ( ImGui::Selectable( _textureName.c_str(), _isSelected ) )
                    {
                        m_currentSpecularMap = _cachedTexture;
                        m_currentSpecularMapName = _textureName;
                    }

                    if ( _isSelected )
                        ImGui::SetItemDefaultFocus();
                }
            }

            m_materialRef->setSpecularMap( m_currentSpecularMap );

            if ( m_currentSpecularMap )
                ImGui::Image( (void*)(intptr_t) m_currentSpecularMap->openglId(), ImVec2( 64, 64 ) );

            _menuUiSpecularProps();
        }

        ImGui::Spacing();
        ImGui::Text( m_materialRef->toString().c_str() );

        m_anyMenuHovered |= ImGui::IsWindowHovered();

        ImGui::End();
    }

    void _menuUiDiffuseProps()
    {
        float _cAmbient[3]  = { m_materialRef->ambient.x(), m_materialRef->ambient.y(), m_materialRef->ambient.z() };
        float _cDiffuse[3]  = { m_materialRef->diffuse.x(), m_materialRef->diffuse.y(), m_materialRef->diffuse.z() };
        ImGui::ColorEdit3( "cAmbient", _cAmbient );
        ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
        m_materialRef->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
        m_materialRef->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
    }

    void _menuUiSpecularProps()
    {
        float _cSpecular[3] = { m_materialRef->specular.x(), m_materialRef->specular.y(), m_materialRef->specular.z() };
        ImGui::ColorEdit3( "cSpecular", _cSpecular );
        ImGui::SliderFloat( "cShininess", &m_materialRef->shininess, 32.0f, 256.0f );
        m_materialRef->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
    }

    void _menuUiLight()
    {
        if ( !m_lightRef )
            return;

        ImGui::Begin( "Light-color-properties" );

        ImGui::Checkbox( "funky???", &m_lightsAnimated );

        if ( m_lightsAnimated )
        {
            m_lightRef->ambient.x() = 0.1f * std::sin( tinyutils::Clock::GetWallTime() * 2.0f ) + 0.1f;
            m_lightRef->ambient.y() = 0.1f * std::sin( tinyutils::Clock::GetWallTime() * 0.7f ) + 0.1f;
            m_lightRef->ambient.z() = 0.1f * std::sin( tinyutils::Clock::GetWallTime() * 1.3f ) + 0.1f;
            m_lightRef->diffuse = 5.0f * m_lightRef->ambient;
            m_lightRef->specular = { 1.0f, 1.0f, 1.0f };
        }
        else
        {
            float _cAmbient[3]  = { m_lightRef->ambient.x(), m_lightRef->ambient.y(), m_lightRef->ambient.z() };
            float _cDiffuse[3]  = { m_lightRef->diffuse.x(), m_lightRef->diffuse.y(), m_lightRef->diffuse.z() };
            float _cSpecular[3] = { m_lightRef->specular.x(), m_lightRef->specular.y(), m_lightRef->specular.z() };

            ImGui::ColorEdit3( "cAmbient", _cAmbient );
            ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
            ImGui::ColorEdit3( "cSpecular", _cSpecular );
            ImGui::SliderFloat( "cIntensity", &m_lightRef->intensity, 0.1f, 1.0f );

            m_lightRef->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
            m_lightRef->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
            m_lightRef->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
        }

        ImGui::Spacing();
        ImGui::Text( m_lightRef->toString().c_str() );

        m_anyMenuHovered |= ImGui::IsWindowHovered();

        ImGui::End();
    }

    void _menuUiGeometry()
    {
        ImGui::Begin( "Mesh-selection" );

        if ( ImGui::BeginCombo( "Geometries", m_meshSelectedName.c_str() ) )
        {
            for ( size_t i = 0; i < m_meshes.size(); i++ )
            {
                std::string _meshName = m_meshesNames[i];
                bool _isSelected = ( _meshName == m_meshSelectedName );

                if ( ImGui::Selectable( _meshName.c_str(), _isSelected ) )
                {
                    m_meshSelectedName = _meshName;
                    m_meshSelectedIndex = i;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if ( m_meshSelectedIndex != -1 )
        {
            ImGui::SliderFloat( "angle", &m_meshRotationAngle, 0.0f, 2.0f * ENGINE_PI );
            m_meshes[m_meshSelectedIndex]->rotation = tinymath::rotation( engine::CVec3( 0.0f, 1.0f, 0.0f ), m_meshRotationAngle );
        }

        m_anyMenuHovered |= ImGui::IsWindowHovered();

        ImGui::End();
    }

    engine::CMaterial* m_materialRef;
    engine::CILight* m_lightRef;
    bool m_lightsAnimated;

    std::string m_currentAlbedoMapName;
    std::string m_currentSpecularMapName;
    engine::CTexture* m_currentAlbedoMap;
    engine::CTexture* m_currentSpecularMap;
    std::vector< engine::CTexture* > m_cachedTextures;

    std::vector< std::unique_ptr<engine::CIRenderable> > m_meshes;
    std::vector< std::string > m_meshesNames;
    std::string m_meshSelectedName;
    int m_meshSelectedIndex;

    float m_meshRotationAngle;

    bool m_anyMenuHovered;
    bool m_wantsToCaptureMouse;
};

int main()
{
    auto _app = std::make_unique<engine::CApplication>();
    auto _uiLayer = std::make_unique<GuiLightmapsLayer>( "Lightmaps-utils" );
    auto _uiLayerRef = dynamic_cast<GuiLightmapsLayer*>( _app->addGuiLayer( std::move( _uiLayer ) ) );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 0.0f, 0.0f, 3.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Y,
                                                           _cameraProjData,
                                                           _app->window()->width(),
                                                           _app->window()->height() );

    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->position = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_lightmaps";
    auto _shaderLightingRef = engine::CShaderManager::CreateShaderFromFiles( "phong_lightmaps_shader",
                                                                             _baseNamePhong + "_vs.glsl",
                                                                             _baseNamePhong + "_fs.glsl" );

    ENGINE_ASSERT( _shaderLightingRef, "Could not load phong-material shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmoRef = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmoRef, "Could not grab the basic3d shader to render the light gizmo :(" );

    /* create material properties */
    auto _phongMaterial = std::make_unique<engine::CMaterial>( "phong_material", 
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 0.5f, 0.31f ),
                                                               engine::CVec3( 1.0f, 0.5f, 0.31f ),
                                                               engine::CVec3( 1.0f, 0.5f, 0.31f ),
                                                               32.0f );

    /* create light properties */
    auto _pointLight = std::make_unique<engine::CPointLight>( "point_light",
                                                              engine::CVec3( 0.1f, 0.1f, 0.1f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              _gizmo->position,
                                                              1.0, 0.7, 1.8 );

    bool _moveLight = false;
    float _mvParam = 0.0f;

    _uiLayerRef->setMaterial( _phongMaterial.get() );
    _uiLayerRef->setLight( _pointLight.get() );

    while( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
        {
            _camera->setActiveMode( false );
            _uiLayerRef->setActive( true );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
        {
            _camera->setActiveMode( true );
            _uiLayerRef->setActive( false );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_P ) )
        {
            _moveLight = !_moveLight;
            ENGINE_INFO( "Light state: {0}", ( _moveLight ) ? "moving" : "fixed" );
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();
        _app->begin();
        _camera->update();

        if ( _moveLight )
        {
            _mvParam += tinyutils::Clock::GetAvgTimeStep();
            // _gizmo->position.x() = 1.0f + std::sin( _mvParam ) * 2.0f;
            // _gizmo->position.y() = std::sin( _mvParam / 2.0f ) * 1.0f;

            _pointLight->position.x() = 4.0f * std::sin( _mvParam );
            _pointLight->position.y() = 4.0f * std::cos( _mvParam );
            _pointLight->position.z() = 0.0f;
        }

        auto _mesh = _uiLayerRef->selectedMesh();

        _gizmo->position = _pointLight->position;

        /* do our thing here ************************/
        _shaderLightingRef->bind();
        _shaderLightingRef->setMat4( "u_modelMatrix", _mesh->matModel() );
        _shaderLightingRef->setMat4( "u_viewProjMatrix", _camera->matProj() * _camera->matView() );
        _shaderLightingRef->setMat4( "u_normalMatrix", tinymath::inverse( _mesh->matModel() ).transpose() );
        _phongMaterial->bind( _shaderLightingRef );
        _shaderLightingRef->setVec3( "u_light.ambient", _pointLight->ambient );
        _shaderLightingRef->setVec3( "u_light.diffuse", _pointLight->diffuse );
        _shaderLightingRef->setVec3( "u_light.specular", _pointLight->specular );
        _shaderLightingRef->setFloat( "u_light.intensity", _pointLight->intensity );
        _shaderLightingRef->setVec3( "u_light.position", _pointLight->position );
        _shaderLightingRef->setVec3( "u_viewerPosition", _camera->position() );

        _mesh->render();

        _phongMaterial->unbind();
        _shaderLightingRef->unbind();

        _shaderGizmoRef->bind();
        _shaderGizmoRef->setMat4( "u_tModel", _gizmo->matModel() );
        _shaderGizmoRef->setMat4( "u_tView", _camera->matView() );
        _shaderGizmoRef->setMat4( "u_tProj", _camera->matProj() );
        _shaderGizmoRef->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

        _gizmo->render();

        _shaderGizmoRef->unbind();
        /********************************************/

        _app->render();
        _app->end();
        tinyutils::Clock::Tock();

        ENGINE_TRACE( "frame-time: {0}", tinyutils::Clock::GetAvgTimeStep() );
    }

    return 0;
}