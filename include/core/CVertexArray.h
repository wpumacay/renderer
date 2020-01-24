#pragma once

#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>

namespace engine
{

    /**
    *   @brief VAO (Vertex Array Object) abstraction class
    *
    *   @details
    *   Defines the functionality of a Vertex Array Object, which can hold the
    *   information from VBOs and EBOs in a group that can be either bind or
    *   unbind with just one single call. Ownership of the related VBOs and EBOs
    *   belongs to this object once these transferred to this VAO.
    *
    */
    class CVertexArray
    {

    public :

        /**
        *   @brief Creates a Vertex Array Object and initializes its OpenGL resources.
        *
        *   @param track    Whether or not to keep track of the allocations of this object
        */
        CVertexArray( bool track = false );

        /**
        *   @brief Destroys this VAO and releases the owned OpenGL resources, as well as owned VBOs and EBOs
        */
        ~CVertexArray();

        /**
        *   @brief Adds the given VBO to the group defined by this VAO, and transfers its ownership
        *
        *   @param vertexBuffer VBO being transferred to this group
        *   @param isInstanced  Whether or not to create instancing divisors
        *
        *   Example:
        *   @code
        *       // create some VBOs
        *       auto vboPosition = ...;
        *       auto vboColor = ...;
        *       // move these VBOs to this VAO
        *       vao->addVertexBuffer( std::move( vboPosition ) );
        *       vao->addVertexBuffer( std::move( vboColor ) );
        *   @endcode
        */
        void addVertexBuffer( std::unique_ptr<CVertexBuffer> vertexBuffer, bool isInstanced = false );

        /**
        *   @brief Adds the given EBO to the group defined by this VAO, and transfers its ownership
        *
        *   @param indexBuffer  EBO being transferred to this group
        */
        void setIndexBuffer( std::unique_ptr<CIndexBuffer> indexBuffer );

        /**
        *   @brief Binds the current VAO, setting the gl-fsm to the appropriate state
        *
        *   Example:
        *   @code
        *       // bind current VAO (binds current VBOs, EBO to the gl state machine)
        *       vao->bind();
        *       // draw something using the current buffer bound to the gl-fsm
        *       glDrawElements( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, NULL );
        *   @endcode
        */
        void bind();

        /**
        *   @brief Unbinds the current VAO, setting the gl-fsm to the 0 state
        *
        *   Example:
        *   @code
        *       // draw something using the current buffer bound to the gl-fsm
        *       glDrawElements( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, NULL );
        *       // unbind the VBOs and EBO from the fsm (to let other VAOs work afterwards)
        *       vao->unbind();
        *   @endcode
        */
        void unbind();

        /**
        *   @brief Returns a mutable reference to the currently owned VBOs
        *
        *   @return     Mutable-reference to the container (std::vector) storing the owned VBOs
        */
        std::vector< std::unique_ptr<CVertexBuffer> >& vertexBuffers() { return m_vertexBuffers; }

        /**
        *   @brief Returns a immutable (const) reference to the currently owned VBOs
        *
        *   @return     Const-reference to the container (std::vector) storing the owned VBOs
        */
        const std::vector< std::unique_ptr<CVertexBuffer> >& vertexBuffers() const { return m_vertexBuffers; }

        /**
        *   @brief Returns a mutable reference to the currently owned EBO
        *
        *   @return     Mutable-reference to the owned EBO
        */
        std::unique_ptr<CIndexBuffer>& indexBuffer() { return m_indexBuffer; }

        /**
        *   @brief Returns a immutable (const) reference to the currently owned EBO
        *
        *   @return     Const-reference to the owned EBO
        */
        const std::unique_ptr<CIndexBuffer>& indexBuffer() const { return m_indexBuffer; }

        /**
        *   @brief Returns the uint32 identifier of the OpenGL object related to this VAO
        *
        *   @return     Identifier of this VAO inside OpenGL
        */
        uint32 openglId() const { return m_openglId; }

        /**
        *   @brief Returns the current number of attributes in the VAO
        *
        *   @return     Integer representing the current number of attribs in the VAO
        */
        uint32 numAttribs() const { return m_numAttribIndx; }

    private :

        /** @brief Container for the owner Vertex Buffer Objects */
        std::vector< std::unique_ptr<CVertexBuffer> > m_vertexBuffers;

        /** @brief Owned Element Buffer Object */
        std::unique_ptr<CIndexBuffer> m_indexBuffer;

        /** @brief Index of the current available slot in the attributePointer list of the VAO */
        uint32 m_numAttribIndx;

        /** @brief Identifier of this VAO in OpenGL */
        uint32 m_openglId;

        bool m_track;
    };

}