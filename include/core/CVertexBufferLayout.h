#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    enum class eElementType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4
    };

    std::string toString( const eElementType& type );
    uint32 toOpenGLType( const eElementType& type );

    uint32 sizeOfElement( const eElementType& type );
    uint32 countOfElement( const eElementType& type );

    struct CVertexBufferElement
    {
        std::string     name;
        eElementType    type;
        uint32          count;
        uint32          size;
        uint32          offset;
        bool            normalized;

        CVertexBufferElement();
        CVertexBufferElement( const std::string& name,
                              const eElementType& type,
                              bool normalized );
    };

    class CVertexBufferLayout
    {

    public :
        CVertexBufferLayout();
        CVertexBufferLayout( const std::initializer_list< CVertexBufferElement >& elements );

        uint32 stride() { return m_stride; }
        std::vector< CVertexBufferElement > elements() { return m_elements; }

    private :

        void _initialize();

    private :
        uint32 m_stride;
        std::vector< CVertexBufferElement > m_elements;
    };


}