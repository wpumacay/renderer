
#include <CEngine.h>

class ApplicationUi : public engine::CImguiUi
{

public :

    ApplicationUi( engine::COpenGLContext* context ) 
        : engine::CImguiUi( context ) {}

    ~ApplicationUi() 
    {
        for ( auto _mesh : m_meshes )
            delete _mesh;

        for ( auto _light : m_lights )
            delete _light;

        m_meshes.clear();
        m_lights.clear();
        m_cachedTextures.clear();
    }

    void setMaterial( engine::CIMaterial* material )
    {
        m_material = material;
    }

    engine::CIRenderable* selectedMesh()
    {
        return m_meshes[ m_meshSelectedIndex ];
    }

    engine::CILight* selectedLight()
    {
        return m_lights[ m_lightSelectedIndex ];
    }

    bool useRandomPattern()
    {
        return m_meshesUseRandomPattern;
    }

    bool isLightLockedToCamera()
    {
        return m_lightLockedToCamera;
    }

    void updateLightDirection( const engine::CVec3& direction )
    {
        m_lightDirection = direction;
    }

protected :

    void _initInternal() override
    {
        m_material = nullptr;

        auto _box       = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
        auto _sphere    = engine::CMeshBuilder::createSphere( 0.5f );
        auto _ellipsoid = engine::CMeshBuilder::createEllipsoid( 0.5f, 0.675f, 0.75f );
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

        m_meshes = { _box, _sphere, _ellipsoid, _cylinderX, _cylinderY, _cylinderZ, 
                     _capsuleX, _capsuleY, _capsuleZ, _arrowX, _arrowY, _arrowZ, _axes };
        m_meshesNames = { "box", "sphere", "ellipsoid", "cylinderX", "cylinderY", "cylinderZ", 
                          "capsuleX", "capsuleY", "capsuleZ", "arrowX", "arrowY", "arrowZ", "axes" };
        m_meshSelectedName = "box";
        m_meshSelectedIndex = 0;
        m_meshesUseRandomPattern = true;

        m_cachedTextures = engine::CTextureManager::GetAllCachedTextures();
        m_cachedTextures.push_back( nullptr );
        m_currentDiffuseMapName = "none";
        m_currentSpecularMapName = "none";
        m_currentSpecularMap = nullptr;
        m_currentDiffuseMap = nullptr;

        auto _directionalLight = new engine::CDirectionalLight( "directional",
                                                                { 0.2f, 0.2f, 0.2f },
                                                                { 0.5f, 0.5f, 0.5f },
                                                                { 1.0f, 1.0f, 1.0f },
                                                                { -0.2f, -1.0f, -0.3f } );

        auto _pointLight = new engine::CPointLight( "point",
                                                    { 0.2f, 0.2f, 0.2f },
                                                    { 0.5f, 0.5f, 0.5f },
                                                    { 1.0f, 1.0f, 1.0f },
                                                    { 1.2f, 1.0f, 2.0f },
                                                    1.0f, 0.09f, 0.032f );

        auto _spotLight = new engine::CSpotLight( "spot",
                                                  { 0.2f, 0.2f, 0.2f },
                                                  { 0.5f, 0.5f, 0.5f },
                                                  { 1.0f, 1.0f, 1.0f },
                                                  { 0.0f, 0.0f, 2.0f },
                                                  { 0.0f, 0.0f, -1.0f },
                                                  1.0f, 0.09f, 0.032f,
                                                  ENGINE_PI / 4.0f,
                                                  ENGINE_PI / 3.0f );

        m_lights = { _directionalLight, _pointLight, _spotLight };
        m_lightsNames = { _directionalLight->name(), _pointLight->name(), _spotLight->name() };
        m_lightSelectedName = _directionalLight->name();
        m_lightSelectedIndex = 0;
        m_lightLockedToCamera = false;
        m_lightDirection = { -0.2f, -1.0f, -0.3f };
    }

    void _renderInternal() override
    {
        _menuUiMaterial();
        _menuUiGeometry();
        _menuUiLight();
    }

private :

