

#include "LILight.h"
#include "LShaderManager.h"



namespace engine
{


    LILight::LILight( const LVec3& ambient,
                      const LVec3& diffuse,
                      const LVec3& specular,
                      int lIndx )
    {
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        m_indx = lIndx;

        m_type = "base";

        active = 1;
    }

    LILight::~LILight()
    {

    }


}