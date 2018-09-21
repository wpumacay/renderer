#version 330 core

in vec3 position;

out vec3 vTexDir;

uniform mat4 u_tProj;
uniform mat4 u_tView;

void main()
{
    vTexDir = position;
    vec4 pos = u_tProj * u_tView * vec4( position, 1.0 );
    gl_Position = pos.xyww;
} 