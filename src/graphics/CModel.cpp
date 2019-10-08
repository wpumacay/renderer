
#include <graphics/CModel.h>

namespace engine
{

    CModel::CModel( const std::string& name )
        : CIRenderable( name )
    {
        m_type = eRenderableType::MODEL;
    }

    CModel::~CModel()
    {
        // nothing to release manually
    }

    void CModel::addMesh( std::unique_ptr< CMesh > mesh )
    {
        m_meshes.push_back( std::move( mesh ) );
    }

    void CModel::render()
    {
        for ( auto& meshPtr : m_meshes )
            meshPtr->render();
    }

}