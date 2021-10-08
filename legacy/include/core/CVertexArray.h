/// @file CVertexArray.h
/// @brief Vertex Array implementation
#pragma once

#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>

namespace engine
{
    /// Vertex Array Object (VAO) abstraction class, used to handle VBOs and IBOs
    ///
    /// @details
    /// Defines the functionality of a Vertex Array Object, which can hold the
    /// information of VBOs and IBOs in a group that can be either bound or
    /// unbound with just one single call. Ownership of the related VBOs and IBOs
    /// belongs to this object once these are transferred to this VAO.
    class CVertexArray
    {

    public :

        /// Creates a Vertex Array Object and initializes its OpenGL resources
        CVertexArray();

        /// Destroys this VAO and releases the owned OpenGL resources, as well as owned VBOs and IBOs
        ~CVertexArray();

        /// Adds the given VBO to the group defined by this VAO, and transfers its ownership
        ///
        /// @param vertexBuffer VBO being transferred to this group
        /// @param isInstanced Whether or not to create instancing divisors
        void AddVertexBuffer( std::unique_ptr<CVertexBuffer> vertexBuffer, bool isInstanced = false );

        /// Adds the given IBO to the group defined by this VAO, and transfers its ownership
        ///
        /// @param indexBuffer  IBO being transferred to this group
        void SetIndexBuffer( std::unique_ptr<CIndexBuffer> indexBuffer );

        /// Binds the current VAO, setting the gl-fsm to the appropriate state
        void Bind();

        /// Unbinds the current VAO, setting the gl-fsm to the 0 state
        void Unbind();

        /// Returns mutable references to the owned VBOs in this vertex array
        std::vector<CVertexBuffer*> vertex_buffers();

        /// Returns immutable (const) references to the owned VBOs in this vertex array
        std::vector<const CVertexBuffer*> vertex_buffers() const;


        /// Returns a mutable reference to the currently owned IBO
        CIndexBuffer* index_buffer() { return m_IndexBuffer.get(); }

        /// Returns an immutable (const) reference to the currently owned IBO
        const CIndexBuffer* index_buffer() const { return m_IndexBuffer.get(); }

        /// Returns the uint32 identifier of the OpenGL object related to this VAO
        uint32 opengl_id() const { return m_OpenglID; }

        /// Returns the current number of attributes in the VAO
        uint32 num_attribs() const { return m_NumAttribIndx; }

    private :

        /// Container for the owner Vertex Buffer Objects
        std::vector<std::unique_ptr<CVertexBuffer>> m_VertexBuffers;
        /// Owned Element Buffer Object
        std::unique_ptr<CIndexBuffer> m_IndexBuffer = nullptr;
        /// Index of the current available slot in the attributePointer list of the VAO
        uint32 m_NumAttribIndx = 0;
        /// Identifier of this VAO in OpenGL
        uint32 m_OpenglID = 0;

        ADD_CLASS_SMART_POINTERS(CVertexArray);
    };
}