    void _menuUiMaterial()
    {
        if ( !m_material )
            return;

        ImGui::Begin( "Material-properties" );

        /* diffuse properties (phong and lambert materials) */
        if ( m_material->type() == engine::eMaterialType::PHONG || m_material->type() == engine::eMaterialType::LAMBERT )
        {
            if ( ImGui::BeginCombo( "Diffuse-map", m_currentDiffuseMapName.c_str() ) )
            {
                for ( auto& _cachedTexture : m_cachedTextures )
                {
                    std::string _textureName = ( _cachedTexture ) ? _cachedTexture->name() : "none";
                    bool _isSelected = ( _textureName == m_currentDiffuseMapName );
    
                    if ( ImGui::Selectable( _textureName.c_str(), _isSelected ) )
                    {
                        m_currentDiffuseMap = _cachedTexture;
                        m_currentDiffuseMapName = _textureName;
                    }
    
                    if ( _isSelected )
                        ImGui::SetItemDefaultFocus();
                }
    
                ImGui::EndCombo();
            }

            if ( m_material->type() == engine::eMaterialType::PHONG )
                reinterpret_cast< engine::CPhongMaterial* >( m_material )->setDiffuseMap( m_currentDiffuseMap );

            if ( m_material->type() == engine::eMaterialType::LAMBERT )
                reinterpret_cast< engine::CLambertMaterial* >( m_material )->setDiffuseMap( m_currentDiffuseMap );

            if ( m_currentDiffuseMap )
                ImGui::Image( (void*)(intptr_t) m_currentDiffuseMap->openglId(), ImVec2( 64, 64 ) );
            else
                _menuUiMaterialDiffuseProps();
        }

        /* specular properties (phong material only) */
        if ( m_material->type() == engine::eMaterialType::PHONG )
        {
            if ( ImGui::BeginCombo( "Specular-map", m_currentSpecularMapName.c_str() ) )
            {
                for ( auto& _cachedTexture : m_cachedTextures )
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

            reinterpret_cast< engine::CPhongMaterial* >( m_material )->setSpecularMap( m_currentSpecularMap );

            if ( m_currentSpecularMap )
                ImGui::Image( (void*)(intptr_t) m_currentSpecularMap->openglId(), ImVec2( 64, 64 ) );
            else
                _menuUiMaterialSpecularProps();
        }

        ImGui::Spacing();
        ImGui::Text( m_material->toString().c_str() );
        ImGui::End();
    }

    void _menuUiMaterialDiffuseProps()
    {
        if ( m_material->type() == engine::eMaterialType::PHONG )
        {
            auto _materialPhong = reinterpret_cast< engine::CPhongMaterial* >( m_material );

            float _cAmbient[3]  = { _materialPhong->ambient.x, _materialPhong->ambient.y, _materialPhong->ambient.z };
            float _cDiffuse[3]  = { _materialPhong->diffuse.x, _materialPhong->diffuse.y, _materialPhong->diffuse.z };
            ImGui::ColorEdit3( "cAmbient", _cAmbient );
            ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
            _materialPhong->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
            _materialPhong->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
        }
        else if ( m_material->type() == engine::eMaterialType::LAMBERT )
        {
            auto _materialLambert = reinterpret_cast< engine::CLambertMaterial* >( m_material );

            float _cAmbient[3]  = { _materialLambert->ambient.x, _materialLambert->ambient.y, _materialLambert->ambient.z };
            float _cDiffuse[3]  = { _materialLambert->diffuse.x, _materialLambert->diffuse.y, _materialLambert->diffuse.z };
            ImGui::ColorEdit3( "cAmbient", _cAmbient );
            ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
            _materialLambert->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
            _materialLambert->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
        }
    }

    void _menuUiMaterialSpecularProps()
    {
        if ( m_material->type() == engine::eMaterialType::PHONG )
        {
            auto _materialPhong = reinterpret_cast< engine::CPhongMaterial* >( m_material );

            float _cSpecular[3] = { _materialPhong->specular.x, _materialPhong->specular.y, _materialPhong->specular.z };
            ImGui::ColorEdit3( "cSpecular", _cSpecular );
            ImGui::SliderFloat( "cShininess", &_materialPhong->shininess, 32.0f, 256.0f );
            _materialPhong->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
        }
    }

    void _menuUiLight()
    {
        ImGui::Begin( "Light-configuration" );

        ImGui::Checkbox( "locked-to-camera", &m_lightLockedToCamera );
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
        }

        _menuUiLightProperties();

        ImGui::Spacing();
        ImGui::Text( m_lights[ m_lightSelectedIndex ]->toString().c_str() );
        ImGui::End();
    }

