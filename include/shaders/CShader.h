
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    class CShader
    {

    public :

        CShader( const std::string& name, uint32 openglId );
        ~CShader();

        void bind();
        void unbind();

        void setInt( const std::string& uName, GLint value );
        void setFloat( const std::string& uName, GLfloat value );
        void setVec2( const std::string& uName, const CVec2& vec );
        void setVec3( const std::string& uName, const CVec3& vec );
        void setVec4( const std::string& uName, const CVec4& vec );
        void setMat4( const std::string& uName, const CMat4& mat );

        std::string name() const { return m_name; }
        uint32 openglId() const { return m_openglId; }

    protected :

        void _setInt( uint32 uniform, GLint value );
        void _setFloat( uint32 uniform, GLfloat value );
        void _setVec2( uint32 uniform, const CVec2& vec );
        void _setVec3( uint32 uniform, const CVec3& vec );
        void _setVec4( uint32 uniform, const CVec4& vec );
        void _setMat4( uint32 uniform, const CMat4& mat );

        uint32 _getUniformLocation( const std::string& name );

        std::string                                 m_name;
        uint32                                      m_openglId;
        std::unordered_map< std::string, uint32 >   m_uniformLocations;

    };


}