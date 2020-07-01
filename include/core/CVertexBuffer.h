#pragma once

#include <CCommon.h>
#include <core/CVertexBufferLayout.h>

namespace engine
{
    /// Vertex Buffer Object (VBO) abstraction class, used to store data vertex-data on the GPU
    ///
    /// @details
    /// Defines the functionality of a Vertex Buffer Object, which is used to hold vertex-data
    /// on the GPU that can be used during render calls, such as vertex positions, colors, etc.
    class CVertexBuffer
    {
    public :

        /// Creates a VBO with a given layout, usage and initialized data
        ///
        /// @param layout     Layout of the data to be stored in GPU memory
        /// @param usage      Hint for the type of usage for this buffer in GPU memory
        /// @param size       Size of the buffer (in bytes) in GPU memory
        /// @param data       Pointer to the data in CPU to be transferred to GPU memory
        CVertexBuffer( const CVertexBufferLayout& layout, 
                       const eBufferUsage& usage, 
                       const uint32& size, 
                       const float32* data );

        /// Destroys the current VBO and releases its resources in GPU
        ~CVertexBuffer();

        /// Resizes the GPU-buffer to the required size
        ///
        /// @details
        /// This methods allows to change the size of the VBO on the GPU. Use this method if the
        /// required data to be updated is bigger than the current size of the buffer on the GPU.
        ///
        /// @param size New size of the buffer on the GPU
        void Resize( const uint32& size );

        /// Updated the GPU-memory data of this buffer from given CPU-data
        ///
        /// @details
        /// This method allows to transfers and update memory from CPU to GPU. To better allow
        /// this transfer (it depends on the backend, currently only OpenGL), this VBO must have
        /// been given the DYNAMIC hint during creation.
        ///
        /// @param size     How much data (in bytes) will be transferred from CPU to GPU.
        /// @param data     Pointer to the memory in CPU to be transferred to GPU.
        void UpdateData( const uint32& size, const float32* data );

        /// Binds the current VBO such that subsequent draw calls are executed with its data
        void Bind();

        /// Unbinds the current VBO such that subsequent draw calls won't use its data
        void Unbind();

        /// Returns the layout representation of this VBO
        CVertexBufferLayout layout() const { return m_Layout; }

        /// Returns the size of memory (in bytes) of this VBO in GPU
        uint32 size() const { return m_Size; }

        /// Returns the type of usage hint used to create this buffer
        eBufferUsage usage() const { return m_Usage; }

        /// Returns the opengl-id of this VBO
        uint32 opengl_id() const { return m_OpenglID; }

    private :

        /// Object representing how memory is laid out
        CVertexBufferLayout m_Layout;
        /// Usage hint used during the creation of this VBO
        eBufferUsage m_Usage;
        /// Size of the buffer of memory in GPU (in bytes)
        uint32 m_Size;
        /// OpenGL-id of the linked VBO OpenGL-object
        uint32 m_OpenglID;

        ADD_CLASS_SMART_POINTERS(CVertexBuffer);
    };
}