    void _menuUiLightProperties()
    {
        auto _type = m_lights[m_lightSelectedIndex]->type();
        if ( _type == engine::eLightType::DIRECTIONAL )
        {
            auto _dirlight = reinterpret_cast< engine::CDirectionalLight* >( m_lights[m_lightSelectedIndex] );
            if ( !m_lightLockedToCamera )
            {
                float _vDirection[3] = { m_lightDirection.x, m_lightDirection.y, m_lightDirection.z };
                ImGui::SliderFloat3( "direction", _vDirection, -1.0f, 1.0f );
                m_lightDirection = { _vDirection[0], _vDirection[1], _vDirection[2] };
                _dirlight->direction = engine::CVec3::normalize( m_lightDirection );
            }
        }
        else if ( _type == engine::eLightType::POINT )
        {
            auto _pointLight = reinterpret_cast< engine::CPointLight* >( m_lights[m_lightSelectedIndex] );
            if ( !m_lightLockedToCamera )
            {
                float _vPosition[3] = { _pointLight->position.x, _pointLight->position.y, _pointLight->position.z };
                ImGui::SliderFloat3( "position", _vPosition, -10.0f, 10.0f );
                _pointLight->position = { _vPosition[0], _vPosition[1], _vPosition[2] };
            }

            ImGui::SliderFloat( "attn-linear", &_pointLight->atnLinear, 0.0f, 1.0f );
            ImGui::SliderFloat( "attn-quadratic", &_pointLight->atnQuadratic, 0.0f, 1.0f );
        }
        else if ( _type == engine::eLightType::SPOT )
        {
            auto _spotLight = reinterpret_cast< engine::CSpotLight* >( m_lights[m_lightSelectedIndex] );

            if ( !m_lightLockedToCamera )
            {
                float _vPosition[3] = { _spotLight->position.x, _spotLight->position.y, _spotLight->position.z };
                ImGui::SliderFloat3( "position", _vPosition, -10.0f, 10.0f );
                _spotLight->position = { _vPosition[0], _vPosition[1], _vPosition[2] };

                float _vDirection[3] = { m_lightDirection.x, m_lightDirection.y, m_lightDirection.z };
                ImGui::SliderFloat3( "direction", _vDirection, -1.0f, 1.0f );
                m_lightDirection = { _vDirection[0], _vDirection[1], _vDirection[2] };
                _spotLight->direction = engine::CVec3::normalize( m_lightDirection );
            }

            ImGui::SliderFloat( "attn-linear", &_spotLight->atnLinear, 0.0f, 1.0f );
            ImGui::SliderFloat( "attn-quadratic", &_spotLight->atnQuadratic, 0.0f, 1.0f );
            ImGui::SliderFloat( "inner-cutoff", &_spotLight->innerCutoff, ENGINE_PI / 20.0f, ENGINE_PI / 3.0f );
            ImGui::SliderFloat( "outer-cutoff", &_spotLight->outerCutoff, _spotLight->innerCutoff, ENGINE_PI / 3.0f );
        }
    }

    void _menuUiGeometry()
    {
        ImGui::Begin( "Mesh-selection" );

        ImGui::Checkbox( "random-pattern", &m_meshesUseRandomPattern );

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
        }

