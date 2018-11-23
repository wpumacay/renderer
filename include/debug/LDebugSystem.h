
#pragma once

#include <LCommon.h>
#include <debug/LDebugDrawer.h>
#include <debug/LDebugViewer.h>

namespace engine { namespace DebugSystem {

    void init();

    // drawer calls

    void drawLine( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawArrow( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawClipVolume( const LMat4& clipMatrix, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawTrailPoints( const std::vector< LVec3 >& trailpoints, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawLinesBatch( const std::vector< LLine >& linesBatch, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );

    // viewer calls

    void renderView( const LVec2& pos, const LVec2& size, GLuint textureID );

    void setupMatrices( const LMat4& viewMatrix, const LMat4& projectionMatrix );
    void render();

    void release();
}}