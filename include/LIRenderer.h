

#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <LIRenderable.h>
#include <camera/CICamera.h>
#include <LILight.h>
#include <LScene.h>
#include <LShadowMap.h>
#include <shaders/CShaderManager.h>

namespace engine
{

    class LIRenderer
    {

    public :

        virtual void begin( LScene* pScene ) = 0;
        virtual void renderScene( LScene* pScene ) = 0;
        virtual void renderToShadowMap( LScene* pScene, LShadowMap* shadowMap ) = 0;
        virtual void renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap ) = 0;
        virtual void end( LScene* pScene ) = 0;

    };

}

