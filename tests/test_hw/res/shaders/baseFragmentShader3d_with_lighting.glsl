#version 330 core

uniform vec3 u_light_ambient;
uniform vec3 u_light_diffuse;
uniform vec3 u_light_specular;
uniform vec3 u_light_position;

uniform vec3 u_mat_ambient;
uniform vec3 u_mat_diffuse;
uniform vec3 u_mat_specular;
uniform float u_mat_shininess;

in vec3 vNormal;
in vec3 vFragPos;
out vec4 color;

void main()
{
    // ambient component

    vec3 c_ambient = u_light_ambient * u_mat_ambient;

    // diffuse component

    vec3 _normal = normalize( vNormal );
    vec3 _lightDir = normalize( u_light_position - vFragPos );
    float _diff = max( dot( _normal, _lightDir ), 0.0 );
    vec3 c_diffuse = _diff * u_light_diffuse;

    // vec3 c_specular = ...

    vec3 _res = ( c_ambient + c_diffuse ) * u_mat_ambient;

    color = vec4( _res, 1.0 );
}