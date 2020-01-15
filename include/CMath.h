#pragma once

#include <CCommon.h>

// tinymath-library
#include <vector_t.h>
#include <matrix_t.h>
#include <transforms.h>

namespace engine
{
    // redefine the vector types to use
    typedef tinymath::Vector<float32, 2 > CVec2;
    typedef tinymath::Vector<float32, 3 > CVec3;
    typedef tinymath::Vector<float32, 4 > CVec4;

    // redefine the matrix types to use
    typedef tinymath::Matrix<float32, 2 > CMat2;
    typedef tinymath::Matrix<float32, 3 > CMat3;
    typedef tinymath::Matrix<float32, 4 > CMat4;

    /***********************************************************************************************
    *                                   Vec and Mat helpers                                        *
    ***********************************************************************************************/

    bool operator==( const CVec3& v1, const CVec3& v2 );
    bool operator!=( const CVec3& v1, const CVec3& v2 );
    CVec3 operator- ( const CVec3& v );

    // define our own to-string methods (mostly to avoid replacing current engine::toString usage)
    std::string toString( const CVec2& v );
    std::string toString( const CVec3& v );
    std::string toString( const CVec4& v );
    std::string toString( const CMat2& m );
    std::string toString( const CMat3& m );
    std::string toString( const CMat4& m );

    /***********************************************************************************************
    *                                         CLine                                                *
    ************************************************************************************************/

    struct CLine
    {
        CVec3 start;
        CVec3 end;

        CLine();
        CLine( const CVec3& start, const CVec3& end );
    };

    std::string toString( const CLine& line );

    /***********************************************************************************************
    *                                      Matrix4x4 helpers                                       *
    ***********************************************************************************************/

    CMat4 perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar );
    CMat4 ortho( float32 width, float32 height, float32 zNear, float32 zFar );
    CMat4 lookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp );
    CMat4 scale( const CVec3& v );
    CMat4 rotate( const CMat3& m );
    CMat4 translation( const CVec3& v );

    /***********************************************************************************************
    *                                             CInd3                                            *
    ***********************************************************************************************/

    struct CInd3
    {
        GLint buff[3];

        CInd3();
        CInd3( GLint v1, GLint v2, GLint v3 );
    };

    std::string toString( const CInd3& ind3 );

    /***********************************************************************************************
    *                                       Geometric helpers                                      *
    ***********************************************************************************************/

    void computeFrameAxes( const CVec3& axis1, CVec3& axis2, CVec3& axis3, const CVec3& worldUp );

    struct CPlane
    {
        CVec3 normal;
        CVec3 position;
    };

    float32 signedDistToPlane( const CVec3& point, const CPlane& plane );
    float32 distToPlane( const CVec3& point, const CPlane& plane );
    CVec3 projInPlane( const CVec3& point, const CPlane& plane );

    struct CBoundingBox
    {
        CVec3 size;
        CMat4 worldTransform;
    };

    std::array< CVec3, 8 > computeBoxCorners( const CBoundingBox& bbox );

    struct CBoundingSphere
    {
        float32 radius;
        CVec3 worldPosition;
    };

    struct CFrustum
    {
        std::array< CVec3, 8 > corners;
        std::array< CPlane, 6 > planes;

        CFrustum( const CMat4& viewProjMat );
    };

    bool certainlyOutsideFrustum( const CFrustum& frustum, const CBoundingBox& bbox );
    bool certainlyOutsideFrustum( const CFrustum& frustum, const CBoundingSphere& bsphere );

    struct CComparatorSignedDistancePlane
    {
        CPlane plane;

        bool operator() ( const CVec3& p1, const CVec3& p2 ) 
        { 
            return signedDistToPlane( p1, plane ) < signedDistToPlane( p2, plane );
        }
    };

    struct CComparatorX 
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.x() < v2.x(); }
    };

    struct CComparatorY
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.y() < v2.y(); }
    };

    struct CComparatorZ
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.z() < v2.z(); }
    };

    struct CComparatorClosestToPoint
    {
        CVec3 point;

        bool operator() ( const CVec3& p1, const CVec3& p2 )
        {
            return ( p1 - point ).length() < ( p2 - point ).length();
        }
    };

    struct CComparatorFarthestFromPoint
    {
        CVec3 point;

        bool operator() ( const CVec3& p1, const CVec3& p2 )
        {
            return ( p1 - point ).length() > ( p2 - point ).length();
        }
    };

    void computeMinMaxVertexToPlane( const CPlane& plane, const CBoundingBox& bbox, CVec3& minVertex, CVec3& maxVertex );

    /***********************************************************************************************
    *                                        Extra helpers                                         *
    ***********************************************************************************************/

    float32 toRadians( float32 angle );
    float32 toDegrees( float32 angle );

}
