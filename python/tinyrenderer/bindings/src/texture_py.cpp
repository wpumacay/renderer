
#include <texture_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_texture( py::module& m )
    {
        // Expose eTextureFormat enumerator
        py::enum_< engine::eTextureFormat >( m, "TextureFormat", py::arithmetic() )
            .value( "NONE", engine::eTextureFormat::NONE )
            .value( "RGB", engine::eTextureFormat::RGB )
            .value( "RGBA", engine::eTextureFormat::RGBA )
            .value( "DEPTH", engine::eTextureFormat::DEPTH )
            .value( "STENCIL", engine::eTextureFormat::STENCIL )
            .value( "DEPTH24_STENCIL8", engine::eTextureFormat::DEPTH24_STENCIL8 );

        // Expose eTextureWrap enumerator
        py::enum_< engine::eTextureWrap >( m, "TextureWrap", py::arithmetic() )
            .value( "NONE", engine::eTextureWrap::NONE )
            .value( "REPEAT", engine::eTextureWrap::REPEAT )
            .value( "REPEAT_MIRROR", engine::eTextureWrap::REPEAT_MIRROR )
            .value( "CLAMP_TO_EDGE", engine::eTextureWrap::CLAMP_TO_EDGE )
            .value( "CLAMP_TO_BORDER", engine::eTextureWrap::CLAMP_TO_BORDER );

        // Expose eTextureFilter enumerator
        py::enum_< engine::eTextureFilter >( m, "TextureFilter", py::arithmetic() )
            .value( "NONE", engine::eTextureFilter::NONE )
            .value( "NEAREST", engine::eTextureFilter::NEAREST )
            .value( "LINEAR", engine::eTextureFilter::LINEAR );

        // Expose CTextureOptions struct (contains description for GL-specific configuration of the texture)
        py::class_< CTextureOptions >( m, "TextureOptions" )
            .def( py::init<>() )
            .def_readwrite( "filterMin", &CTextureOptions::filterMin )
            .def_readwrite( "filterMag", &CTextureOptions::filterMag )
            .def_readwrite( "wrapU", &CTextureOptions::wrapU )
            .def_readwrite( "wrapV", &CTextureOptions::wrapV )
            .def_readwrite( "dtype", &CTextureOptions::dtype )
            .def_property( "borderColorU", 
                []( const CTextureOptions& self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<tinymath::tfloat,4>( self.borderColorU );
                    },
                []( CTextureOptions& self, const py::array_t<tinymath::tfloat>& bColorU ) -> void
                    {
                        self.borderColorU = tinymath::nparray_to_vector<tinymath::tfloat,4>( bColorU );
                    } )
            .def_property( "borderColorV",
                []( const CTextureOptions& self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<tinymath::tfloat,4>( self.borderColorV );
                    },
                []( CTextureOptions& self, const py::array_t<tinymath::tfloat>& bColorV ) -> void
                    {
                        self.borderColorV = tinymath::nparray_to_vector<tinymath::tfloat,4>( bColorV );
                    } )
            .def( "__repr__", []( const CTextureOptions& self )
                {
                    auto _strrep = std::string( "TextureOptions(\n" );
                    _strrep += "filterMin: " + engine::toString( self.filterMin ) + "\n";
                    _strrep += "filterMag: " + engine::toString( self.filterMag ) + "\n";
                    _strrep += "wrapU: " + engine::toString( self.wrapU ) + "\n";
                    _strrep += "wrapV: " + engine::toString( self.wrapV ) + "\n";
                    _strrep += "borderColorU: " + tinymath::toString( self.borderColorU ) + "\n";
                    _strrep += "borderColorV: " + tinymath::toString( self.borderColorV ) + "\n";
                    _strrep += "dtype: " + engine::toString( self.dtype ) + ")";

                    return _strrep;
                } );

        // Expose CTextureData struct (contains description of the texture and its data in CPU)
        py::class_< CTextureData >( m, "TextureData" )
            .def( py::init<>() )
            .def_readwrite( "name", &CTextureData::name )
            .def_readwrite( "width", &CTextureData::width )
            .def_readwrite( "height", &CTextureData::height )
            .def_readwrite( "channels", &CTextureData::channels )
            .def_readwrite( "format", &CTextureData::format )
            .def_readwrite( "internalFormat", &CTextureData::internalFormat )
            .def( "__repr__", []( const CTextureData& self )
                {
                    auto _strrep = std::string( "TextureData(\n" );
                    _strrep += "cpp-address: " + engine::pointerToHexAddress( &self ) + "\n";
                    _strrep += engine::toString( self );
                    return _strrep;
                } );
            // @todo: allow users to create its own textures by passing texture data
            //// .def_property( "data", []() -> py::array_t<uint8> {}, []() -> void {} )

        // Expose CTexture class (abstraction on a GPU texture)
        py::class_< CTexture >( m, "Texture" )
            .def( py::init( []( std::unique_ptr<CTextureData> texData,
                                const eTextureFilter& filterMin,
                                const eTextureFilter& filterMag,
                                const eTextureWrap& wrapU,
                                const eTextureWrap& wrapV,
                                const py::array_t<float32>& borderColorU,
                                const py::array_t<float32>& borderColorV,
                                const ePixelDataType& dtype )
                {
                    return new CTexture( std::move( texData ),
                                         filterMin, filterMag, 
                                         wrapU, wrapV, 
                                         tinymath::nparray_to_vector<tinymath::tfloat, 4>( borderColorU ), 
                                         tinymath::nparray_to_vector<tinymath::tfloat, 4>( borderColorV ), 
                                         dtype );
                } ) )
            .def( "bind", &CTexture::bind )
            .def( "unbind", &CTexture::unbind )
            .def( "resize", []( CTexture& self, int width, int height, const py::array_t<uint8>& arr_data )
                {
                    auto arr_info = arr_data.request();
                    if ( arr_info.size != ( width * height ) )
                        throw std::runtime_error( "Texture::resize >>> given buffer and dimensions don't match" );
                    self.resize( width, height, (const uint8*) arr_info.ptr );
                } )
            .def( "data", []( CTexture& self ) -> CTextureData*
                {
                    return self.data().get(); // @todo: remove return unique_ptr& in CTexture.h
                }, py::return_value_policy::reference_internal )
            .def_property_readonly( "name", &CTexture::name )
            .def_property_readonly( "width", &CTexture::width )
            .def_property_readonly( "height", &CTexture::height )
            .def_property_readonly( "channels", &CTexture::channels )
            .def_property_readonly( "format", &CTexture::format )
            .def_property_readonly( "filterMin", &CTexture::filterMin )
            .def_property_readonly( "filterMag", &CTexture::filterMag )
            .def_property_readonly( "wrapU", &CTexture::wrapU )
            .def_property_readonly( "wrapV", &CTexture::wrapV )
            .def_property_readonly( "dtype", &CTexture::dtype )
            .def_property_readonly( "openglId", &CTexture::openglId )
            .def_property_readonly( "borderColorU", []( const CTexture& self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray( self.borderColorU() );
                } )
            .def_property_readonly( "borderColorV", []( const CTexture& self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray( self.borderColorV() );
                } );
    }

    void bindings_textureCube( py::module& m )
    {
        // Expose CTextureCubeData struct (contains description of the texture-cube and its data in CPU)
        py::class_< CTextureCubeData >( m, "TextureCubeData" )
            .def( py::init<>() )
            .def_readwrite( "name", &CTextureCubeData::name )
            .def_readwrite( "width", &CTextureCubeData::width )
            .def_readwrite( "height", &CTextureCubeData::height )
            .def_readwrite( "channels", &CTextureCubeData::channels )
            .def_readwrite( "format", &CTextureCubeData::format );

        // Expose CTextureCube class (abstraction on a GPU cubemap)
        py::class_< CTextureCube >( m, "TextureCube" )
            .def( py::init<std::unique_ptr<CTextureCubeData>>() )
            .def( "bind", &CTextureCube::bind )
            .def( "unbind", &CTextureCube::unbind )
            .def( "data", []( CTextureCube& self ) -> CTextureCubeData*
                {
                    return self.data().get();
                }, py::return_value_policy::reference )
            .def_property_readonly( "name", &CTextureCube::name )
            .def_property_readonly( "width", &CTextureCube::width )
            .def_property_readonly( "height", &CTextureCube::height )
            .def_property_readonly( "channels", &CTextureCube::channels )
            .def_property_readonly( "format", &CTextureCube::format )
            .def_property_readonly( "openglId", &CTextureCube::openglId );
    }

    void bindings_textureManager( py::module& m )
    {
        py::class_< CTextureManager >( m, "TextureManager" )
            .def_static( "Init", &CTextureManager::Init )
            .def_static( "Release", &CTextureManager::Release )
            .def_static( "LoadTexture", []( const std::string& filepath,
                                            const eTextureFilter& filterMin,
                                            const eTextureFilter& filterMag,
                                            const eTextureWrap& wrapU,
                                            const eTextureWrap& wrapV,
                                            const py::array_t<engine::float32>& borderColorU,
                                            const py::array_t<engine::float32>& borderColorV,
                                            const ePixelDataType& dtype = ePixelDataType::UINT_8,
                                            bool flipVertically ) -> CTexture*
                {
                    return CTextureManager::LoadTexture( filepath,
                                                         filterMin, filterMag,
                                                         wrapU, wrapV,
                                                         tinymath::nparray_to_vector<engine::float32,4>( borderColorU ),
                                                         tinymath::nparray_to_vector<engine::float32,4>( borderColorV ),
                                                         dtype,
                                                         flipVertically );
                }, py::return_value_policy::reference )
            .def_static( "LoadTexture", []( const std::string& filepath,
                                            const CTextureOptions& texOptions,
                                            bool flipVertically ) -> CTexture*
                {
                    return CTextureManager::LoadTexture( filepath, texOptions, flipVertically );
                }, py::return_value_policy::reference )
            .def_static( "LoadTextureCube", []( const std::array< std::string, 6 >& filepaths, 
                                                bool flipVertically ) -> CTextureCube*
                {
                    return CTextureManager::LoadTextureCube( filepaths, flipVertically );
                }, py::return_value_policy::reference )
            .def_static( "HasCachedTextureData", &CTextureManager::HasCachedTextureData )
            .def_static( "HasCachedTextureCubeData", &CTextureManager::HasCachedTextureCubeData )
            .def_static( "HasCachedTexture", &CTextureManager::HasCachedTexture )
            .def_static( "HasCachedTextureCube", &CTextureManager::HasCachedTextureCube )
            .def_static( "GetCachedTextureData", &CTextureManager::GetCachedTextureData, py::return_value_policy::reference )
            .def_static( "GetCachedTextureCubeData", &CTextureManager::GetCachedTextureCubeData, py::return_value_policy::reference )
            .def_static( "GetCachedTexture", &CTextureManager::GetCachedTexture, py::return_value_policy::reference )
            .def_static( "GetCachedTextureCube", &CTextureManager::GetCachedTextureCube, py::return_value_policy::reference )
            .def_static( "GetAllCachedTexturesData", &CTextureManager::GetAllCachedTexturesData )
            .def_static( "GetAllCachedTextures", &CTextureManager::GetAllCachedTextures )
            .def_static( "GetAllCachedTexturesCubeData", &CTextureManager::GetAllCachedTexturesCubeData )
            .def_static( "GetAllCachedTexturesCube", &CTextureManager::GetAllCachedTexturesCube );
    }
}