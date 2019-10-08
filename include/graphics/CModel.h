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

        void render() override;

        void addMesh( std::unique_ptr< CMesh > mesh );

        std::vector< CMesh* > meshes() const;

    private :

        std::vector< std::unique_ptr< CMesh > > m_meshes;

    };

}