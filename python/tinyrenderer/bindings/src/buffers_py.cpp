
#include <buffers_py.h>
#include <stdexcept>

namespace py = pybind11;

namespace engine
{
    void bindings_bufferLayout( py::module& m )
    {
        py::enum_< engine::eElementType >( m, "ElementType", py::arithmetic() )
            .value( "Float", engine::eElementType::Float )
            .value( "Float2", engine::eElementType::Float2 )
            .value( "Float3", engine::eElementType::Float3 )
            .value( "Float4", engine::eElementType::Float4 )
            .value( "Int", engine::eElementType::Int )
            .value( "Int2", engine::eElementType::Int2 )
            .value( "Int3", engine::eElementType::Int3 )
            .value( "Int4", engine::eElementType::Int4 );

        py::class_< CVertexBufferLayout >( m, "VertexBufferLayout" )
            .def( py::init<>() )
            .def( py::init( []( py::list py_elements )
                {
                    auto cpp_elements = std::vector< CVertexBufferElement >();
                    for ( auto& py_element : py_elements )
                    {
                        if ( !py::isinstance<py::list>( py_element ) )
                            std::runtime_error( "VertexBufferLayout >>> buffer elements must lists " );

                        auto py_bufferElement = py::reinterpret_borrow<py::list>( py_element );

                        if ( py_bufferElement.size() != 3 )
                            throw std::runtime_error( "VertexBufferLayout >>> buffer elements must be lists of size 3," );

                        if ( !py::isinstance<py::str>( py_bufferElement[0] ) ||
                             !py::isinstance<eElementType>( py_bufferElement[1] ) ||
                             !py::isinstance<py::bool_>( py_bufferElement[2] ) )
                            throw std::runtime_error( "VertexBufferLayout >>> wrong types given to buffer element. Expected [str,tr.ElementType,bool]" );

                        auto cpp_element = CVertexBufferElement( py::str( py_bufferElement[0] ), 
                                                                 py::cast<eElementType>( py_bufferElement[1] ), 
                                                                 py::bool_( py_bufferElement[2] ) );
                        cpp_elements.push_back( cpp_element );
                    }

                    return new CVertexBufferLayout( cpp_elements );
                }
             ) )
            .def( "__repr__", []( const CVertexBufferLayout& self )
                {
                    auto _strrep = std::string( "BufferLayout(\n" );
                    auto cpp_elements = self.elements();
                    for ( auto& element : cpp_elements )
                    {
                        _strrep += "name : " + element.name + " - ";
                        _strrep += "type : " + engine::toString( element.type ) + " - ";
                        _strrep += "count : " + std::to_string( element.count ) + " - ";
                        _strrep += "size : " + std::to_string( element.size ) + " - ";
                        _strrep += "offset : " + std::to_string( element.offset ) + " - ";
                        _strrep += "normalized : " + std::string( element.normalized ? "True" : "False" ) + "\n";
                    }
                    _strrep += ")";
                    return _strrep;
                } );
    }

    void bindings_vertexBuffer( py::module& m )
    {
        py::enum_<engine::eBufferUsage>( m, "BufferUsage", py::arithmetic() )
            .value( "STATIC", engine::eBufferUsage::STATIC )
            .value( "DYNAMIC", engine::eBufferUsage::DYNAMIC );

        py::class_<CVertexBuffer, CVertexBuffer::uptr>( m, "VertexBuffer" )
            .def( py::init( []( const CVertexBufferLayout& layout, const eBufferUsage& usage, uint32 size, py::array_t<float32> data )
                {
                    if ( size != ( data.size() * sizeof( float32 ) ) )
                        throw std::runtime_error( "VertexBuffer >>> given size argument must match size of the np-array buffer. \
                                                   Given size was " + std::to_string( size ) + " but np-array size is " + std::to_string( data.size() * sizeof( float32 ) ) );

                    return std::make_unique<CVertexBuffer>( layout, usage, size, (const float32*) data.request().ptr );
                }
            ), py::arg( "layout" ), py::arg( "usage" ), py::arg( "size" ), py::arg( "data" ) )
            .def( "Bind", &CVertexBuffer::Bind )
            .def( "Unbind", &CVertexBuffer::Unbind )
            .def( "Resize", &CVertexBuffer::Resize )
            .def( "UpdateData", []( CVertexBuffer& self, uint32 size, py::array_t<float32> data )
                {
                    if ( size != ( data.size() * sizeof( float32 ) ) )
                        throw std::runtime_error( "VertexBuffer::updateData >>> given size argument must match size of the np-array buffer. \
                                                   Given size was " + std::to_string( size ) + " but np-array size is " + std::to_string( data.size() * sizeof( float32 ) ) );

                    self.UpdateData( size, (const float32*) data.request().ptr );
                } )
            .def_property_readonly( "layout", &CVertexBuffer::layout )
            .def_property_readonly( "size", &CVertexBuffer::size )
            .def_property_readonly( "usage", &CVertexBuffer::usage )
            .def_property_readonly( "opengl_id", &CVertexBuffer::opengl_id )
            .def( "__repr__", []( const CVertexBuffer& self )
                {
                    auto _strrep = std::string( "VertexBuffer(\n" );
                    _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( &self ) + "\n";
                    _strrep += "usage       : " + engine::ToString( self.usage() ) + "\n";
                    _strrep += "size        : " + std::to_string( self.size() ) + "\n";
                    _strrep += "opengl-ID   : " + std::to_string( self.opengl_id() ) + ")";
                    return _strrep;
                } );
    }

