
#include "LModel.h"

namespace engine
{


    LModel::LModel( const std::string& name )
    {
        m_name = name;

        m_type = RENDERABLE_TYPE_MODEL;

        m_material = new LMaterial();
        
        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LModel::~LModel()
    {
        for ( size_t i = 0; i < m_meshes.size(); i++ )
        {
            delete m_meshes[i];
        }
        m_meshes.clear();
    }

    void LModel::addMesh( LMesh* meshPtr )
    {
        m_meshes.push_back( meshPtr );
    }

    void LModel::setWireframeMode( bool useWireframe )
    {
        LIRenderable::setWireframeMode( useWireframe );

        for ( size_t i = 0; i < m_meshes.size(); i++ )
        {
            m_meshes[i]->setWireframeMode( useWireframe );
        }
    }

    void LModel::render()
    {
        for ( size_t i = 0; i < m_meshes.size(); i++ )
        {
            m_meshes[i]->render();
        }
    }

}