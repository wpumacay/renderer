
#include <debug/LDebugSystem.h>

namespace engine { namespace DebugSystem {

    void init()
    {
        LDebugDrawer::create();
    }

    void drawLine( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        LDebugDrawer::INSTANCE->drawLine( start, end, color );
    }

    void drawArrow( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        LDebugDrawer::INSTANCE->drawArrow( start, end, color );
    }

    void drawClipVolume( const glm::mat4& clipMatrix, const LVec3& color )
    {
        LDebugDrawer::INSTANCE->drawClipVolume( clipMatrix, color );
    }

    void drawTrailPoints( const vector< LVec3 >& trailpoints, const LVec3& color )
    {
        LDebugDrawer::INSTANCE->drawTrailPoints( trailpoints, color );
    }

    void renderView( const LVec2& pos, const LVec2& size, GLuint textureID )
    {

    }

    void setupMatrices( glm::mat4 viewMatrix, glm::mat4 projectionMatrix )
    {
        LDebugDrawer::INSTANCE->setupMatrices( viewMatrix, projectionMatrix );
    }

    void render()
    {
        LDebugDrawer::INSTANCE->render();
    }

    void release()
    {
        LDebugDrawer::release();
    }

}}