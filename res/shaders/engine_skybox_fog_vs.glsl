#version 330 core

layout ( location = 0 ) in vec3 position;

out vec3 fSampleDir;

uniform mat4 u_viewProjMatrix;

void main()
{
    /* direction used for sampling the cube */
    fSampleDir = position;
    /* position in clip space */
    gl_Position = u_viewProjMatrix * vec4( position, 1.0f );
}