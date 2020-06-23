
#include <core/CVertexBufferLayout.h>

namespace engine
{
    std::string ToString( const eElementType& type )
    {
        /**/ if ( type == eElementType::Float )  return "Float";
        else if ( type == eElementType::Float2 ) return "Float2";
        else if ( type == eElementType::Float3 ) return "Float3";
        else if ( type == eElementType::Float4 ) return "Float4";
        else if ( type == eElementType::Int )    return "Int";
        else if ( type == eElementType::Int2 )   return "Int2";
        else if ( type == eElementType::Int3 )   return "Int3";
        else if ( type == eElementType::Int4 )   return "Int4";

        ENGINE_CORE_CRITICAL( "ToString >>> Invalid eElementType enum given" );
        return "undefined";
    }

    uint32 ToOpenGLType( const eElementType& type )
    {
        /**/ if ( type == eElementType::Float )  return GL_FLOAT;
        else if ( type == eElementType::Float2 ) return GL_FLOAT;
        else if ( type == eElementType::Float3 ) return GL_FLOAT;
        else if ( type == eElementType::Float4 ) return GL_FLOAT;
        else if ( type == eElementType::Int )    return GL_INT;
        else if ( type == eElementType::Int2 )   return GL_INT;
        else if ( type == eElementType::Int3 )   return GL_INT;
        else if ( type == eElementType::Int4 )   return GL_INT;

        ENGINE_CORE_CRITICAL( "ToOpenGLType >>> Invalid eElementType enum given" );
        return 0;
    }

    uint32 SizeOfElement( const eElementType& type )
    {
        /**/ if ( type == eElementType::Float )  return 4;
        else if ( type == eElementType::Float2 ) return 4 * 2;
        else if ( type == eElementType::Float3 ) return 4 * 3;
        else if ( type == eElementType::Float4 ) return 4 * 4;
        else if ( type == eElementType::Int )    return 4;
        else if ( type == eElementType::Int2 )   return 4 * 2;
        else if ( type == eElementType::Int3 )   return 4 * 3;
        else if ( type == eElementType::Int4 )   return 4 * 4;

        ENGINE_CORE_CRITICAL( "SizeOfElement >>> Invalid eElementType enum given" );
        return 0;
    }

    uint32 CountOfElement( const eElementType& type )
    {
        /**/ if ( type == eElementType::Float )  return 1;
        else if ( type == eElementType::Float2 ) return 2;
        else if ( type == eElementType::Float3 ) return 3;
        else if ( type == eElementType::Float4 ) return 4;
        else if ( type == eElementType::Int )    return 1;
        else if ( type == eElementType::Int2 )   return 2;
        else if ( type == eElementType::Int3 )   return 3;
        else if ( type == eElementType::Int4 )   return 4;

        ENGINE_CORE_CRITICAL( "CountOfElement >>> Invalid eElementType enum given" );
        return 0;
    }

    /**************************************************************************
    *                         CVertexBufferElement impl.                      *
    ***************************************************************************/

    CVertexBufferElement::CVertexBufferElement()
    {
        this->name          = "none";
        this->type          = eElementType::Float;
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
        this->count         = CountOfElement( type );
        this->size          = SizeOfElement( type );
        this->offset        = 0; // written by layout when defined
        this->normalized    = normalized;
    }

    /**************************************************************************
    *                         CVertexBufferLayout impl.                       *
    ***************************************************************************/

    CVertexBufferLayout::CVertexBufferLayout()
    {
        m_Stride = 0;
    }

    CVertexBufferLayout::CVertexBufferLayout( const std::initializer_list< CVertexBufferElement >& elements )
    {
        m_Stride = 0;
        m_Elements = elements;
        _Initialize();
    }

    CVertexBufferLayout::CVertexBufferLayout( const std::vector< CVertexBufferElement >& elements )
    {
        m_Stride = 0;
        m_Elements = elements;
        _Initialize();
    }

    void CVertexBufferLayout::_Initialize()
    {
        m_Stride = 0;
        uint32 current_offset = 0;
        for ( size_t i = 0; i < m_Elements.size(); i++ )
        {
            m_Elements[i].offset = current_offset;
            current_offset += m_Elements[i].size;

            m_Stride += m_Elements[i].size;
        }
    }
}