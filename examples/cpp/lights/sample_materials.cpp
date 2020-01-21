
#include <CEngine.h>

class GuiMaterialsLayer : public engine::CImGuiLayer
{

public :

    GuiMaterialsLayer( const std::string& name ) 
        : engine::CImGuiLayer( name ) 
    {
        m_light = nullptr;
        m_material = nullptr;
        m_materialSelectedIndex = -1;
        m_lightsAnimated = false;

        m_materialsList = { { "emerald"       , engine::eMaterialType::PHONG, { 0.0215, 0.1745, 0.0215 }, { 0.07568, 0.61424, 0.07568 }, { 0.633, 0.727811, 0.633 }, 0.6 * 128 },
                            { "jade"          , engine::eMaterialType::PHONG, { 0.1350, 0.2225, 0.1575 }, { 0.54, 0.89, 0.63 }, { 0.316228, 0.316228, 0.316228 }, 0.1 * 128 },
                            { "obsidian"      , engine::eMaterialType::PHONG, { 0.05375, 0.05, 0.06625 }, { 0.18275, 0.17, 0.22525 }, { 0.332741, 0.328634, 0.346435 }, 0.3 * 128 },
                            { "pearl"         , engine::eMaterialType::PHONG, { 0.25, 0.20725, 0.20725 }, { 1.0, 0.829, 0.829 }, { 0.296648, 0.296648, 0.296648 }, 0.088 * 128 },
                            { "ruby"          , engine::eMaterialType::PHONG, { 0.1745, 0.01175, 0.01175 }, { 0.61424, 0.04136, 0.04136 }, { 0.727811, 0.626959, 0.626959 }, 0.6 * 128 },
                            { "turquoise"     , engine::eMaterialType::PHONG, { 0.1, 0.18725, 0.1745 }, { 0.396, 0.74151, 0.69102 }, { 0.297254, 0.30829, 0.306678 }, 0.1 * 128 },
                            { "brass"         , engine::eMaterialType::PHONG, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 }, { 0.992157, 0.941176, 0.807843 }, 0.21794872 * 128 },
                            { "bronze"        , engine::eMaterialType::PHONG, { 0.2125, 0.1275, 0.054 }, { 0.714, 0.4284, 0.18144 }, { 0.393548, 0.271906, 0.166721 }, 0.2 * 128 },
                            { "chrome"        , engine::eMaterialType::PHONG, { 0.25, 0.25, 0.25 }, { 0.4, 0.4, 0.4 }, { 0.774597, 0.774597, 0.774597 }, 0.6 * 128 },
                            { "copper"        , engine::eMaterialType::PHONG, { 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 }, { 0.256777, 0.137622, 0.086014 }, 0.1 * 128 },
                            { "gold"          , engine::eMaterialType::PHONG, { 0.24725, 0.1995, 0.0745 }, { 0.75164, 0.60648, 0.22648 }, { 0.628281, 0.555802, 0.366065 }, 0.4 * 128 },
                            { "silver"        , engine::eMaterialType::PHONG, { 0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 0.4 * 128 },
                            { "blackPlastic"  , engine::eMaterialType::PHONG, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 }, { 0.50, 0.50, 0.50 }, 0.25 * 128 },
                            { "cyanPlastic"   , engine::eMaterialType::PHONG, { 0.0, 0.1, 0.06 }, { 0.0, 0.50980392, 0.50980392 }, { 0.50196078, 0.50196078, 0.50196078 }, 0.25 * 128 },
                            { "greenPlastic"  , engine::eMaterialType::PHONG, { 0.0, 0.0, 0.0 }, { 0.1, 0.35, 0.1 }, { 0.45, 0.55, 0.45 }, 0.25 * 128 },
                            { "redPlastic"    , engine::eMaterialType::PHONG, { 0.0, 0.0, 0.0 }, { 0.5, 0.0, 0.0 }, { 0.7, 0.6, 0.6 }, 0.25 * 128 },
                            { "whitePlastic"  , engine::eMaterialType::PHONG, { 0.0, 0.0, 0.0 }, { 0.55, 0.55, 0.55 }, { 0.70, 0.70, 0.70 }, 0.25 * 128 },
                            { "yellowPlastic" , engine::eMaterialType::PHONG, { 0.0, 0.0, 0.0 }, { 0.5, 0.5, 0.0 }, { 0.60, 0.60, 0.50 }, 0.25 * 128 },
                            { "blackRubber"   , engine::eMaterialType::PHONG, { 0.02, 0.02, 0.02 }, { 0.01, 0.01, 0.01 }, { 0.4, 0.4, 0.4 }, 0.078125 * 128 },
                            { "cyanRubber"    , engine::eMaterialType::PHONG, { 0.0, 0.05, 0.05 }, { 0.4, 0.5, 0.5 }, { 0.04, 0.7, 0.7 }, 0.078125 * 128 },
                            { "greenRubber"   , engine::eMaterialType::PHONG, { 0.0, 0.05, 0.0 }, { 0.4, 0.5, 0.4 }, { 0.04, 0.7, 0.04 }, 0.078125 * 128 },
                            { "redRubber"     , engine::eMaterialType::PHONG, { 0.05, 0.0, 0.0 }, { 0.5, 0.4, 0.4 }, { 0.7, 0.04, 0.04 }, 0.078125 * 128 },
                            { "whiteRubber"   , engine::eMaterialType::PHONG, { 0.05, 0.05, 0.05 }, { 0.5, 0.5, 0.5 }, { 0.7, 0.7, 0.7 }, 0.078125 * 128 },
                            { "yelloRubber"   , engine::eMaterialType::PHONG, { 0.05, 0.05, 0.0 }, { 0.5, 0.5, 0.4 }, { 0.7, 0.7, 0.04 }, 0.078125 * 128 } };

