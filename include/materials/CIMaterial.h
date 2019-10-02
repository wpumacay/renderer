#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <shaders/CShader.h>

namespace engine
{

    enum class eMaterialType
    {
        NONE = 0,
        LAMBERT, PHONG
    };

    std::string toString( const eMaterialType& type );

    class CIMaterial
    {

    public :

        CIMaterial( const std::string& name );
        virtual ~CIMaterial();

        virtual void bind( CShader* shaderPtr ) = 0;
        virtual void unbind() = 0;

        std::string toString();

        std::string name() const { return m_name; }
        eMaterialType type() const { return m_type; }

    protected :

        virtual std::string _toStringInternal() = 0;

        std::string m_name;
        eMaterialType m_type;

    };

}