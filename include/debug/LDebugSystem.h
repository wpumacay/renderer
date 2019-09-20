
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <debug/LDebugDrawer.h>

namespace engine { namespace DebugSystem {

    void init();

    // drawer calls

    void drawLine( const CVec3& start, const CVec3& end, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawArrow( const CVec3& start, const CVec3& end, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawClipVolume( const CMat4& clipMatrix, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );

    // viewer calls

    void renderView( const CVec2& pos, const CVec2& size, GLuint textureID );

    void setupMatrices( const CMat4& viewMatrix, const CMat4& projectionMatrix );
    void render();

    void release();
}}