        m_anyMenuHovered = false;
    }

    ~GuiMaterialsLayer() 
    {
        m_light = nullptr;
        m_material = nullptr;
    }

    void setMaterial( engine::CMaterial* material )
    {
        m_material = material;
    }

    void setLight( engine::CILight* light )
    {
        m_light = light;
    }

    void render() override
    {
        m_anyMenuHovered = false;
        m_wantsToCaptureMouse = false;

        _menuUiMaterial();
        _menuUiLight();

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
        if ( !m_material )
            return;

        ImGui::Begin( "Material-properties" );

        std::string _materialSelectionName = ( m_materialSelectedIndex == -1 ) ? "" : 
                                                            m_materialsList[m_materialSelectedIndex].name();

        if ( ImGui::BeginCombo( "materials-list", _materialSelectionName.c_str() ) )
        {
            for ( size_t i = 0; i < m_materialsList.size(); i++ )
            {
                bool _isSelected = ( _materialSelectionName == m_materialsList[i].name() );
                if ( ImGui::Selectable( m_materialsList[i].name().c_str(), _isSelected ) )
                    m_materialSelectedIndex = i;

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( ImGui::Button( "set material" ) && ( m_materialSelectedIndex != -1 ) )
        {
            m_material->ambient  = m_materialsList[m_materialSelectedIndex].ambient;
            m_material->diffuse  = m_materialsList[m_materialSelectedIndex].diffuse;
            m_material->specular = m_materialsList[m_materialSelectedIndex].specular;
            m_material->shininess = m_materialsList[m_materialSelectedIndex].shininess;
        }

        ImGui::Spacing();

        float _cAmbient[3]  = { m_material->ambient.x(), m_material->ambient.y(), m_material->ambient.z() };
        float _cDiffuse[3]  = { m_material->diffuse.x(), m_material->diffuse.y(), m_material->diffuse.z() };
        float _cSpecular[3] = { m_material->specular.x(), m_material->specular.y(), m_material->specular.z() };

        ImGui::ColorEdit3( "cAmbient", _cAmbient );
        ImGui::ColorEdit3( "cDiffuse", _cDiffuse );
        ImGui::ColorEdit3( "cSpecular", _cSpecular );
        ImGui::SliderFloat( "cShininess", &m_material->shininess, 32.0f, 256.0f );

        m_material->ambient  = { _cAmbient[0], _cAmbient[1], _cAmbient[2] };
        m_material->diffuse  = { _cDiffuse[0], _cDiffuse[1], _cDiffuse[2] };
        m_material->specular = { _cSpecular[0], _cSpecular[1], _cSpecular[2] };

        ImGui::Spacing();
        ImGui::Text( m_material->toString().c_str() );

        m_anyMenuHovered |= ImGui::IsWindowHovered();

        ImGui::End();
    }

    void _menuUiLight()
    {
        if ( !m_light )
            return;

        ImGui::Begin( "Light-color-properties" );

        ImGui::Checkbox( "funky???", &m_lightsAnimated );

        if ( m_lightsAnimated )
        {
            m_light->ambient.x() = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 2.0f );
            m_light->ambient.y() = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 0.7f );
            m_light->ambient.z() = 0.1f * std::sin( 10.0f * engine::CTime::GetWallTime() * 1.3f );
            m_light->diffuse = 5.0f * m_light->ambient;
            m_light->specular = { 1.0f, 1.0f, 1.0f };
        }
        else
        {
            float _cAmbient[3]  = { m_light->ambient.x(), m_light->ambient.y(), m_light->ambient.z() };
            float _cDiffuse[3]  = { m_light->diffuse.x(), m_light->diffuse.y(), m_light->diffuse.z() };
            float _cSpecular[3] = { m_light->specular.x(), m_light->specular.y(), m_light->specular.z() };

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

        m_anyMenuHovered |= ImGui::IsWindowHovered();

        ImGui::End();
    }

    engine::CMaterial* m_material;
    engine::CILight* m_light;
    std::vector< engine::CMaterial > m_materialsList;
    int m_materialSelectedIndex;
    bool m_lightsAnimated;

    bool m_anyMenuHovered;
    bool m_wantsToCaptureMouse;
};

