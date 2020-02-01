/**
*   @file CVertexBufferLayout.h
*   @brief Vertex Buffer layout implementation and helper methods|enums
*/

#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    /// Type of element used for part (of all) elements in a Vertex Buffer
    enum class eElementType
    {
        Undefined,  //!< None type (undefined) for error checking
        Float,      //!< Single float, size 4-bytes
        Float2,     //!< Two float compound (vec2), size 8-bytes
        Float3,     //!< Three float compound (vec3), size 12-bytes
        Float4,     //!< Four float compound (vec4), size 16-bytes
        Int,        //!< Single integer, size 4-bytes (=int32)
        Int2,       //!< Two integer compound (int2), size 8-bytes
        Int3,       //!< Three integer compound (int3), size 12-bytes
        Int4,       //!< Four integer compound (int4), size 16-bytes
    };

    /// @brief Returns the string representation of the given element-type enum
    std::string toString( const eElementType& type );

    /**
    *   @brief Converts the given element-type to its corresponding OpenGL enum
    *
    *   @param type     Element-type enum of a given element in a buffer
    *   @return         ÓpenGL enum related to this element-type
    */
    uint32 toOpenGLType( const eElementType& type );

    /// @brief Returns the number of bytes used by a given Element-type
    uint32 sizeOfElement( const eElementType& type );

    /**
    *   @brief Returns the number of single-components in a given Element-type
    *
    *   @param type     Element-type enum of a vien element in a buffer
    *   @return         Number of single-components in the given element-type, e.g. Float2 has 2-components
    */
    uint32 countOfElement( const eElementType& type );

    /**
    *   @brief Description of a given element in the buffer
    */
    struct CVertexBufferElement
    {
        /// @brief Name of the element in the buffer (for user debugging mainly), e.g. "position"
        std::string name;
        /// @brief Type of the element, e.g. Float3
        eElementType type;
        /// @brief Number of single-components of the element, e.g. Float3 has 3-floats
        uint32 count;
        /// @brief Size of the element (in bytes), e.g. Float3 has size of 12 bytes
        uint32 size;
        /// @brief Offset of the element's position in the buffer (managed by the layout)
        uint32 offset;
        /// @brief Whether or not this element should be normalized, e.g. when using normals as attributes
        bool normalized;

        // Used internally to avoid issues with containers and initializer lists
        CVertexBufferElement();

        /**
        *   @brief Creates a description object of a given element in the buffer
        *
        *   @param name         Name of the element in the buffer (for debugging mainly)
        *   @param type         Type of the element, e.g. eElementType::Float3
        *   @param normalized   Whether or not the elements should be normalized before being used by a shader
        */
        CVertexBufferElement( const std::string& name,
                              const eElementType& type,
                              bool normalized );
    };

    /**
    *   @brief Description of the layout of all (or part) data in a Vertex Buffer
    */
    class CVertexBufferLayout
    {

    public :

        // Used internally to avoid issues with containers
        CVertexBufferLayout();

        /**
        *   @brief Creates the layout of data in a VBO from an initializer-list
        *
        *   Example:
        *   @code
        *   // Create a buffer layout from an initializer list
        *   engine::CVertexBufferLayout layout = { { "pos", engine::eElementType::Float2, false },
        *                                          { "color", engine::eElementType::Float3, false } };
        *   @endcode
        */
        CVertexBufferLayout( const std::initializer_list< CVertexBufferElement >& elements );

        /**
        *   @brief Creates the layout of data in a VBO from an vector container
        *
        *   Example:
        *   @code
        *   // Create a container of buffer elements, and add some entries describing the layout
        *   auto vecElements = std::vector<engine::CVertexBufferElement>();
        *   vecElement.push_back( { "pos", engine::eElementType::Float2, false } );
        *   vecElement.push_back( { "color", engine::eElementType::Float3, false } );
        *   // Create a buffer layout from the container of elements
        *   auto layout = engine::CVertexBufferLayout( vecElements );
        *   @endcode
        */
        CVertexBufferLayout( const std::vector< CVertexBufferElement >& elements );

        /**
        *   @brief Returns the stride (offset) of the group of elements in the VBO
        *
        *   @details
        *   The stride consists of an offset in bytes required to go to the next element. This quantity
        *   is computed from how large (in bytes) is a group of elements in the layout. For example,
        *   if a layout consists of 1 Float2 for Position (xy) and 1 Float3 for Color (rgb), then
        *   the stride would be 5 floats (or 20 bytes), which is the step required in bytes to get
        *   from one element to the next associated element (e.g. have to jump 20 bytes to get from
        *   one position element to the next position element).
        */
        uint32 stride() { return m_stride; }

        /// @brief Returns a mutable reference to the container of the elements in the layout
        std::vector< CVertexBufferElement >& elements() { return m_elements; }

        /// @brief Returns a immutable reference to the container of the elements in the layout
        const std::vector< CVertexBufferElement >& elements() const { return m_elements; }

    private :

        /// @brief Computes internal quantities, like the stride, from the elements given during construction
        void _initialize();

    private :

        /// @brief Offset in bytes required to jump from one to the next "same" element in the memory buffer
        uint32 m_stride;

        /// @brief Container of the elements stored in this layout
        std::vector< CVertexBufferElement > m_elements;
    };

}