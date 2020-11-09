
#include <common_py.h>
#include <shaders/CShader.h>
#include <shaders/CProgram.h>
#include <shaders/CShaderManager.h>

namespace py = pybind11;

namespace engine
{
    void bindings_shaders( py::module& m )
    {
        py::enum_<engine::eShaderType>( m, "ShaderType", py::arithmetic() )
            .value( "VERTEX", engine::eShaderType::VERTEX )
            .value( "FRAGMENT", engine::eShaderType::FRAGMENT );

        py::class_<CShader, CShader::ptr>( m, "Shader" )
            .def( py::init<const std::string&, const eShaderType&, const std::string&>() )
            .def_property_readonly( "name", &CShader::name )
            .def_property_readonly( "type", &CShader::type )
            .def_property_readonly( "source", &CShader::source )
            .def_property_readonly( "opengl_id", &CShader::opengl_id )
            .def_property_readonly( "compiled", &CShader::compiled )
            .def( "__repr__", []( const CShader* self )
                {
                    std::string strrep = "Shader(\n";
                    strrep += "name     : " + self->name() + "\n";
                    strrep += "type     : " + engine::ToString( self->type() ) + "\n";
                    strrep += "compiled : " + std::string( self->compiled() ? "yes" : "no" ) + "\n";
                    strrep += "opengl_id: " + std::to_string( self->opengl_id() ) + "\n";
                    strrep += "source   :\n" + self->source() + "\n";
                    strrep += ")";
                    return strrep;
                } );

        py::class_<CProgram, CProgram::ptr>( m, "Program" )
            .def( py::init<const std::string, const std::string&, const std::string&>() )
            .def( "Bind", &CProgram::Bind )
            .def( "Unbind", &CProgram::Unbind )
            .def( "SetInt", []( CProgram* self, const std::string& name, const int32& value )
                {
                    self->SetInt( name, value );
                } )
            .def( "SetFloat", []( CProgram* self, const std::string& name, const float32& value )
                {
                    self->SetFloat( name, value );
                } )
            .def( "SetVec2", []( CProgram* self, const std::string& name, const py::array_t<float32>& vec )
                {
                    self->SetVec2( name, tinymath::nparray_to_vector<float32, 2>( vec ) );
                } )
            .def( "SetVec3", []( CProgram* self, const std::string& name, const py::array_t<float32>& vec )
                {
                    self->SetVec3( name, tinymath::nparray_to_vector<float32, 3>( vec ) );
                } )
            .def( "SetVec4", []( CProgram* self, const std::string& name, const py::array_t<float32>& vec )
                {
                    self->SetVec4( name, tinymath::nparray_to_vector<float32, 4>( vec ) );
                } )
            .def( "SetMat4", []( CProgram* self, const std::string& name, const py::array_t<float32>& mat )
                {
                    self->SetMat4( name, tinymath::nparray_to_matrix<float32, 4>( mat ) );
                } )
            .def( "GetVertexShaderSource", &CProgram::GetVertexShaderSource )
            .def( "GetFragmentShaderSource", &CProgram::GetFragmentShaderSource )
            .def_property_readonly( "linked", &CProgram::linked )
            .def_property_readonly( "name", &CProgram::name )
            .def_property_readonly( "opengl_id", &CProgram::opengl_id )
            .def( "__repr__", []( const CProgram* self )
                {
                    std::string strrep = "Program(\n";
                    strrep += "name     : " + self->name();
                    strrep += "linked   : " + std::string( self->linked() ? "yes" : "no" ) + "\n";
                    strrep += "opengl_id: " + std::to_string( self->opengl_id() ) + "\n";
                    strrep += "vert-src :\n" + self->GetVertexShaderSource() + "\n";
                    strrep += "frag-src :\n" + self->GetFragmentShaderSource() + "\n";
                    strrep += ");";
                    return strrep;
                } );

        py::class_<CShaderManager>( m, "ShaderManager" )
            .def_static( "Init", &CShaderManager::Init )
            .def_static( "Release", &CShaderManager::Release )
            .def_static( "CreateProgramFromFiles", &CShaderManager::CreateProgramFromFiles )
            .def_static( "CreateProgramFromSources", &CShaderManager::CreateProgramFromSources )
            .def_static( "GetCachedProgram", &CShaderManager::GetCachedProgram );
    }
}