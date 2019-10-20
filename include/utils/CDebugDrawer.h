
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <shaders/CShaderManager.h>
#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <camera/CICamera.h>
#include <lights/CILight.h>
#include <graphics/CMesh.h>
#include <graphics/CMeshBuilder.h>

#define DEBUG_DRAWER_BATCH_SIZE 1024
#define DEBUG_DRAWER_SPHERE_DIVISIONS 20

namespace engine
{
    struct CLinePositions
    {
        CVec3 vStart;
        CVec3 vEnd;
    };

    struct CLineColors
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
        static void Render( CICamera* camera, CILight* light );
        static void DrawLine( const CVec3& start, const CVec3& end, const CVec3& color );
        static void DrawBox( const CVec3& size, const CMat4& transform, const CVec3& color );
        static void DrawSphere( float32 radius, const CMat4& transform );
        static void DrawSphere( float32 radius, const CMat4& transform, const CVec3& color );
        static void DrawArrow( const CVec3& start, const CVec3& end, const CVec3& color );
        static void DrawClipVolume( const CMat4& clipMatrix, const CVec3& color );
        static void DrawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color );
        static void DrawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color );
        static void DrawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color );
        static void DrawNormals( CMesh* meshPtr, const CVec3& color );
        static void DrawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size );
        static void DrawFrame( const CMat4& frame, float32 size );
        static void DrawPlane( const CPlane& plane, const CVec2& size, const CVec3& color );

        static void DrawSolidBox( const CVec3& size, const CMat4& transform, const CVec4& color );

        ~CDebugDrawer();

    private :

        static CDebugDrawer* s_instance;

        CDebugDrawer();

        void _drawLine( const CVec3& start, const CVec3& end, const CVec3& color );
        void _drawArrow( const CVec3& start, const CVec3& end, const CVec3& color );
        void _drawBox( const CVec3& size, const CMat4& transform, const CVec3& color );
        void _drawSphere( float32 radius, const CMat4& transform, const CVec3& colorXY, const CVec3& colorYZ, const CVec3& colorXZ );
        void _drawClipVolume( const CMat4& clipMatrix, const CVec3& color );
        void _drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color );
        void _drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color );
        void _drawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color );
        void _drawNormals( CMesh* meshPtr, const CVec3& color );
        void _drawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size );
        void _drawFrame( const CMat4& frame, float32 size );
        void _drawPlane( const CPlane& plane, const CVec2& size, const CVec3& color );

        void _drawSolidBox( const CVec3& size, const CMat4& transform, const CVec4& color );

        void _render( CICamera* camera );
        void _render( CICamera* camera, CILight* light );

        void _renderLines( CICamera* camera );
        void _renderBatchOfLines( int numLines );

        void _renderSolidBoxes( CICamera* camera );
        void _renderSolidBoxes( CICamera* camera, CILight* light );
        void _renderBatchOfSolidBoxes( int numBoxes, bool updateNormals );

        std::vector< CLinePositions > m_linesRenderBufferPositions;
        std::vector< CLineColors > m_linesRenderBufferColors;

        std::vector< CLinePositions > m_linesPositions;
        std::vector< CLineColors > m_linesColors;

        std::unique_ptr< CVertexBuffer >    m_linesPositionsVBO;
        std::unique_ptr< CVertexBuffer >    m_linesColorsVBO;
        std::unique_ptr< CVertexArray >     m_linesVAO;

        CShader* m_shaderLinesPtr;

        /* resources required to render solid objects *******************************/

        /// shaders used for rendering solid objects
        CShader* m_shaderSolidLightingPtr;
        CShader* m_shaderSolidNoLightingPtr;

        // solid cubes resources (each aligned in its own buffer to copy without much extra work)
        std::array< CMat4, DEBUG_DRAWER_BATCH_SIZE > m_renderBufferCubesModelMats;
        std::array< CMat4, DEBUG_DRAWER_BATCH_SIZE > m_renderBufferCubesNormalMats;
        std::array< CVec4, DEBUG_DRAWER_BATCH_SIZE > m_renderBufferCubesColors;
        std::vector< CMat4 > m_cubesModelMats;
        std::vector< CMat4 > m_cubesNormalMats;
        std::vector< CVec4 > m_cubesColors;

        std::unique_ptr< CVertexArray > m_cubeVAO;
        std::unique_ptr< CIndexBuffer > m_cubeIBO;
        std::unique_ptr< CVertexBuffer > m_cubeVBOpositions;
        std::unique_ptr< CVertexBuffer > m_cubeVBOnormals;
        std::unique_ptr< CVertexBuffer > m_cubeVBOinstancesColors;
        std::unique_ptr< CVertexBuffer > m_cubeVBOinstancesModelMats;
        std::unique_ptr< CVertexBuffer > m_cubeVBOinstancesNormalMats;

        /****************************************************************************/
    };

}
