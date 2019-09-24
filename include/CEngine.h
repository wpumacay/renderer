#pragma once

#include <CCommon.h>
#include <CMath.h>

#include <utils/CTime.h>
#include <utils/CLogger.h>
#include <utils/CImguiUi.h>
#include <utils/CImguiUiDemo.h>
#include <LHeightmapGenerator.h>                    // #include <utils/CHeightMapGenerator.h>

#include <input/CInputCallbacks.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/LInputHandler.h>                    // #include <input/CInputHandler.h>
#include <input/LInputSystem.h>                     // #include <input/CInputSystem.h>

#include <core/CVertexBuffer.h>
#include <core/CVertexBufferLayout.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>
#include <core/CTexture.h>
#include <core/CTextureCube.h>
#include <core/CFrameBuffer.h>

#include <LIRenderable.h>                           // #include <graphics/CRenderable.h>
#include <LMesh.h>                                  // #include <graphics/CMesh.h>
#include <LModel.h>                                 // #include <graphics/CModel.h>
#include <graphics/CMeshBuilder.h>
#include <LMaterial.h>                              // #include <graphics/CMaterial.h>
#include <LFog.h>                                   // #include <graphics/CFog.h>
#include <LScene.h>                                 // #include <graphics/CScene.h>

#include <shaders/CShaderUniforms.h>
#include <shaders/LShader.h>                        // #include <shaders/CShader.h>
#include <shaders/LShaderBasic3d.h>                 // #include <shaders/CShaderBasic3d.h>
#include <shaders/LShaderDebug3d.h>                 // #include <shaders/CShaderDebug3d.h>
#include <shaders/LShaderEntitiesLighting.h>        // #include <shaders/CShaderLighting.h>
#include <shaders/LShaderEntitiesLightingShadows.h> // #include <shaders/CShaderLightingShadows.h>
#include <shaders/LShaderShadowMap.h>               // #include <shaders/CShaderShadowMap.h>
#include <shaders/LShaderSkybox.h>                  // #include <shaders/CShaderSkybox.h>
#include <shaders/LShaderManager.h>                 // #include <shaders/CShaderManager.h>

#include <camera/CICamera.h>                        // #include <camera/CCamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

#include <LILight.h>                                // #include <light/CLight.h>
#include <LLightDirectional.h>                      // #include <light/CDirectiona.hlLight.h>
#include <LLightPoint.h>                            // #include <light/CPointLight.h>
#include <LLightSpot.h>                             // #include <light/CSpotLight.h>

#include <debug/LDebugDrawer.h>                     // #include <debug/CDebugDrawer.h>
#include <debug/LDebugSystem.h>                     // #include <debug/CDebugSystem.h>

#include <LMasterRenderer.h>                        // #include <renderer/COpenGLRenderer.h>
#include <LMeshRenderer.h>                          // #include <renderer/COpenGLMeshRenderer.h>
#include <LSkyboxRenderer.h>                        // #include <renderer/COpenGLSkyboxRender.h>

#include <assets/CTextureManager.h>

#include <app/COpenGLWindow.h>
#include <app/COpenGLContext.h>
#include <app/COpenGLApp.h>


