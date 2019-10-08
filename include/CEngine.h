#pragma once

#include <CCommon.h>
#include <CMath.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <utils/CImguiUi.h>
#include <utils/CImguiUiDemo.h>
#include <utils/CNoiseGenerator.h>
#include <utils/CDebugDrawer.h>

#include <input/CInputCallbacks.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputHandler.h>

#include <core/CVertexBuffer.h>
#include <core/CVertexBufferLayout.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>
#include <core/CTexture.h>
#include <core/CTextureCube.h>
#include <core/CFrameBuffer.h>

#include <graphics/CShadowMap.h>        // REPLACE IN RENDERER PIPELINE
#include <graphics/CFog.h>              // REPLACE IN RENDERER PIPELINE
#include <graphics/CSkybox.h>           // REPLACE IN RENDERER PIPELINE
#include <graphics/CMeshBuilder.h>      // REPLACE IN RENDERER PIPELINE
#include <graphics/CIRenderable.h>      // REPLACE IN RENDERER PIPELINE
#include <graphics/CMesh.h>             // REPLACE IN RENDERER PIPELINE
#include <graphics/CModel.h>            // REPLACE IN RENDERER PIPELINE
#include <graphics/CScene.h>            // REPLACE IN RENDERER PIPELINE

#include <materials/CIMaterial.h>
#include <materials/CPhongMaterial.h>
#include <materials/CLambertMaterial.h>

#include <shaders/CShaderUniforms.h>
#include <shaders/CShader.h>
#include <shaders/CShaderManager.h>

#include <camera/CICamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>

// #include <renderers/CMainRenderer.h>     // IMPLEMENT RENDERER PIPELINE
// #include <renderers/CMeshRenderer.h>     // IMPLEMENT RENDERER PIPELINE
// #include <renderers/CSkyboxRenderer.h>   // IMPLEMENT RENDERER PIPELINE
// #include <renderers/CGizmosRenderer.h>   // IMPLEMENT RENDERER PIPELINE

#include <assets/CTextureManager.h>

#include <app/COpenGLWindow.h>
#include <app/COpenGLContext.h>
#include <app/COpenGLApp.h>