        ImGui::End();
    }

    engine::CIMaterial* m_material;

    std::string m_currentDiffuseMapName;
    std::string m_currentSpecularMapName;
    engine::CTexture* m_currentDiffuseMap;
    engine::CTexture* m_currentSpecularMap;
    std::vector< engine::CTexture* > m_cachedTextures;

    std::vector< engine::CIRenderable* > m_meshes;
    std::vector< std::string > m_meshesNames;
    std::string m_meshSelectedName;
    int m_meshSelectedIndex;
    bool m_meshesUseRandomPattern;

    std::vector< engine::CILight* > m_lights;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;
    bool m_lightLockedToCamera;
    engine::CVec3 m_lightDirection;
};

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _ui = new ApplicationUi( engine::COpenGLApp::GetWindow()->context() );
    _ui->init();

    _app->setUi( std::unique_ptr< ApplicationUi >( _ui ) );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->position = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_lightcasters";
    auto _shaderLightCasters = engine::CShaderManager::CreateShaderFromFiles( "phong_lightcasters_shader",
                                                                       _baseNamePhong + "_vs.glsl",
                                                                       _baseNamePhong + "_fs.glsl" );

    ENGINE_ASSERT( _shaderLightCasters, "Could not load phong-lightcasters shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    /* create material properties */
    auto _phongMaterial = new engine::CPhongMaterial( "phong_material", 
                                                      { 1.0f, 0.5f, 0.31f },
                                                      { 1.0f, 0.5f, 0.31f },
                                                      { 1.0f, 0.5f, 0.31f },
                                                      32.0f );

    bool _moveLight = false;
    float _mvParam = 0.0f;

    _ui->setMaterial( _phongMaterial );

    std::vector< engine::CVec3 > _meshPositions =  {
        {  0.0f,  0.0f,  0.0f },
        {  2.0f,  5.0f, -15.0f },
        { -1.5f, -2.2f, -2.5f },
        { -3.8f, -2.0f, -12.3f },
        {  2.4f, -0.4f, -3.5f },
        { -1.7f,  3.0f, -7.5f },
        {  1.3f, -2.0f, -2.5f },
        {  1.5f,  2.0f, -2.5f },
        {  1.5f,  0.2f, -1.5f },
        { -1.3f,  1.0f, -1.5f }
    };

    while( _app->active() )
    {
        if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
        {
            break;
        }
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
        {
            _camera->setActiveMode( false );
        }
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
        {
            _camera->setActiveMode( true );
        }
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_P ) )
        {
            _moveLight = !_moveLight;
            ENGINE_INFO( "Light state: {0}", ( _moveLight ) ? "moving" : "fixed" );
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();
        _camera->update();

        auto _mesh = _ui->selectedMesh();
        auto _light = _ui->selectedLight();

        if ( _ui->isLightLockedToCamera() )
        {
            if ( _light->type() == engine::eLightType::DIRECTIONAL )
            {
                reinterpret_cast< engine::CDirectionalLight* >( _light )->direction = _camera->front();
                _ui->updateLightDirection( _camera->front() );
            }
            else if ( _light->type() == engine::eLightType::POINT )
            {
                reinterpret_cast< engine::CPointLight* >( _light )->position = _camera->position();
            }
            else if ( _light->type() == engine::eLightType::SPOT )
            {
                reinterpret_cast< engine::CSpotLight* >( _light )->position = _camera->position();
                reinterpret_cast< engine::CSpotLight* >( _light )->direction = _camera->front();
                _ui->updateLightDirection( _camera->front() );
            }
        }

        _gizmo->setVisibility( !_ui->isLightLockedToCamera() );

        if ( _light->type() == engine::eLightType::POINT )
            _gizmo->position = reinterpret_cast< engine::CPointLight* >( _light )->position;
        else if ( _light->type() == engine::eLightType::SPOT )
            _gizmo->position = reinterpret_cast< engine::CSpotLight* >( _light )->position;
        else
            _gizmo->position = { 2000.0f, 2000.0f, 2000.0f }; 

        if ( _light->type() == engine::eLightType::SPOT )
        {
            auto _position = reinterpret_cast< engine::CSpotLight* >( _light )->position;
            auto _direction = reinterpret_cast< engine::CSpotLight* >( _light )->direction;
            engine::CDebugDrawer::DrawLine( _position, _position + 0.1f * _direction, { 0.0f, 1.0f, 1.0f } );
        }

        /* do our thing here ************************/
        _shaderLightCasters->bind();
        _shaderLightCasters->setMat4( "u_viewProjMatrix", _camera->matProj() * _camera->matView() );
        _phongMaterial->bind( _shaderLightCasters );
        // deactivate all lights first
        _shaderLightCasters->setInt( "u_pointLight.enabled", 0 );
        _shaderLightCasters->setInt( "u_directionalLight.enabled", 0 );
        // _shaderLightCasters->setInt( "u_spotLIght.enabled", 0 );

        if ( _light->type() == engine::eLightType::DIRECTIONAL )
        {
            _shaderLightCasters->setInt( "u_directionalLight.enabled", 1 );
            _shaderLightCasters->setVec3( "u_directionalLight.ambient", _light->ambient );
            _shaderLightCasters->setVec3( "u_directionalLight.diffuse", _light->diffuse );
            _shaderLightCasters->setVec3( "u_directionalLight.specular", _light->specular );
            _shaderLightCasters->setFloat( "u_directionalLight.intensity", _light->intensity );
            _shaderLightCasters->setVec3( "u_directionalLight.direction", reinterpret_cast< engine::CDirectionalLight* >( _light )->direction );
        }
        else if ( _light->type() == engine::eLightType::POINT )
        {
            _shaderLightCasters->setInt( "u_pointLight.enabled", 1 );
            _shaderLightCasters->setVec3( "u_pointLight.ambient", _light->ambient );
            _shaderLightCasters->setVec3( "u_pointLight.diffuse", _light->diffuse );
            _shaderLightCasters->setVec3( "u_pointLight.specular", _light->specular );
            _shaderLightCasters->setFloat( "u_pointLight.intensity", _light->intensity );
            _shaderLightCasters->setVec3( "u_pointLight.position", reinterpret_cast< engine::CPointLight* >( _light )->position );
            _shaderLightCasters->setFloat( "u_pointLight.attnk0", reinterpret_cast< engine::CPointLight* >( _light )->atnConstant );
            _shaderLightCasters->setFloat( "u_pointLight.attnk1", reinterpret_cast< engine::CPointLight* >( _light )->atnLinear );
            _shaderLightCasters->setFloat( "u_pointLight.attnk2", reinterpret_cast< engine::CPointLight* >( _light )->atnQuadratic );
        }
        else if ( _light->type() == engine::eLightType::SPOT )
        {
            _shaderLightCasters->setInt( "u_spotLight.enabled", 1 );
            _shaderLightCasters->setVec3( "u_spotLight.ambient", _light->ambient );
            _shaderLightCasters->setVec3( "u_spotLight.diffuse", _light->diffuse );
            _shaderLightCasters->setVec3( "u_spotLight.specular", _light->specular );
            _shaderLightCasters->setFloat( "u_spotLight.intensity", _light->intensity );
            _shaderLightCasters->setVec3( "u_spotLight.position", reinterpret_cast< engine::CSpotLight* >( _light )->position );
            _shaderLightCasters->setFloat( "u_spotLight.attnk0", reinterpret_cast< engine::CSpotLight* >( _light )->atnConstant );
            _shaderLightCasters->setFloat( "u_spotLight.attnk1", reinterpret_cast< engine::CSpotLight* >( _light )->atnLinear );
            _shaderLightCasters->setFloat( "u_spotLight.attnk2", reinterpret_cast< engine::CSpotLight* >( _light )->atnQuadratic );
            _shaderLightCasters->setVec3( "u_spotLight.direction", reinterpret_cast< engine::CSpotLight* >( _light )->direction );
            _shaderLightCasters->setFloat( "u_spotLight.innerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( _light )->innerCutoff ) );
            _shaderLightCasters->setFloat( "u_spotLight.outerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( _light )->outerCutoff ) );
        }

        _shaderLightCasters->setVec3( "u_viewerPosition", _camera->position() );

        for ( size_t i = 0; i < _meshPositions.size(); i++ )
        {
            engine::CMat4 _modelMat;

            if ( _ui->useRandomPattern() )
            {
                auto _xyz = _meshPositions[i];
                float _theta = engine::toRadians( 20.0f * i );
                _modelMat = engine::CMat4::translate( _xyz ) * engine::CMat4::rotation( _theta, { 1.0f, 0.3f, 0.5f } );
            }
            else
            {
                int _row = i / 5;
                int _col = i % 5;

                engine::CVec3 _xyz = { ( _col - 2.0f ) * 2.0f, 0.0f, -( ( _row == 0 ) ? 0.0f : 2.0f ) };
                _modelMat = engine::CMat4::translate( _xyz );
            }

            _shaderLightCasters->setMat4( "u_modelMatrix", _modelMat );
            _shaderLightCasters->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
            _mesh->render();
        }

        _phongMaterial->unbind();
        _shaderLightCasters->unbind();

        if ( _gizmo->visible() )
        {
            _shaderGizmo->bind();
            _shaderGizmo->setMat4( "u_tModel", _gizmo->matModel() );
            _shaderGizmo->setMat4( "u_tView", _camera->matView() );
            _shaderGizmo->setMat4( "u_tProj", _camera->matProj() );
            _shaderGizmo->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

            _gizmo->render();

            _shaderGizmo->unbind();
        }
        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        if ( !_camera->active() )
            _app->renderUi();

        _app->end();

        // ENGINE_TRACE( "frame-time: {0}", engine::CTime::GetRawTimeStep() );
    }

    return 0;
}