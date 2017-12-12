

#pragma once

#include "../LCommon.h"
#include "../graphics/LIRenderable.h"
#include "../camera/LICamera.h"
#include "../light/LILight.h"
#include "../scene/LScene.h"

using namespace std;

namespace engine
{



    class LIRenderer
    {
        protected :

        GLuint m_shader;

        public :

        virtual void render( LIRenderable* renderable ) = 0;
        virtual void renderScene( LScene* pScene ) = 0;
        virtual void begin( LScene* pScene ) = 0;
        virtual void end( LScene* pScene ) = 0;

    };





}

