#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <shaders/CShader.h>

namespace engine
{

    enum class eFogType
    {
        NONE = 0,
        LINEAR,
        EXPONENTIAL
    };

    std::string toString( const eFogType& type );

    class CFog
    {

    public :

        CVec3   color;
        float32 density;
        float32 gradient;
        float32 distStart;
        float32 distEnd;

        CFog( const eFogType& type, 
              const CVec3& fogColor, 
              float32 fogDensity, 
              float32 fogGradient, 
              float32 fogDistStart, 
              float32 fogDistEnd );

        ~CFog();

        void bind( CShader* shaderPtr );
        void unbind();

        std::string toString();

        void setType( const eFogType& type ) { m_type = type; }
        void setActive( bool active ) { m_active = active; }

        eFogType type() const { return m_type; }
        bool active() const { return m_active; }

    private :

        eFogType    m_type;
        bool        m_active;
    };
}
