#version 330 core

// fragment-pair    : engine_lambert_no_shadows_fs
// material         : lambert
// shadows          : no
// fog              : yes (if enabled)

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texcoord;

uniform mat4 u_modelMatrix;    // converts position from model to world space
uniform mat4 u_normalMatrix;   // converts normals from model to world space
uniform mat4 u_viewProjMatrix; // converts position from world to clip-space (passed to frag. shader)
uniform mat4 u_viewMatrix;     // converts position from world to camera space (for fog-computations)

out vec3 fPosition;         // vertex position
out vec3 fNormal;           // vertex normal (not-normalized in fs. due to interpolation)
out vec2 fTexcoord;         // vertex texture coordinates (uvs)
out vec3 fPositionToCamera; // position w.r.t. camera (for fog calculation)

void main()
{
    vec4 _posInWorld = u_modelMatrix * vec4( position, 1.0f );
    gl_Position = u_viewProjMatrix * _posInWorld;

    fPosition = vec3( _posInWorld );
    fNormal = vec3( u_normalMatrix * vec4( normal, 0.0f ) );
    fTexcoord = texcoord;

    fPositionToCamera = vec3( u_viewMatrix * _posInWorld );
}