
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

        m_meshes.clear();
        m_cachedTextures.clear();
    }

    void setMaterial( engine::CIMaterial* material )
    {
        m_material = material;
    }

    void setLight( engine::CILight* light )
    {
        m_light = light;
    }

    engine::CIRenderable* selectedMesh()
    {
        return m_meshes[ m_meshSelectedIndex ];
    }

protected :

    void _initInternal() override
    {
        m_light = nullptr;
        m_material = nullptr;
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

        m_meshes = { _box, _sphere, _ellipsoid, _cylinderX, _cylinderY, _cylinderZ, 
                     _capsuleX, _capsuleY, _capsuleZ, _arrowX, _arrowY, _arrowZ, _axes };
        m_meshesNames = { "box", "sphere", "ellipsoid", "cylinderX", "cylinderY", "cylinderZ", 
                          "capsuleX", "capsuleY", "capsuleZ", "arrowX", "arrowY", "arrowZ", "axes" };
        m_meshSelectedName = "box";
        m_meshSelectedIndex = 0;

        m_cachedTextures = engine::CTextureManager::GetAllCachedTextures();
        m_cachedTextures.push_back( nullptr );
        m_currentDiffuseMapName = "none";
        m_currentSpecularMapName = "none";
    }

    void _renderInternal() override
    {
        _menuUiMaterial();
        _menuUiLight();
        _menuUiGeometry();
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

            m_material->setDiffuseMap( m_currentDiffuseMap );

            if ( m_currentDiffuseMap )
                ImGui::Image( (void*)(intptr_t) m_currentDiffuseMap->openglId(), ImVec2( 64, 64 ) );
            else
                _menuUiDiffuseProps();
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
            else
                _menuUiSpecularProps();
        }

        ImGui::Spacing();
        ImGui::Text( m_material->toString().c_str() );
        ImGui::End();
    }

    void _menuUiDiffuseProps()
    {
        float _cAmbient[3]  = { m_material->ambient.x, m_material->ambient.y, m_material->ambient.z };
        float _cDiffuse[3]  = { m_material->diffuse.x, m_material->diffuse.y, m_material->diffuse.z };
        ImGui::ColorEdit3( "cAmbient", _cAmbient );
        ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
        m_material->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
        m_material->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
    }

    void _menuUiSpecularProps()
    {
        float _cSpecular[3] = { m_material->specular.x, m_material->specular.y, m_material->specular.z };
        ImGui::ColorEdit3( "cSpecular", _cSpecular );
        ImGui::SliderFloat( "cShininess", &m_material->shininess, 32.0f, 256.0f );
        m_material->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
    }

    void _menuUiLight()
    {
        if ( !m_light )
            return;

        ImGui::Begin( "Light-color-properties" );

        ImGui::Checkbox( "funky???", &m_lightsAnimated );

        if ( m_lightsAnimated )
        {
            m_light->ambient.x = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 2.0f );
            m_light->ambient.y = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 0.7f );
            m_light->ambient.z = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 1.3f );
            m_light->diffuse = 5.0f * m_light->ambient;
            m_light->specular = { 1.0f, 1.0f, 1.0f };
        }
        else
        {
            float _cAmbient[3]  = { m_light->ambient.x, m_light->ambient.y, m_light->ambient.z };
            float _cDiffuse[3]  = { m_light->diffuse.x, m_light->diffuse.y, m_light->diffuse.z };
            float _cSpecular[3] = { m_light->specular.x, m_light->specular.y, m_light->specular.z };

            ImGui::ColorEdit3( "cAmbient", _cAmbient );
            ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
            ImGui::ColorEdit3( "cSpecular", _cSpecular );
            ImGui::SliderFloat( "cIntensity", &m_light->intensity, 0.1f, 1.0f );

            m_light->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
            m_light->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
            m_light->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };
        }

        ImGui::Spacing();
        ImGui::Text( m_light->toString().c_str() );
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
        }

        ImGui::End();
    }

    engine::CIMaterial* m_material;
    engine::CILight* m_light;
    bool m_lightsAnimated;

    std::string m_currentDiffuseMapName;
    std::string m_currentSpecularMapName;
    engine::CTexture* m_currentDiffuseMap;
    engine::CTexture* m_currentSpecularMap;
    std::vector< engine::CTexture* > m_cachedTextures;

    std::vector< engine::CIRenderable* > m_meshes;
    std::vector< std::string > m_meshesNames;
    std::string m_meshSelectedName;
    int m_meshSelectedIndex;
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
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_lightmaps";
    auto _shaderLighting = engine::CShaderManager::CreateShaderFromFiles( "phong_lightmaps_shader",
                                                                       _baseNamePhong + "_vs.glsl",
                                                                       _baseNamePhong + "_fs.glsl" );

    ENGINE_ASSERT( _shaderLighting, "Could not load phong-material shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    /* create material properties */
    auto _phongMaterial = new engine::CPhongMaterial( "phong_material", 
                                                      { 1.0f, 0.5f, 0.31f },
                                                      { 1.0f, 0.5f, 0.31f },
                                                      { 1.0f, 0.5f, 0.31f },
                                                      32.0f );

    /* create light properties */
    auto _pointLight = new engine::CPointLight( "point_light",
                                                { 0.1f, 0.1f, 0.1f },
                                                { 1.0f, 1.0f, 1.0f },
                                                { 1.0f, 1.0f, 1.0f },
                                                _gizmo->position,
                                                1.0, 0.7, 1.8 );

    bool _moveLight = false;
    float _mvParam = 0.0f;

    _ui->setMaterial( _phongMaterial );
    _ui->setLight( _pointLight );

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

        _app->beginRendering();
        _camera->update();

        if ( _moveLight )
        {
            _mvParam += 10.0f * engine::CTime::GetTimeStep();
            // _gizmo->position.x = 1.0f + std::sin( _mvParam ) * 2.0f;
            // _gizmo->position.y = std::sin( _mvParam / 2.0f ) * 1.0f;

            _pointLight->position.x = 4.0f * std::sin( _mvParam );
            _pointLight->position.y = 4.0f * std::cos( _mvParam );
            _pointLight->position.z = 0.0f;
        }

        auto _mesh = _ui->selectedMesh();

        _gizmo->position = _pointLight->position;

        /* do our thing here ************************/
        _shaderLighting->bind();
        _shaderLighting->setMat4( "u_modelMatrix", _mesh->matModel() );
        _shaderLighting->setMat4( "u_viewProjMatrix", _camera->matProj() * _camera->matView() );
        _shaderLighting->setMat4( "u_normalMatrix", ( ( _mesh->matModel() ).inverse() ).transpose() );
        _phongMaterial->bind( _shaderLighting );
        _shaderLighting->setVec3( "u_light.ambient", _pointLight->ambient );
        _shaderLighting->setVec3( "u_light.diffuse", _pointLight->diffuse );
        _shaderLighting->setVec3( "u_light.specular", _pointLight->specular );
        _shaderLighting->setFloat( "u_light.intensity", _pointLight->intensity );
        _shaderLighting->setVec3( "u_light.position", _pointLight->position );
        _shaderLighting->setVec3( "u_viewerPosition", _camera->position() );

        _mesh->render();

        _phongMaterial->unbind();
        _shaderLighting->unbind();

        _shaderGizmo->bind();
        _shaderGizmo->setMat4( "u_tModel", _gizmo->matModel() );
        _shaderGizmo->setMat4( "u_tView", _camera->matView() );
        _shaderGizmo->setMat4( "u_tProj", _camera->matProj() );
        _shaderGizmo->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

        _gizmo->render();

        _shaderGizmo->unbind();
        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        if ( !_camera->active() )
            _app->renderUi();

        _app->endRendering();

        // ENGINE_TRACE( "frame-time: {0}", engine::CTime::GetRawTimeStep() );
    }

    return 0;
}