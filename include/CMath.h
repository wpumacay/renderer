#pragma once

#include <CCommon.h>

namespace engine
{
    struct CVec4;
    struct CVec3;
    struct CVec2;

    /**************************************************************************
    *                                CVec2                                    *
    ***************************************************************************/

    struct CVec2
    {
        float32 x;
        float32 y;

        CVec2();
        CVec2( float32 x, float32 y );

        void normalize();
        void scale( float32 sx, float32 sy );
        void scale( const CVec2& vscale );
        float32 length();

        static CVec2 normalize( const CVec2& v );
        static CVec2 scale( const CVec2& v, float32 val );
        static CVec2 scale( const CVec2& v, const CVec2& vals );
        static float32 dot( const CVec2& v1, const CVec2& v2 );
        static bool equal( const CVec2& v1, const CVec2& v2 );
    };

    std::string toString( const CVec2& v );

    CVec2 operator+ ( const CVec2& v1, const CVec2& v2 );
    CVec2 operator- ( const CVec2& v1, const CVec2& v2 );
    CVec2 operator* ( const CVec2& v, float32 s );
    CVec2 operator* ( float32 s, const CVec2& v );

    /**************************************************************************
    *                                CVec3                                    *
    ***************************************************************************/

    struct CVec3
    {
        float32 x;
        float32 y;
        float32 z;

        CVec3();
        CVec3( float32 x, float32 y, float32 z );
        CVec3( const CVec4& v4 );

        void normalize();
        void scale( float32 sx, float32 sy, float32 sz );
        void scale( const CVec3& vscale );
        float32 length();

        static CVec3 normalize( const CVec3& v );
        static float32 length( const CVec3& v );
        static CVec3 scale( const CVec3& v, float32 val );
        static CVec3 scale( const CVec3& v, const CVec3& vals );
        static CVec3 cross( const CVec3& v1, const CVec3& v2 );
        static float32 dot( const CVec3& v1, const CVec3& v2 );
        static bool equal( const CVec3& v1, const CVec3& v2 );

        static CVec3 Red() { return { 1.0f, 0.0f, 0.0f }; }
        static CVec3 Green() { return { 0.0f, 1.0f, 0.0f }; }
        static CVec3 Blue() { return { 0.0f, 0.0f, 1.0f }; }

        static CVec3 X() { return { 1.0f, 0.0f, 0.0f }; }
        static CVec3 Y() { return { 0.0f, 1.0f, 0.0f }; }
        static CVec3 Z() { return { 0.0f, 0.0f, 1.0f }; }
    };

    std::string toString( const CVec3& v );

    CVec3 operator+ ( const CVec3& v1, const CVec3& v2 );
    CVec3 operator- ( const CVec3& v1, const CVec3& v2 );
    CVec3 operator- ( const CVec3& v );
    CVec3 operator* ( const CVec3& v, float32 s );
    CVec3 operator* ( float32 s, const CVec3& v );

    /**************************************************************************
    *                                CLine                                    *
    ***************************************************************************/

    struct CLine
    {
        CVec3 start;
        CVec3 end;

        CLine();
        CLine( const CVec3& start, const CVec3& end );
    };

    std::string toString( const CLine& line );

    /**************************************************************************
    *                                CVec4                                    *
    ***************************************************************************/

    struct CVec4
    {
        float32 x;
        float32 y;
        float32 z;
        float32 w;

        CVec4();
        CVec4( float32 x, float32 y, float32 z, float32 w );
        CVec4( const CVec3& vec3, float32 w );
    };

    std::string toString( const CVec4& vec4 );

    /**************************************************************************
    *                                CMat4                                    *
    ***************************************************************************/

    struct CMat4
    {
        /* Buffer in Column-Major format */
        float32 buff[16];

        CMat4();
        CMat4( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis );
        CMat4( const CMat4& other );

        void set( int row, int col, float32 val );
        float32 get( int row, int col ) const;
        CVec3 getBasisVectorX() const;
        CVec3 getBasisVectorY() const;
        CVec3 getBasisVectorZ() const;
        CVec3 getPosition() const;
        CMat4 getRotation() const;
        CMat4 inverse() const;
        CMat4 transpose() const;

        static CMat4 perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar );
        static CMat4 ortho( float32 width, float32 height, float32 zNear, float32 zFar );
        static CMat4 lookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp );
        static CMat4 scale( const CVec3& v );
        static CMat4 translation( const CVec3& v );
        static CMat4 rotationX( float32 theta );
        static CMat4 rotationY( float32 theta );
        static CMat4 rotationZ( float32 theta );
        static CMat4 rotation( float32 theta, const eAxis& axis );
        static CMat4 rotation( float32 theta, const CVec3& axis );
        static CMat4 fromEuler( const CVec3& euler );
        static CMat4 fromEulerIntrinsicXYZ( const CVec3& euler );
    };

    std::string toString( const CMat4& mat );

    CMat4 operator* ( const CMat4& mat1, const CMat4& mat2 );
    CVec4 operator* ( const CMat4& mat, const CVec4& vec );

    /**************************************************************************
    *                                CInd3                                    *
    ***************************************************************************/

    struct CInd3
    {
        GLint buff[3];

        CInd3();
        CInd3( GLint v1, GLint v2, GLint v3 );
    };

    std::string toString( const CInd3& ind3 );

    /**************************************************************************
    *                            Geometric helpers                            *
    ***************************************************************************/

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
            return engine::signedDistToPlane( p1, plane ) < engine::signedDistToPlane( p2, plane );
        }
    };

    struct CComparatorX 
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.x < v2.x; }
    };

    struct CComparatorY
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.y < v2.y; }
    };

    struct CComparatorZ
    { 
        bool operator() ( const CVec3& v1, const CVec3& v2 ) { return v1.z < v2.z; }
    };

    struct CComparatorClosestToPoint
    {
        CVec3 point;

        bool operator() ( const CVec3& p1, const CVec3& p2 )
        {
            return engine::CVec3::length( p1 - point ) < engine::CVec3::length( p2 - point );
        }
    };

    struct CComparatorFarthestFromPoint
    {
        CVec3 point;

        bool operator() ( const CVec3& p1, const CVec3& p2 )
        {
            return engine::CVec3::length( p1 - point ) > engine::CVec3::length( p2 - point );
        }
    };

    void computeMinMaxVertexToPlane( const CPlane& plane, const CBoundingBox& bbox, CVec3& minVertex, CVec3& maxVertex );

    /**************************************************************************
    *                               Helpers                                   *
    ***************************************************************************/

    float32 toRadians( float32 angle );
    float32 toDegrees( float32 angle );

}