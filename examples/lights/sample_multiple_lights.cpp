
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

        m_directionalLight = nullptr;
        m_spotLight = nullptr;

        m_meshes.clear();
        m_lights.clear();
        m_pointLights.clear();
        m_cachedTextures.clear();
    }

    void setMaterial( engine::CMaterial* material )
    {
        m_material = material;
    }

    engine::CIRenderable* selectedMesh()
    {
        return m_meshes[ m_meshSelectedIndex ];
    }

    std::vector< engine::CPointLight* > pointLights()
    {
        return m_pointLights;
    }

    engine::CDirectionalLight* directionalLight()
    {
        return m_directionalLight;
    }

    engine::CSpotLight* spotLight()
    {
        return m_spotLight;
    }

    bool useRandomPattern()
    {
        return m_meshesUseRandomPattern;
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
        m_currentAlbedoMapName = "none";
        m_currentSpecularMapName = "none";
        m_currentAlbedoMap = nullptr;
        m_currentSpecularMap = nullptr;

        m_directionalLight = new engine::CDirectionalLight( "directional",
                                                            { 0.2f, 0.2f, 0.2f },
                                                            { 0.5f, 0.5f, 0.5f },
                                                            { 1.0f, 1.0f, 1.0f },
                                                            { -0.2f, -1.0f, -0.3f } );
        m_lights.push_back( m_directionalLight );
        m_lightsNames.push_back( "directional" );

        // positions of the pointLights
        std::vector< engine::CVec3 > _pointLightPositions = {
            { 0.7f,  0.2f,  2.0f },
            { 2.3f, -3.3f, -4.0f },
            {-4.0f,  2.0f, -12.0f },
            { 0.0f,  0.0f, -3.0f }
        };

        for ( size_t i = 0; i < _pointLightPositions.size(); i++ )
        {
            auto _pointLight = new engine::CPointLight( std::string( "point_" ) + std::to_string( i ),
                                                        { 0.2f, 0.2f, 0.2f },
                                                        { 0.5f, 0.5f, 0.5f },
                                                        { 1.0f, 1.0f, 1.0f },
                                                        _pointLightPositions[i],
                                                        1.0f, 0.09f, 0.032f );
            m_lights.push_back( _pointLight );
            m_pointLights.push_back( _pointLight );
            m_lightsNames.push_back( std::string( "point_" ) + std::to_string( i ) );
        }

        m_spotLight = new engine::CSpotLight( "spot",
                                              { 0.2f, 0.2f, 0.2f },
                                              { 0.5f, 0.5f, 0.5f },
                                              { 1.0f, 1.0f, 1.0f },
                                              { 0.0f, 0.0f, 2.0f },
                                              { 0.0f, 0.0f, -1.0f },
                                              1.0f, 0.09f, 0.032f,
                                              ENGINE_PI / 4.0f,
                                              ENGINE_PI / 3.0f );
        m_lights.push_back( m_spotLight );
        m_lightsNames.push_back( "spot" );

        m_lightSelectedName = m_directionalLight->name();
        m_lightSelectedIndex = 0;
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
            if ( ImGui::BeginCombo( "Albedo-map", m_currentAlbedoMapName.c_str() ) )
            {
                for ( auto& _cachedTexture : m_cachedTextures )
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

            m_material->setAlbedoMap( m_currentAlbedoMap );

            if ( m_currentAlbedoMap )
                ImGui::Image( (void*)(intptr_t) m_currentAlbedoMap->openglId(), ImVec2( 64, 64 ) );

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

            m_material->setSpecularMap( m_currentSpecularMap );

            if ( m_currentSpecularMap )
                ImGui::Image( (void*)(intptr_t) m_currentSpecularMap->openglId(), ImVec2( 64, 64 ) );

            _menuUiMaterialSpecularProps();
        }

        ImGui::Spacing();
        ImGui::Text( m_material->toString().c_str() );
        ImGui::End();
    }

    void _menuUiMaterialDiffuseProps()
    {
        float _cAmbient[3]  = { m_material->ambient.x, m_material->ambient.y, m_material->ambient.z };
        float _cDiffuse[3]  = { m_material->diffuse.x, m_material->diffuse.y, m_material->diffuse.z };
        ImGui::ColorEdit3( "cAmbient", _cAmbient );
        ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
        m_material->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
        m_material->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
    }

    void _menuUiMaterialSpecularProps()
    {
        float _cSpecular[3] = { m_material->specular.x, m_material->specular.y, m_material->specular.z };
        ImGui::ColorEdit3( "cSpecular", _cSpecular );
        ImGui::SliderFloat( "cShininess", &m_material->shininess, 32.0f, 256.0f );
        m_material->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
    }

    void _menuUiLight()
    {
        ImGui::Begin( "Light-configuration" );

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
        ImGui::Checkbox( "enabled", &m_lights[m_lightSelectedIndex]->enabled );

        auto _light = m_lights[m_lightSelectedIndex];
        auto _type = _light->type();
        if ( _type == engine::eLightType::DIRECTIONAL )
        {
            float _vDirection[3] = { m_lightDirection.x, m_lightDirection.y, m_lightDirection.z };
            ImGui::SliderFloat3( "direction", _vDirection, -1.0f, 1.0f );
            m_lightDirection = { _vDirection[0], _vDirection[1], _vDirection[2] };
            _light->direction = engine::CVec3::normalize( m_lightDirection );
        }
        else if ( _type == engine::eLightType::POINT )
        {
            float _vPosition[3] = { _light->position.x, _light->position.y, _light->position.z };
            ImGui::SliderFloat3( "position", _vPosition, -10.0f, 10.0f );
            _light->position = { _vPosition[0], _vPosition[1], _vPosition[2] };

            ImGui::SliderFloat( "attn-linear", &_light->atnLinear, 0.0f, 1.0f );
            ImGui::SliderFloat( "attn-quadratic", &_light->atnQuadratic, 0.0f, 1.0f );
        }
        else if ( _type == engine::eLightType::SPOT )
        {
            float _vPosition[3] = { _light->position.x, _light->position.y, _light->position.z };
            ImGui::SliderFloat3( "position", _vPosition, -10.0f, 10.0f );
            _light->position = { _vPosition[0], _vPosition[1], _vPosition[2] };

            float _vDirection[3] = { m_lightDirection.x, m_lightDirection.y, m_lightDirection.z };
            ImGui::SliderFloat3( "direction", _vDirection, -1.0f, 1.0f );
            m_lightDirection = { _vDirection[0], _vDirection[1], _vDirection[2] };
            _light->direction = engine::CVec3::normalize( m_lightDirection );

            ImGui::SliderFloat( "attn-linear", &_light->atnLinear, 0.0f, 1.0f );
            ImGui::SliderFloat( "attn-quadratic", &_light->atnQuadratic, 0.0f, 1.0f );
            ImGui::SliderFloat( "inner-cutoff", &_light->innerCutoff, ENGINE_PI / 20.0f, ENGINE_PI / 3.0f );
            ImGui::SliderFloat( "outer-cutoff", &_light->outerCutoff, _light->innerCutoff, ENGINE_PI / 3.0f );
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

    engine::CMaterial* m_material;

    std::string m_currentAlbedoMapName;
    std::string m_currentSpecularMapName;
    engine::CTexture* m_currentAlbedoMap;
    engine::CTexture* m_currentSpecularMap;
    std::vector< engine::CTexture* > m_cachedTextures;

    std::vector< engine::CIRenderable* > m_meshes;
    std::vector< std::string > m_meshesNames;
    std::string m_meshSelectedName;
    int m_meshSelectedIndex;
    bool m_meshesUseRandomPattern;

    std::vector< engine::CILight* > m_lights;
    std::vector< engine::CPointLight* > m_pointLights;
    engine::CDirectionalLight* m_directionalLight;
    engine::CSpotLight* m_spotLight;
    std::vector< std::string > m_lightsNames;
    std::string m_lightSelectedName;
    int m_lightSelectedIndex;
    engine::CVec3 m_lightDirection;
};

int main()
{
    auto _app = new engine::CApplication();
    _app->init();

    auto _ui = new ApplicationUi( _app->window()->context() );
    _ui->init();

    _app->setUi( std::unique_ptr< ApplicationUi >( _ui ) );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             _app->window()->width(),
                                             _app->window()->height() );

    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->position = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_multiple_lights";
    auto _shaderMultipleLights = engine::CShaderManager::CreateShaderFromFiles( "phong_multiple_lights_shader",
                                                                       _baseNamePhong + "_vs.glsl",
                                                                       _baseNamePhong + "_fs.glsl" );

    ENGINE_ASSERT( _shaderMultipleLights, "Could not load phong-multiple-lights shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    /* create material properties */
    auto _phongMaterial = new engine::CMaterial( "phong_material", 
                                                 engine::eMaterialType::PHONG,
                                                 { 1.0f, 0.5f, 0.31f },
                                                 { 1.0f, 0.5f, 0.31f },
                                                 { 1.0f, 0.5f, 0.31f },
                                                 32.0f );

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
        if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
            _camera->setActiveMode( true );

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();
        _camera->update();

        auto _mesh = _ui->selectedMesh();

        /* do our thing here ************************/
        _shaderMultipleLights->bind();
        _shaderMultipleLights->setMat4( "u_viewProjMatrix", _camera->matProj() * _camera->matView() );
        _shaderMultipleLights->setVec3( "u_viewerPosition", _camera->position() );
        _shaderMultipleLights->setInt( "u_numberOfPointLights", _ui->pointLights().size() );
        _shaderMultipleLights->setInt( "u_numberOfSpotLights", 1 );

        // set directional light uniforms
        auto _directionalLight = _ui->directionalLight();
        _shaderMultipleLights->setInt( "u_directionalLight.enabled", ( _directionalLight->enabled ) ? 1 : 0 );
        _shaderMultipleLights->setVec3( "u_directionalLight.ambient", _directionalLight->ambient );
        _shaderMultipleLights->setVec3( "u_directionalLight.diffuse", _directionalLight->diffuse );
        _shaderMultipleLights->setVec3( "u_directionalLight.specular", _directionalLight->specular );
        _shaderMultipleLights->setFloat( "u_directionalLight.intensity", _directionalLight->intensity );
        _shaderMultipleLights->setVec3( "u_directionalLight.direction", _directionalLight->direction );

        // set all point light uniforms
        auto _pointLights = _ui->pointLights();
        for ( size_t i = 0; i < _pointLights.size(); i++ )
        {
            _shaderMultipleLights->setInt( "u_pointLights[" + std::to_string( i ) + "].enabled", ( _pointLights[i]->enabled ) ? 1 : 0 );
            _shaderMultipleLights->setVec3( "u_pointLights[" + std::to_string( i ) + "].ambient", _pointLights[i]->ambient );
            _shaderMultipleLights->setVec3( "u_pointLights[" + std::to_string( i ) + "].diffuse", _pointLights[i]->diffuse );
            _shaderMultipleLights->setVec3( "u_pointLights[" + std::to_string( i ) + "].specular", _pointLights[i]->specular );
            _shaderMultipleLights->setFloat( "u_pointLights[" + std::to_string( i ) + "].intensity", _pointLights[i]->intensity );
            _shaderMultipleLights->setVec3( "u_pointLights[" + std::to_string( i ) + "].position", _pointLights[i]->position );
            _shaderMultipleLights->setFloat( "u_pointLights[" + std::to_string( i ) + "].attnk0", _pointLights[i]->atnConstant );
            _shaderMultipleLights->setFloat( "u_pointLights[" + std::to_string( i ) + "].attnk1", _pointLights[i]->atnLinear );
            _shaderMultipleLights->setFloat( "u_pointLights[" + std::to_string( i ) + "].attnk2", _pointLights[i]->atnQuadratic );
        }

        // set spot light uniforms
        auto _spotLight = _ui->spotLight();
        _shaderMultipleLights->setInt( "u_spotLights[0].enabled", ( _spotLight->enabled ) ? 1 : 0 );
        _shaderMultipleLights->setVec3( "u_spotLights[0].ambient", _spotLight->ambient );
        _shaderMultipleLights->setVec3( "u_spotLights[0].diffuse", _spotLight->diffuse );
        _shaderMultipleLights->setVec3( "u_spotLights[0].specular", _spotLight->specular );
        _shaderMultipleLights->setFloat( "u_spotLights[0].intensity", _spotLight->intensity );
        _shaderMultipleLights->setVec3( "u_spotLights[0].position", _spotLight->position );
        _shaderMultipleLights->setFloat( "u_spotLights[0].attnk0", _spotLight->atnConstant );
        _shaderMultipleLights->setFloat( "u_spotLights[0].attnk1", _spotLight->atnLinear );
        _shaderMultipleLights->setFloat( "u_spotLights[0].attnk2", _spotLight->atnQuadratic );
        _shaderMultipleLights->setVec3( "u_spotLights[0].direction", _spotLight->direction );
        _shaderMultipleLights->setFloat( "u_spotLights[0].innerCutoffCos", std::cos( _spotLight->innerCutoff ) );
        _shaderMultipleLights->setFloat( "u_spotLights[0].outerCutoffCos", std::cos( _spotLight->outerCutoff ) );

        _phongMaterial->bind( _shaderMultipleLights );

        for ( size_t i = 0; i < _meshPositions.size(); i++ )
        {
            engine::CMat4 _modelMat;

            if ( _ui->useRandomPattern() )
            {
                auto _xyz = _meshPositions[i];
                float _theta = engine::toRadians( 20.0f * i );
                _modelMat = engine::CMat4::translation( _xyz ) * engine::CMat4::rotation( _theta, { 1.0f, 0.3f, 0.5f } );
            }
            else
            {
                int _row = i / 5;
                int _col = i % 5;

                engine::CVec3 _xyz = { ( _col - 2.0f ) * 2.0f, 0.0f, -( ( _row == 0 ) ? 0.0f : 2.0f ) };
                _modelMat = engine::CMat4::translation( _xyz );
            }

            _shaderMultipleLights->setMat4( "u_modelMatrix", _modelMat );
            _shaderMultipleLights->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
            _mesh->render();
        }

        _phongMaterial->unbind();
        _shaderMultipleLights->unbind();

        for ( size_t i = 0; i < _pointLights.size(); i++ )
        {
            if ( !_pointLights[i]->enabled )
                continue;

            _shaderGizmo->bind();
            _shaderGizmo->setMat4( "u_tModel", engine::CMat4::translation( _pointLights[i]->position ) );
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