    void bindings_indexBuffer( py::module& m )
    {
        py::class_< CIndexBuffer >( m, "IndexBuffer", py::arithmetic() )
            .def( py::init( []( const eBufferUsage& usage, uint32 count, py::array_t<uint32> data )
                {
                    if ( count != data.size() )
                        throw std::runtime_error( "IndexBuffer >>> given count argument must match number of elements in np-array. \
                                                   Given count was " + std::to_string( count ) + " but np-array size is " + std::to_string( data.size() ) );

                    return std::make_unique<CIndexBuffer>( usage, count, (uint32*) data.request().ptr );
                }
            ) )
            .def( "bind", &CIndexBuffer::bind )
            .def( "unbind", &CIndexBuffer::unbind )
            .def( "updateData", []( CIndexBuffer& self, uint32 count, py::array_t<uint32> data )
                {
                    if ( count != data.size() )
                        throw std::runtime_error( "IndexBuffer::updateData >>> given count argument must match number of elements in np-array. \
                                                   Given count was " + std::to_string( count ) + " but np-array size is " + std::to_string( data.size() ) );

                    self.updateData( count, (uint32*) data.request().ptr );
                } )
            .def( "count", &CIndexBuffer::count )
            .def( "__repr__", []( const CIndexBuffer& self )
                {
                    auto _strrep = std::string( "IndexBuffer(\n" );
                    _strrep += "cpp-address: " + tinyutils::PointerToHexAddress( &self ) + "\n";
                    _strrep += "usage: " + engine::ToString( self.usage() ) + "\n";
                    _strrep += "indices-count: " + std::to_string( self.count() ) + "\n";
                    _strrep += "openglId: " + std::to_string( self.openglId() ) + ")";
                    return _strrep;
                } );
    }

    void bindings_vertexArray( py::module& m )
    {
        py::class_< CVertexArray >( m, "VertexArray" )
            .def( py::init( []()
                {
                    return std::make_unique<CVertexArray>();
                } 
            ) )
            .def( "addVertexBuffer", []( CVertexArray& self, std::unique_ptr< CVertexBuffer > vbuffer )
                {
                    self.addVertexBuffer( std::move( vbuffer ) );
                } )
            .def( "setIndexBuffer", []( CVertexArray& self, std::unique_ptr< CIndexBuffer > ibuffer )
                {
                    self.setIndexBuffer( std::move( ibuffer ) );
                } )
            .def( "bind", &CVertexArray::bind )
            .def( "unbind", &CVertexArray::unbind )
            .def( "vertexBuffers", []( CVertexArray& self ) -> std::vector<CVertexBuffer*>
                {
                    auto _vboRefs = std::vector<CVertexBuffer*>();
                    auto& _vbos = self.vertexBuffers();
                    for ( auto& _vbo : _vbos )
                        _vboRefs.push_back( _vbo.get() );
                    return _vboRefs;
                }, py::return_value_policy::reference )
            .def( "indexBuffer", []( CVertexArray& self ) -> CIndexBuffer*
                {
                    return self.indexBuffer().get();
                }, py::return_value_policy::reference )
            .def( "openglId", &CVertexArray::openglId )
            .def( "__repr__", []( const CVertexArray& self )
                {
                    auto _strrep = std::string( "VertexArray(\n" );
                    _strrep += "cpp-address: " + tinyutils::PointerToHexAddress( &self ) + "\n";
                    _strrep += "num-attribs: " + std::to_string( self.numAttribs() ) + "\n";
                    _strrep += "num-vbos: " + std::to_string( self.vertexBuffers().size() ) + "\n";
                    _strrep += "num-ebos: " + std::to_string( self.indexBuffer() ? 1 : 0 ) + "\n";
                    _strrep += "openglId: " + std::to_string( self.openglId() ) + ")";
                    return _strrep;
                } );
    }
}