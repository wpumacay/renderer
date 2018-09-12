

#pragma once

#include "LCommon.h"
#include "LIRenderable.h"
#include "LICamera.h"
#include "LILight.h"
#include "LScene.h"
#include "shaders/LShaderManager.h"

using namespace std;

namespace engine
{



    class LIRenderer
    {

        public :

        virtual void begin( LScene* pScene ) = 0;
        virtual void renderScene( LScene* pScene ) = 0;
        virtual void end( LScene* pScene ) = 0;

    };





}

