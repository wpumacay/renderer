

#pragma once

#include "../LCommon.h"

namespace engine
{

    class LIRenderable
    {

        public :

        virtual void render() = 0;

        virtual void enableLighting() = 0;
        virtual void disableLighting() = 0;
    };

}