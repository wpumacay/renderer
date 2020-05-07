#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CFrameBuffer.h>
#include <shaders/CShaderManager.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <camera/CICamera.h>

namespace engine
{

    enum class ePickerMode
    {
        NONE = 0,
        NORMAL, // normal encoding-decoding operation
        VISUALIZE, // encodings are given colors from a map to visualize the masks
        STOPPED // no operations executed
    };

    std::string toString( const ePickerMode& mode );

    class CObjectPicker
    {

    public :

        CObjectPicker( int viewportWidth, int viewportHeight );
        ~CObjectPicker();

        void setMode( const ePickerMode& mode );

        void begin( CICamera* camera );
        void submit( const std::vector< CIRenderable* >& renderables );
        void render();

        CIRenderable* getObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight );
        CFrameBuffer* getFbo();

    private :


        CVec3 _createNormalEncoding( int objId );
        CVec3 _createVisualizationEncoding( int objId );

        void _encodeMesh( CMesh* meshPtr );
        void _encodeModel( CModel* modelPtr );
        
    private :

        CShader*                        m_shaderObjIdEncoder;   // shader used to render with ids encoded into colors
        ePickerMode                     m_mode;                 // which mode should the picker be working on
        std::unique_ptr< CFrameBuffer > m_fboObjsIds;           // fbo in which we'll render color-encoded ids
        std::vector< CIRenderable* >    m_renderablesInView;    // list of renderables encoded in the fbo
        std::unique_ptr< CFrustum >     m_viewFrustum;
        CMat4                           m_viewProjMatrix;

        int m_viewportWidth;
        int m_viewportHeight;

        /* resources to generate random colors for visualization-mode */
        std::default_random_engine                  m_randGen;
        std::uniform_real_distribution< float32 >   m_randDist;
        std::unordered_map< int, CVec3 >            m_cachedRandomColors;
    };

}