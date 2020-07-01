#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>

namespace engine
{
    /// Index Buffer Object (IBO|EBO) abstraction class, used to store index-data on the GPU
    ///
    /// @details
    /// Defines the functionality of an Index Buffer Object, which is used to hold index-data
    /// (face indices) on the GPU that can be used during render calls
    class CIndexBuffer
    {
    public :

        ///   @brief Creates an IBO from some given configuration data
        ///
        ///   @param bufferUsage  Hint for the type of usage for this buffer in GPU memory
        ///   @param bufferCount  Number of indices stored in the buffer
        ///   @param bufferData   Pointer to the data in CPU to be transferred to GPU memory
        ///   @param track        Debug flag used to keep track of allocations and deallocations.
        CIndexBuffer( const eBufferUsage& bufferUsage,
                      uint32 bufferCount,
                      uint32* bufferData );

        /** @brief Destroys the current IBO and releases its resources in GPU */
        ~CIndexBuffer();

        /**
        *   @brief Resizes the GPU-buffer to the required size
        *
        *   @details
        *   This methods allows to change the size of the IBO on the GPU. Use this method if the
        *   required data to be updated is bigger than the current size of the buffer on the GPU.
        *
        *   @param bufferCount  Number of indices that will be contained in the buffer
        */
        void resize( uint32 bufferCount );

        /**
        *   @brief Updated the GPU-memory data of this buffer from given CPU-data
        *
        *   @details
        *   This method allows to transfers and update memory from CPU to GPU. To better allow
        *   this transfer (it depends on the backend, currently only OpenGL), this IBO must have
        *   been given the DYNAMIC hint during creation.
        *
        *   @param bufferCount  How many indices (one per index, or int) will be transferred from CPU to GPU.
        *   @param bufferData   Pointer to the memory in CPU to be transferred to GPU.
        */
        void updateData( uint32 bufferCount, uint32* bufferData );

        /**
        *   @brief Binds the current IBO such that the subsequent draw calls are executed with its data
        *
        *   Example:
        *   @code
        *   // bind the IBO
        *   ibo->bind();
        *   // execute some draw-calls (index-data for faces comes from the currently bound IBO)
        *   glDrawElements( GL_TRIANGLES, ibo->cound(), GL_UNSIGNED_INT, NULL );
        *   @endcode
        */
        void bind();

        /** @brief Unbinds the current IBO such that subsequent draw calls won't use its data */
        void unbind();

        /** @brief Returns the type of usage hint used to create this buffer */
        eBufferUsage usage() const { return m_bufferUsage; }

        /** @brief Returns the number of indices of this IBO in GPU */
        uint32 count() const { return m_bufferCount; }

        /** @brief Returns the opengl id of this IBO */
        uint32 openglId() const { return m_openglId; }

    private :

        /** @brief Usage hint used during the creation of this IBO */
        eBufferUsage m_bufferUsage;

        /** @brief Number of indices in the buffer */
        uint32 m_bufferCount;

        /** @brief OpenGL-id of the linked IBO OpenGL-object */
        uint32 m_openglId;
    };

}