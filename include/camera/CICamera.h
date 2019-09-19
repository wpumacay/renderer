#pragma once

#include <LCommon.h>
#include <utils/CLogger.h>

namespace engine
{

    enum class eCameraType
    {
        BASE = 0,
        FIXED, FPS, ORBIT
    };

    std::string toString( const eCameraType& type );

    enum class eCameraProjection
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC,
    };

    std::string toString( const eCameraProjection& proj );

    struct CCameraProjData
    {
        eCameraProjection projection;
        /* perspective-camera parameters */
        float fov;
        float aspect;
        /* orthographics-camera parameters */
        float width;
        float height;
        /* common to both types*/
        float zNear;
        float zFar;
    };

    std::string toString( const CCameraProjData& projData );

    class CICamera
    {
    public :

        CICamera( const std::string& name,
                  const LVec3& position,
                  const LVec3& targetPoint,
                  const eAxis& upAxis,
                  const CCameraProjData& projData );

        virtual ~CICamera();

        static eCameraType GetStaticType() { return eCameraType::BASE; }

        void setPosition( const LVec3& position );
        void setActiveMode( bool activeMode );
        void setProjectionData( const CCameraProjData& projData );
        void update();
        std::string toString();

        std::string name() const { return m_name; }
        eCameraType type() const { return m_type; }
        bool active() const { return m_active; }

        LVec3 targetPoint() const { return m_targetPoint; }
        LVec3 targetDir() const { return m_targetDir; }
        LVec3 position() const { return m_position; }

        CCameraProjData projData() const { return m_projData; }

        LMat4 matView() const { return m_matView; }
        LMat4 matProj() const { return m_matProj; }

    protected :

        void _buildViewMatrix();
        void _buildProjMatrix();

        virtual void _positionChangedInternal() = 0;
        virtual void _updateInternal() = 0;
        virtual std::string _toStringInternal() = 0;

    protected :

        std::string m_name;
        eCameraType m_type;
        eAxis m_upAxis;

        LVec3 m_position;
        LVec3 m_worldUp;
        LVec3 m_targetPoint;
        LVec3 m_targetDir;
        
        LVec3 m_front;
        LVec3 m_up;
        LVec3 m_right;
        
        LMat4 m_matView;
        LMat4 m_matProj;

        CCameraProjData m_projData;

        bool m_active;
    };

}