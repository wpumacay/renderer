
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
        py::enum_< engine::eBufferUsage >( m, "BufferUsage", py::arithmetic() )
            .value( "STATIC", engine::eBufferUsage::STATIC )
            .value( "DYNAMIC", engine::eBufferUsage::DYNAMIC );

        py::class_< CVertexBuffer >( m, "VertexBuffer" )
            .def( py::init( []( const CVertexBufferLayout& layout, const eBufferUsage& usage, uint32 size, py::array_t<float32> data )
                {
                    if ( size != ( data.size() * sizeof( float32 ) ) )
                        throw std::runtime_error( "VertexBuffer >>> given size argument must match size of the np-array buffer. \
                                                   Given size was " + std::to_string( size ) + " but np-array size is " + std::to_string( data.size() * sizeof( float32 ) ) );

                    return new CVertexBuffer( layout, usage, size, (float32*) data.request().ptr, true );
                }
            ) )
            .def( "bind", &CVertexBuffer::bind )
            .def( "unbind", &CVertexBuffer::unbind )
            .def( "updateData", []( CVertexBuffer& self, uint32 size, py::array_t<float32> data )
                {
                    if ( size != ( data.size() * sizeof( float32 ) ) )
                        throw std::runtime_error( "VertexBuffer::updateData >>> given size argument must match size of the np-array buffer. \
                                                   Given size was " + std::to_string( size ) + " but np-array size is " + std::to_string( data.size() * sizeof( float32 ) ) );

                    self.updateData( size, (float32*) data.request().ptr );
                } )
            .def( "layout", &CVertexBuffer::layout )
            .def( "size", &CVertexBuffer::size )
            .def( "usage", &CVertexBuffer::usage )
            .def( "openglId", &CVertexBuffer::openglId );
    }

    void bindings_indexBuffer( py::module& m )
    {
        py::class_< CIndexBuffer >( m, "IndexBuffer", py::arithmetic() )
            .def( py::init( []( const eBufferUsage& usage, uint32 count, py::array_t<uint32> data )
                {
                    if ( count != data.size() )
                        throw std::runtime_error( "IndexBuffer >>> given count argument must match number of elements in np-array. \
                                                   Given count was " + std::to_string( count ) + " but np-array size is " + std::to_string( data.size() ) );

                    return new CIndexBuffer( usage, count, (uint32*) data.request().ptr, true );
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
            .def( "count", &CIndexBuffer::count );
    }

    void bindings_vertexArray( py::module& m )
    {
        py::class_< CVertexArray >( m, "VertexArray" )
            .def( py::init<>() )
            .def( "addVertexBuffer", &CVertexArray::addVertexBuffer, py::arg( "vertexBuffer" ), py::arg( "isInstanced" ) = false, py::keep_alive<1, 2>() )
            //// @todo: enable once drake's fork of pybind is used (pass ownership)
            //// .def( "addVertexBuffer", []( CVertexArray& self, std::unique_ptr< CVertexBuffer > vbuffer )
            ////     {
            ////         self.addVertexBuffer( std::move( vbuffer ) );
            ////     } )
            .def( "setIndexBuffer", &CVertexArray::setIndexBuffer, py::keep_alive<1, 2>() )
            //// @todo: enable once drake's fork of pybind is used (pass ownership)
            //// .def( "setIndexBuffer", []( CVertexArray& self, std::unique_ptr< CIndexBuffer > ibuffer )
            ////     {
            ////         self.setIndexBuffer( std::move( ibuffer ) );
            ////     } )
            .def( "bind", &CVertexArray::bind )
            .def( "unbind", &CVertexArray::unbind )
            .def( "vertexBuffers", &CVertexArray::vertexBuffers )
            .def( "indexBuffer", &CVertexArray::indexBuffer )
            .def( "openglId", &CVertexArray::openglId );
    }
}