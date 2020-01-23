
#include <core/CVertexBufferLayout.h>

namespace engine
{

    std::string toString( const eElementType& type )
    {
        if ( type == eElementType::Float )  return "Float";
        if ( type == eElementType::Float2 ) return "Float2";
        if ( type == eElementType::Float3 ) return "Float3";
        if ( type == eElementType::Float4 ) return "Float4";

        if ( type == eElementType::Int )    return "Int";
        if ( type == eElementType::Int2 )   return "Int2";
        if ( type == eElementType::Int3 )   return "Int3";
        if ( type == eElementType::Int4 )   return "Int4";

        ENGINE_CORE_ASSERT( false, "Invalid element type enum" );
        return "undefined";
    }

    uint32 toOpenGLType( const eElementType& type )
    {
        if ( type == eElementType::Float )  return GL_FLOAT;
        if ( type == eElementType::Float2 ) return GL_FLOAT;
        if ( type == eElementType::Float3 ) return GL_FLOAT;
        if ( type == eElementType::Float4 ) return GL_FLOAT;

        if ( type == eElementType::Int )    return GL_INT;
        if ( type == eElementType::Int2 )   return GL_INT;
        if ( type == eElementType::Int3 )   return GL_INT;
        if ( type == eElementType::Int4 )   return GL_INT;

        ENGINE_CORE_ASSERT( false, "Invalid element type enum" );
        return 0;
    }

    uint32 sizeOfElement( const eElementType& type )
    {
        if ( type == eElementType::Float )  return 4;
        if ( type == eElementType::Float2 ) return 4 * 2;
        if ( type == eElementType::Float3 ) return 4 * 3;
        if ( type == eElementType::Float4 ) return 4 * 4;

        if ( type == eElementType::Int )    return 4;
        if ( type == eElementType::Int2 )   return 4 * 2;
        if ( type == eElementType::Int3 )   return 4 * 3;
        if ( type == eElementType::Int4 )   return 4 * 4;

        ENGINE_CORE_ASSERT( false, "Invalid element type enum" );
        return 0;
    }

    uint32 countOfElement( const eElementType& type )
    {
        if ( type == eElementType::Float )  return 1;
        if ( type == eElementType::Float2 ) return 2;
        if ( type == eElementType::Float3 ) return 3;
        if ( type == eElementType::Float4 ) return 4;

        if ( type == eElementType::Int )    return 1;
        if ( type == eElementType::Int2 )   return 2;
        if ( type == eElementType::Int3 )   return 3;
        if ( type == eElementType::Int4 )   return 4;

        ENGINE_CORE_ASSERT( false, "Invalid element type enum" );
        return 0;
    }

    /**************************************************************************
    *                         CVertexBufferElement impl.                      *
    ***************************************************************************/

    CVertexBufferElement::CVertexBufferElement()
    {
        this->name          = "none";
        this->type          = eElementType::None;
        this->size          = 0;
        this->offset        = 0;
        this->count         = 0;
        this->normalized    = false;
    }

    CVertexBufferElement::CVertexBufferElement( const std::string& name,
                                                const eElementType& type,
                                                bool normalized )
    {
        this->name          = name;
        this->type          = type;
        this->count         = countOfElement( type );
        this->size          = sizeOfElement( type );
        this->offset        = 0; // written by layout when defined
        this->normalized    = normalized;
    }

    /**************************************************************************
    *                         CVertexBufferLayout impl.                       *
    ***************************************************************************/

    CVertexBufferLayout::CVertexBufferLayout()
    {
        m_stride = 0;
    }

    CVertexBufferLayout::CVertexBufferLayout( const std::initializer_list< CVertexBufferElement >& elements )
    {
        m_stride = 0;
        m_elements = elements;

        _initialize();
    }

    CVertexBufferLayout::CVertexBufferLayout( const std::vector< CVertexBufferElement >& elements )
    {
        m_stride = 0;
        m_elements = elements;

        _initialize();
    }

    void CVertexBufferLayout::_initialize()
    {
        m_stride = 0;
        uint32 _currentOffset = 0;

        for ( size_t i = 0; i < m_elements.size(); i++ )
        {
            m_elements[i].offset = _currentOffset;
            _currentOffset += m_elements[i].size;

            m_stride += m_elements[i].size;
        }
    }

}