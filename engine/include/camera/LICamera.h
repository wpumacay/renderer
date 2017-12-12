

#pragma once

#include "../LCommon.h"


namespace engine
{

    class LICamera
    {
        protected :

        LVec3 m_pos;
        LVec3 m_targetDir;
        LVec3 m_worldUp;

        virtual void _updateCamera();

        public :

        LICamera( const LVec3& pos,
                  const LVec3& targetDir,
                  const LVec3& worldUp );

        ~LICamera();

        LVec3 getPosition() const { return m_pos; }
        LVec3 getTargetDir() const { return m_targetDir; }

        void setPosition( const LVec3& pos );

        virtual glm::mat4 getViewMatrix() = 0;

    };



}