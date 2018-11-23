
#include <debug/LDebugSystem.h>

namespace engine { namespace DebugSystem {

    void init()
    {
        LDebugDrawer::GetInstance();
    }

    void drawLine( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        LDebugDrawer::GetInstance()->drawLine( start, end, color );
    }

    void drawArrow( const LVec3& start, const LVec3& end, const LVec3& color )
    {
        LDebugDrawer::GetInstance()->drawArrow( start, end, color );
    }

    void drawClipVolume( const LMat4& clipMatrix, const LVec3& color )
    {
        LDebugDrawer::GetInstance()->drawClipVolume( clipMatrix, color );
    }

    void drawTrailPoints( const std::vector< LVec3 >& trailpoints, const LVec3& color )
    {
        LDebugDrawer::GetInstance()->drawTrailPoints( trailpoints, color );
    }

    void drawLinesBatch( const std::vector< LLine >& linesBatch, const LVec3& color )
    {
        LDebugDrawer::GetInstance()->drawLinesBatch( linesBatch, color );
    }

    void renderView( const LVec2& pos, const LVec2& size, GLuint textureID )
    {

    }

    void setupMatrices( const LMat4& viewMatrix, const LMat4& projectionMatrix )
    {
        LDebugDrawer::GetInstance()->setupMatrices( viewMatrix, projectionMatrix );
    }

    void render()
    {
        LDebugDrawer::GetInstance()->render();
    }

    void release()
    {
        LDebugDrawer::Release();
    }

}}