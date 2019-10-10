#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <camera/CICamera.h>

#include <graphics/CIRenderable>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <graphics/CScene.h>

#include <shaders/CShaderManager.h>
#include <shaders/CShader.h>

namespace engine
{

    class CMeshRenderer
    {

    public :

        CMeshRenderer();
        ~CMeshRenderer();

        void render( CScene* scenePtr, 
                     CICamera* cameraPtr = nullptr, 
                     CFrameBuffer* targetPtr = nullptr,
                     const CRenderOptions& renderOptions = CRenderOptions() );

    private :

        void _processRenderables();
        void _renderToShadowMap();
        void _renderScene();

    };

}