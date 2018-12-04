
#pragma once

#include "LMesh.h"

namespace engine
{


    class LModel : public LIRenderable
    {

        private :

        std::string             m_name;
        std::vector< LMesh* >   m_meshes;

        public :

        LModel( const std::string& name );
        ~LModel();

        void addMesh( LMesh* meshPtr );
        void setWireframeMode( bool useWireframe ) override;
        void render() override;

        std::vector< LMesh* > getMeshes() { return m_meshes; }

    };


}