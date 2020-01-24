#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBufferLayout.h>

namespace engine
{

    /** @brief Type of usage for a buffer object */
    enum class eBufferUsage
    {
        /** Buffer holds memory that won't be changed by the user after its creation. Generally this
            type of buffer is allocated on GPU such that it can read it quickly. */
        STATIC = 0,
        /** Buffer holds memory that can be changed by the user on the fly. This type of buffer is
            allocated on GPU such that it allows write operations from the CPU, so its slower compared
            to the STATIC type of buffer. */
        DYNAMIC
    };

    /**
    *   @brief Returns a string representation of the given BufferUsage enum
    *
    *   @param usage    Usage type of a given buffer
    *   @return         String representation of this usage type
    */
    std::string toString( const eBufferUsage& usage );

    /**
    *   @brief Converts the given buffer usage enum to its related OpenGL enum
    *
    *   @param usage    Usage type of a given buffer
    *   @return         OpenGL enum related to this usage
    */
    uint32 toOpenGLEnum( const eBufferUsage& usage );

    /**
    *   @brief VBO (Vertex Buffer Object) abstraction class
    *
    *   @details
    *   Defines the functionality of a Vertex Buffer Object, which is used to hold vertex-data
    *   on the GPU that can be used during render calls, such as vertex positions, colors, etc.
    */
    class CVertexBuffer
    {

    public :

        /**
        *   @brief Creates a VBO from some given configuration data
        *
        *   @param bufferLayout     Layout of the data to be stored in GPU memory.
        *   @param bufferUsage      Hint for the type of usage for this buffer in GPU memory.
        *   @param bufferSize       Size of the buffer (in bytes) in GPU memory.
        *   @param bufferData       Pointer to the data in CPU to be transferred to GPU memory.
        *   @param track            Debug flag used to keep track of allocations and deallocations.
        */
        CVertexBuffer( const CVertexBufferLayout& bufferLayout, 
                       const eBufferUsage& bufferUsage, 
                       uint32 bufferSize, 
                       float32* bufferData,
                       bool track = false );

        /** @brief Destroys the current VBO and releases its resources in GPU */
        ~CVertexBuffer();

        /**
        *   @brief Updated the GPU-memory data of this buffer from given CPU-data
        *
        *   @details
        *   This method allows to transfers and update memory from CPU to GPU. To better allow
        *   this transfer (it depends on the backend, currently only OpenGL), this VBO must have
        *   been given the DYNAMIC hint during creation.
        *
        *   @param bufferSize   How much data (in bytes) will be transferred from CPU to GPU.
        *   @param bufferData   Pointer to the memory in CPU to be transferred to GPU.
        */
        void updateData( uint32 bufferSize, float32* bufferData );

        /**
        *   @brief Binds the current VBO such that subsequent draw calls are executed with its data
        *
        *   Example:
        *   @code
        *   // bind the VBO
        *   vboQuad->bind();
        *   // execute some draw-calls (vertex-data used is from the currently bound VBO)
        *   glDrawArray( GL_TRIANGLES, 0, 6 );
        *   @endcode
        */
        void bind();

        /** @brief Unbinds the current VBO such that subsequent draw calls won't use its data */
        void unbind();

        /** @brief Returns the layout representation of this VBO */
        CVertexBufferLayout layout() const { return m_bufferLayout; }

        /** @brief Returns the size of memory (in bytes) of this VBO in GPU */
        uint32 size() const { return m_bufferSize; }

        /** @brief Returns the type of usage hint used to create this buffer */
        eBufferUsage usage() const { return m_bufferUsage; }

        /** @brief Returns the opengl-id of this VBO */
        uint32 openglId() const { return m_openglId; }

    private :

        /** @brief Object representing how memory is laid out */
        CVertexBufferLayout m_bufferLayout;

        /** @brief Usage hint used during the creation of this VBO */
        eBufferUsage m_bufferUsage;

        /** @brief Size of the buffer of memory in GPU (in bytes) */
        uint32 m_bufferSize;

        /** @brief OpenGL-id of the linked VBO OpenGL-object */
        uint32 m_openglId;

        /** @brief Debug flag used for tracking allocations and deallocations */
        bool m_track;

    };

}