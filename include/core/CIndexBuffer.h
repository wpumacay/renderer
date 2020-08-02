/// @file CIndexBuffer.h
/// @brief Index Buffer implementation
#pragma once

#include <CCommon.h>

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

        /// Creates an IBO with a given usage, count and index-data
        ///
        /// @param usage Hint for the type of usage for this buffer in GPU memory
        /// @param count Number of indices stored in the buffer
        /// @param data Pointer to the data in CPU to be transferred to GPU memory
        CIndexBuffer( const eBufferUsage& usage,
                      const uint32& count,
                      const uint32* data );

        /// Destroys the current IBO and releases its resources in GPU
        ~CIndexBuffer();

        /// Resizes the GPU-buffer to the required size
        ///
        /// @details
        /// This methods allows to change the size of the IBO on the GPU. Use this method if the
        /// required data to be updated is bigger than the current size of the buffer on the GPU.
        ///
        /// @param count Number of indices that will be contained in the buffer
        void Resize( const uint32& count );

        /// Updated the GPU-memory data of this buffer from given CPU-data
        ///
        /// @details
        /// This method allows to transfers and update memory from CPU to GPU. To better allow
        /// this transfer (it depends on the backend, currently only OpenGL), this IBO must have
        /// been given the DYNAMIC hint during creation.
        ///
        /// @param count How many indices (one per index, or int) will be transferred from CPU to GPU.
        /// @param data Pointer to the memory in CPU to be transferred to GPU.
        void UpdateData( const uint32& count, const uint32* data );

        /// Binds the current IBO such that the subsequent draw calls are executed with its data
        void Bind();

        /// Unbinds the current IBO such that subsequent draw calls won't use its data
        void Unbind();

        /// Returns the string representation for this index-buffer
        std::string ToString() const;

        /// Returns the type of usage hint used to create this buffer
        eBufferUsage usage() const { return m_Usage; }

        /// Returns the number of indices of this IBO in GPU
        uint32 count() const { return m_Count; }

        /// Returns the opengl id of this IBO
        uint32 opengl_id() const { return m_OpenglID; }

    private :

        /// Usage hint used during the creation of this IBO
        eBufferUsage m_Usage;
        /// Number of indices in the buffer
        uint32 m_Count;
        /// OpenGL-id of the linked IBO OpenGL-object
        uint32 m_OpenglID;

        ADD_CLASS_SMART_POINTERS(CIndexBuffer);
    };
}