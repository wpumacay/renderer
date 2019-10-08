
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <shaders/CShaderManager.h>
#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <camera/CICamera.h>
#include <graphics/CMesh.h>

#define DEBUG_DRAWER_BATCH_SIZE 1024

namespace engine
{
    struct LDLinePositions
    {
        CVec3 vStart;
        CVec3 vEnd;
    };

    struct LDLinePositionsColors
    {
        CVec3 cStart;
        CVec3 cEnd;
    };

    class CDebugDrawer
    {

    public :

        static void Init();
        static void Release();
        static void Render( CICamera* camera );
        static void DrawLine( const CVec3& start, const CVec3& end, const CVec3& color );
        static void DrawArrow( const CVec3& start, const CVec3& end, const CVec3& color );
        static void DrawClipVolume( const CMat4& clipMatrix, const CVec3& color );
        static void DrawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color );
        static void DrawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color );
        static void DrawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color );
        static void DrawNormals( CMesh* meshPtr, const CVec3& color );
        static void DrawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size );
        static void DrawFrame( const CMat4& frame, float32 size );

        ~CDebugDrawer();

    private :

        static CDebugDrawer* s_instance;

        CDebugDrawer();

        void _drawLine( const CVec3& start, const CVec3& end, const CVec3& color );
        void _drawArrow( const CVec3& start, const CVec3& end, const CVec3& color );
        void _drawClipVolume( const CMat4& clipMatrix, const CVec3& color );
        void _drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color );
        void _drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color );
        void _drawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color );
        void _drawNormals( CMesh* meshPtr, const CVec3& color );
        void _drawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size );
        void _drawFrame( const CMat4& frame, float32 size );

        void _render( CICamera* camera );
        void _renderLinesBatch( CICamera* camera, int numLines );

        std::vector< LDLinePositions > m_linesRenderBufferPositions;
        std::vector< LDLinePositionsColors > m_linesRenderBufferColors;

        std::vector< LDLinePositions > m_linesPositions;
        std::vector< LDLinePositionsColors > m_linesColors;

        std::unique_ptr< CVertexBuffer >    m_linesPositionsVBO;
        std::unique_ptr< CVertexBuffer >    m_linesColorsVBO;
        std::unique_ptr< CVertexArray >     m_linesVAO;

        CShader* m_shaderPtr;

    };

}
