
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <shaders/LShaderManager.h>
#include <shaders/LShaderDebug3d.h>
#include <LVertexBuffer.h>
#include <LVertexArray.h>

using namespace std;

#define DEBUG_DRAWER_DEFAULT_COLOR CVec3( 0, 0, 1 )

#define DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE 1024

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

    class LDebugDrawer
    {

        private :

        vector< LDLinePositions > m_linesRenderBufferPositions;
        vector< LDLinePositionsColors > m_linesRenderBufferColors;

        vector< LDLinePositions > m_linesPositions;
        vector< LDLinePositionsColors > m_linesColors;

        LVertexBuffer* m_linesPositionsVBO;
        LVertexBuffer* m_linesColorsVBO;
        LVertexArray* m_linesVAO;

        LShaderDebug3d* m_shaderLinesRef;

        CMat4 m_viewMat;
        CMat4 m_projMat;

        void _renderLinesBatch( int countLines );
        void _renderLines();

        static LDebugDrawer* _INSTANCE;
        LDebugDrawer();

        public :

        static LDebugDrawer* GetInstance();
        static void Release();
        ~LDebugDrawer();


        void drawLine( const CVec3& start, const CVec3& end, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawArrow( const CVec3& start, const CVec3& end, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawClipVolume( const CMat4& clipMatrix, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawTrailPoints( const vector< CVec3 >& trailpoints, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawLinesBatch( const vector< CLine >& linesBatch, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );

        void setupMatrices( const CMat4& viewMatrix, const CMat4& projectionMatrix );
        void render();
    };



}

