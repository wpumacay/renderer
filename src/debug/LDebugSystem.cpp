
#include <debug/LDebugSystem.h>

namespace engine { namespace DebugSystem {

    void init()
    {
        LDebugDrawer::GetInstance();
    }

    void drawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawLine( start, end, color );
    }

    void drawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawArrow( start, end, color );
    }

    void drawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawClipVolume( clipMatrix, color );
    }

    void drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawTrailPoints( trailpoints, color );
    }

    void drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawLinesBatch( linesBatch, color );
    }

    void drawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& aabbWorldTransform, const CVec3& color )
    {
        LDebugDrawer::GetInstance()->drawAABB( aabbMin, aabbMax, aabbWorldTransform, color );
    }

    void renderView( const CVec2& pos, const CVec2& size, GLuint textureID )
    {

    }

    void setupMatrices( const CMat4& viewMatrix, const CMat4& projectionMatrix )
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