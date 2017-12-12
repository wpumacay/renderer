
#pragma once

#include "LICamera.h"



namespace engine
{


    class LFixedCamera3d : public LICamera
    {

        protected :

        void _updateCamera() override;

        public :

        LFixedCamera3d( const LVec3& pos,
                        const LVec3& targetDir,
                        const LVec3& worldUp );

        glm::mat4 getViewMatrix() override;

    };




}