int main()
{
    auto _app = new engine::CApplication();
    auto _uiLayer = new GuiMaterialsLayer( "Material-utils" );
    _app->addGuiLayer( std::unique_ptr< GuiMaterialsLayer >( _uiLayer ) );

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

    auto _box = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    auto _sphere = engine::CMeshBuilder::createSphere( 1.5f );
    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->position = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong_materials";
    auto _shaderLighting = engine::CShaderManager::CreateShaderFromFiles( "phong_material_shader",
                                                                          _baseNamePhong + "_vs.glsl",
                                                                          _baseNamePhong + "_fs.glsl" );

    ENGINE_ASSERT( _shaderLighting, "Could not load phong-material shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    engine::CMesh* _mesh = _box;

    /* create material properties */
    auto _phongMaterial = new engine::CMaterial( "phong_material", 
                                                 engine::eMaterialType::PHONG,
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

    _uiLayer->setMaterial( _phongMaterial );
    _uiLayer->setLight( _pointLight );

    while( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
        {
            break;
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
        {
            _camera->setActiveMode( false );
            _uiLayer->setActive( true );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
        {
            _camera->setActiveMode( true );
            _uiLayer->setActive( false );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_P ) )
        {
            _moveLight = !_moveLight;
            ENGINE_INFO( "Light state: {0}", ( _moveLight ) ? "moving" : "fixed" );
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();
        _camera->update();

        if ( _moveLight )
        {
            _mvParam += 10.0f * engine::CTime::GetTimeStep();
            // _gizmo->position.x() = 1.0f + std::sin( _mvParam ) * 2.0f;
            // _gizmo->position.y() = std::sin( _mvParam / 2.0f ) * 1.0f;

            _pointLight->position.x() = 4.0f * std::sin( _mvParam );
            _pointLight->position.y() = 4.0f * std::cos( _mvParam );
            _pointLight->position.z() = 0.0f;
        }

        _gizmo->position = _pointLight->position;

        /* do our thing here ************************/
        _shaderLighting->bind();
        _shaderLighting->setMat4( "u_modelMatrix", _mesh->matModel() );
        _shaderLighting->setMat4( "u_viewProjMatrix", _camera->matProj() * _camera->matView() );
        _shaderLighting->setMat4( "u_normalMatrix", tinymath::inverse( _mesh->matModel() ).transpose() );
        _shaderLighting->setVec3( "u_material.ambient", _phongMaterial->ambient );
        _shaderLighting->setVec3( "u_material.diffuse", _phongMaterial->diffuse );
        _shaderLighting->setVec3( "u_material.specular", _phongMaterial->specular );
        _shaderLighting->setFloat( "u_material.shininess", _phongMaterial->shininess );
        _shaderLighting->setVec3( "u_light.ambient", _pointLight->ambient );
        _shaderLighting->setVec3( "u_light.diffuse", _pointLight->diffuse );
        _shaderLighting->setVec3( "u_light.specular", _pointLight->specular );
        _shaderLighting->setFloat( "u_light.intensity", _pointLight->intensity );
        _shaderLighting->setVec3( "u_light.position", _pointLight->position );
        _shaderLighting->setVec3( "u_viewerPosition", _camera->position() );

        _mesh->render();

        _shaderLighting->unbind();

        _shaderGizmo->bind();
        _shaderGizmo->setMat4( "u_tModel", _gizmo->matModel() );
        _shaderGizmo->setMat4( "u_tView", _camera->matView() );
        _shaderGizmo->setMat4( "u_tProj", _camera->matProj() );
        _shaderGizmo->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

        _gizmo->render();

        _shaderGizmo->unbind();
        /********************************************/

        // engine::CDebugDrawer::DrawNormals( _mesh, { 0.0f, 0.0f, 1.0f } );
        _app->render();
        _app->end();

        // ENGINE_TRACE( "frame-time: {0}", engine::CTime::GetRawTimeStep() );
    }

    return 0;
}