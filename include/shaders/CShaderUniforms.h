# pragma once

namespace engine
{

    struct LFogUniforms
    {
        GLuint type;
        GLuint color;
        GLuint density;
        GLuint start;
        GLuint end;
        GLuint isActive;
    };

    struct LLDirUniforms
    {
        GLuint direction;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LLPointUniforms
    {
        GLuint position;
        GLuint constant;
        GLuint linear;
        GLuint quadratic;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LLSpotUniforms
    {
        GLuint position;
        GLuint direction;
        GLuint cutOff;
        GLuint outerCutOff;
        GLuint constant;
        GLuint linear;
        GLuint quadratic;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LMaterialUniforms
    {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    };

}