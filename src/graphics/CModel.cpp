
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

    void CModel::addMesh( std::unique_ptr< CMesh > mesh, const CMat4& localTransform )
    {
        m_meshes.push_back( std::move( mesh ) );
        m_localTransforms.push_back( localTransform );
    }

    void CModel::render()
    {
        for ( auto& meshPtr : m_meshes )
            meshPtr->render();
    }

    std::vector< CMesh* > CModel::meshes() const
    {
        std::vector< CMesh* > _meshesPtrs;
        for ( auto& _submesh : m_meshes )
            _meshesPtrs.push_back( _submesh.get() );

        return _meshesPtrs;
    }

}