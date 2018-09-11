
#pragma once

#include <LCommon.h>
#include <debug/LDebugDrawer.h>
#include <debug/LDebugViewer.h>

namespace engine { namespace DebugSystem {

    void init();

    // drawer calls

    void drawLine( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawArrow( const LVec3& start, const LVec3& end, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawClipVolume( const glm::mat4& clipMatrix, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );
    void drawTrailPoints( const vector< LVec3 >& trailpoints, const LVec3& color = DEBUG_DRAWER_DEFAULT_COLOR );

    // viewer calls

    void renderView( const LVec2& pos, const LVec2& size, GLuint textureID );

    void setupMatrices( glm::mat4 viewMatrix, glm::mat4 projectionMatrix );
    void render();

    void release();
}}