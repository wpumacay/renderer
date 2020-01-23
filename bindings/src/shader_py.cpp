
#include <shader_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_shader( py::module& m )
    {
        py::class_< CShader >( m, "Shader" )
            .def( "bind", &CShader::bind )
            .def( "unbind", &CShader::unbind )
            .def( "setInt", &CShader::setInt )
            .def( "setFloat", &CShader::setFloat )
            .def( "setVec2",[]( CShader& self, const std::string& uName, py::array_t<engine::float32>& vec )
                {
                    self.setVec2( uName, tinymath::nparray_to_vector<engine::float32, 2>( vec ) );
                } )
            .def( "setVec3",[]( CShader& self, const std::string& uName, py::array_t<engine::float32>& vec )
                {
                    self.setVec3( uName, tinymath::nparray_to_vector<engine::float32, 3>( vec ) );
                } )
            .def( "setVec4",[]( CShader& self, const std::string& uName, py::array_t<engine::float32>& vec )
                {
                    self.setVec4( uName, tinymath::nparray_to_vector<engine::float32, 4>( vec ) );
                } )
            .def( "setMat4",[]( CShader& self, const std::string& uName, py::array_t<engine::float32>& mat )
                {
                    self.setMat4( uName, tinymath::nparray_to_matrix<engine::float32, 4>( mat ) );
                } )
            .def_property_readonly( "name", &CShader::name )
            .def_property_readonly( "openglId", &CShader::openglId );
    }

    void bindings_shaderManager( py::module& m )
    {
        py::class_< CShaderManager >( m, "ShaderManager" )
            .def_static( "Init", &CShaderManager::Init )
            .def_static( "Release", &CShaderManager::Release )
            .def_static( "CreateShaderFromFiles", &CShaderManager::CreateShaderFromFiles, py::return_value_policy::reference )
            .def_static( "CreateShaderFromSources", &CShaderManager::CreateShaderFromSources, py::return_value_policy::reference )
            .def_static( "GetCachedShader", &CShaderManager::GetCachedShader, py::return_value_policy::reference );
    }
}