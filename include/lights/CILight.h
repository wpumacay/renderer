#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    enum class eLightType
    {
        NONE = 0,
        DIRECTIONAL,
        POINT,
        SPOT
    };

    std::string toString( const eLightType& type );

    class CILight
    {

    public :

        bool    enabled;
        CVec3   ambient;
        CVec3   diffuse;
        CVec3   specular;
        float32 intensity;
        bool    producesShadows;

        /* properties used by specific light types */
        CVec3   direction;
        CVec3   position;
        float32 atnConstant;
        float32 atnLinear;
        float32 atnQuadratic;
        float32 innerCutoff;
        float32 outerCutoff;

        CILight( const std::string& name,
                 const CVec3& ambientColor,
                 const CVec3& diffuseColor,
                 const CVec3& specularColor );

        virtual ~CILight();

        std::string toString() const;

        std::string name() const { return m_name; }
        eLightType type() const { return m_type; }

    protected :

        virtual std::string _toStringInternal() const = 0;

        std::string m_name;
        eLightType  m_type;
    };

}