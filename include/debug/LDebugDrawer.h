
#pragma once

#include <LCommon.h>
#include <shaders/LShaderManager.h>
#include <shaders/LShaderDebug3d.h>
#include <LVertexBuffer.h>
#include <LVertexArray.h>

using namespace std;

#define DEBUG_DRAWER_DEFAULT_COLOR LVec3( 0, 0, 1 )

#define DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE 1024

namespace engine
{
    struct LDLinePositions
    {
        LVec3 vStart;
        LVec3 vEnd;
    };

    struct LDLineColors
    {
        LVec3 cStart;
        LVec3 cEnd;
    };

    class LDebugDrawer
    {

        private :

        vector< LDLinePositions > m_linesRenderBufferPositions;
        vector< LDLineColors > m_linesRenderBufferColors;

        vector< LDLinePositions > m_linesPositions;
        vector< LDLineColors > m_linesColors;

        LVertexBuffer* m_linesPositionsVBO;
        LVertexBuffer* m_linesColorsVBO;
        LVertexArray* m_linesVAO;

        LShaderDebug3d* m_shaderLinesRef;

        glm::mat4 m_viewMat;
        glm::mat4 m_projMat;

        void _renderLinesBatch( int countLines );
        void _renderLines();

        LDebugDrawer();

        public :

        static LDebugDrawer* INSTANCE;
        static void create();
        static void release();
        ~LDebugDrawer();


        void drawLine( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawArrow( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawClipVolume( const glm::mat4& clipMatrix, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
        void drawTrailPoints( const vector< LVec3 >& trailpoints, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );

        void setupMatrices( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );
        void render();
    };



}

