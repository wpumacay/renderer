#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CFrameBuffer.h>
#include <shaders/CShaderManager.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <camera/CICamera.h>
#include <utils/CLogger.h>

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

        static void Init( int viewportWidth, int viewportHeight );
        static void Release();
        static void SetMode( const ePickerMode& mode );
        static void Submit( const std::vector< CIRenderable* >& renderables, CICamera* camera );
        static CIRenderable* GetObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight );
        static CFrameBuffer* GetFbo();

        ~CObjectPicker();

    private :

        static CObjectPicker* s_instance;

        CObjectPicker( int viewportWidth, int viewportHeight );

        void _setMode( const ePickerMode& mode );
        void _submit( const std::vector< CIRenderable* >& renderables, CICamera* camera );
        CIRenderable* _getObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight );
        CFrameBuffer* _getFbo();

        CVec3 _createNormalEncoding( int objId );
        CVec3 _createVisualizationEncoding( int objId );

        void _encodeMesh( CMesh* meshPtr );
        void _encodeModel( CModel* modelPtr );
        
        CShader*                        m_shaderObjIdEncoder;   // shader used to render with ids encoded into colors
        ePickerMode                     m_mode;                 // which mode should the picker be working on
        std::unique_ptr< CFrameBuffer > m_fboObjsIds;           // fbo in which we'll render color-encoded ids
        std::vector< CIRenderable* >    m_renderablesInView;    // list of renderables encoded in the fbo

        int m_viewportWidth;
        int m_viewportHeight;

        /* resources to generate random colors for visualization-mode */
        std::default_random_engine                  m_randGen;
        std::uniform_real_distribution< float32 >   m_randDist;
        std::unordered_map< int, CVec3 >            m_cachedRandomColors;
    };

}