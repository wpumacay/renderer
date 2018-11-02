
#include "LCommon.h"

namespace engine
{

    float toRadians( float angle )
    {
        return angle * M_PI / 180.0;
    }

    LVec4 operator* ( const LMat4& mat, const LVec4& vec )
    {
        LVec4 _res;
        _res.x = mat.get( 0, 0 ) * vec.x + mat.get( 0, 1 ) * vec.y + 
                 mat.get( 0, 2 ) * vec.z + mat.get( 0, 3 ) * vec.w;

        _res.y = mat.get( 1, 0 ) * vec.x + mat.get( 1, 1 ) * vec.y + 
                 mat.get( 1, 2 ) * vec.z + mat.get( 1, 3 ) * vec.w;

        _res.z = mat.get( 2, 0 ) * vec.x + mat.get( 2, 1 ) * vec.y + 
                 mat.get( 2, 2 ) * vec.z + mat.get( 2, 3 ) * vec.w;

        _res.w = mat.get( 3, 0 ) * vec.x + mat.get( 3, 1 ) * vec.y + 
                 mat.get( 3, 2 ) * vec.z + mat.get( 3, 3 ) * vec.w;

        return _res;
    }

    LVec3 operator+ ( const LVec3& v1, const LVec3& v2 )
    {
        return LVec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
    }

    LVec3 operator- ( const LVec3& v1, const LVec3& v2 )
    {
        return LVec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
    }

}