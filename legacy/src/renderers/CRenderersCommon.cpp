
#include <renderers/CRenderersCommon.h>

namespace engine
{

    std::string toString( const eRenderMode& mode )
    {
        if ( mode == eRenderMode::NORMAL ) return "normal";
        if ( mode == eRenderMode::DEPTH_ONLY ) return "depth_only";
        if ( mode == eRenderMode::SEMANTIC_ONLY ) return "semantic_only";
        if ( mode == eRenderMode::NO_SUBMIT ) return "no_submit";

        ENGINE_CORE_ASSERT( false, "Invalid eRenderMode enum given" );

        return "undefined";
    }

    std::string toString( const eCullingGeom& geom )
    {
        if ( geom == eCullingGeom::BOUNDING_BOX ) return "bounding_box";
        if ( geom == eCullingGeom::BOUNDING_SPHERE ) return "bounding_sphere";

        ENGINE_CORE_ASSERT( false, "Invalid eCullingGeom enum given" );

        return "undefined";
    }

    std::string toString( const CRenderOptions& renderOptions )
    {
        std::string _strRep;

        _strRep += "mode            : " + engine::toString( renderOptions.mode ) + "\n\r";
        _strRep += "frustum-culling : " + std::string( renderOptions.useFrustumCulling ? "yes" : "no" ) + "\n\r";
        _strRep += "culling-geom    : " + engine::toString( renderOptions.cullingGeom ) + "\n\r";
        _strRep += "face-culling    : " + std::string( renderOptions.useFaceCulling ? "yes" : "no" ) + "\n\r";
        _strRep += "shadow-mapping  : " + std::string( renderOptions.useShadowMapping ? "yes" : "no" ) + "\n\r";
        _strRep += "redraw-shadowmap: " + std::string( renderOptions.redrawShadowMap ? "yes" : "no" ) + "\n\r";
        _strRep += "pcf-filter-size : " + std::to_string( renderOptions.pcfCount ) + "\n\r";
        _strRep += "blending        : " + std::string( renderOptions.useBlending ? "yes" : "no" ) + "\n\r";
        _strRep += "fog             : " + std::string( renderOptions.useFog ? "yes" : "no" ) + "\n\r";
        _strRep += "skybox          : " + std::string( renderOptions.useSkybox ? "yes" : "no" ) + "\n\r";
        _strRep += "skfog-low-lim   : " + std::to_string( renderOptions.skyboxFogLowerLimit ) + "\n\r";
        _strRep += "skfog-upper-lim : " + std::to_string( renderOptions.skyboxFogUpperLimit ) + "\n\r";
        _strRep += "viewport-width  : " + std::to_string( renderOptions.viewportWidth ) + "\n\r";
        _strRep += "viewport-height : " + std::to_string( renderOptions.viewportHeight ) + "\n\r";
        _strRep += "render-target   : " + std::string( renderOptions.renderTargetPtr ? "yes" : "no" ) + "\n\r";
        _strRep += "d-view-zmin     : " + std::to_string( renderOptions.depthViewZmin ) + "\n\r";
        _strRep += "d-view-zmax     : " + std::to_string( renderOptions.depthViewZmax ) + "\n\r";
        _strRep += "d-view-zminColor: " + engine::toString( renderOptions.depthViewZminColor ) + "\n\r";
        _strRep += "d-view-zmaxColor: " + engine::toString( renderOptions.depthViewZmaxColor ) + "\n\r";

        _strRep += "s-view-color-map:\n\r";
        for ( auto kvPair : renderOptions.semanticViewIdMap )
            _strRep += "\t(" + std::to_string( kvPair.first ) + "): " + engine::toString( kvPair.second ) + "\n\r";

        return _strRep;
    }

}
