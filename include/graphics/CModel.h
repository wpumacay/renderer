#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>

namespace engine
{

    class CModel : public CIRenderable
    {

    public :

        CModel( const std::string& name );
        ~CModel();

        void addMesh( std::unique_ptr< CMesh > mesh, const CMat4& localTransform );

        std::vector< CMesh* > meshes() const;

        std::vector< CMat4 > localTransforms() const { return m_localTransforms; }
        std::vector< CMat4 >& localTransforms() { return m_localTransforms; }

        static eRenderableType GetStaticType() { return eRenderableType::MODEL; }

    protected :

        void _renderInternal() override;

    private :

        std::vector< std::unique_ptr< CMesh > > m_meshes;
        std::vector< CMat4 > m_localTransforms;
    };

}