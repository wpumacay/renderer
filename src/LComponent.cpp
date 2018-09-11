

#include "LComponent.h"

using namespace std;


namespace engine
{


    LComponent::LComponent( LEntity* pEntity )
    {
        m_entity = pEntity;
        type = "base";
    }

    LComponent::~LComponent()
    {
        m_entity = NULL;
    }


    string LComponent::getStaticType()
    {
        return string( "base" );
    }

    void LComponent::update( float dt )
    {
        // Override this
    }

}