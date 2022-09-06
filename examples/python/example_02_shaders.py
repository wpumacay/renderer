#!/usr/bin/env python

import renderer as rdr

WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768

VERT_SHADER_SRC = r"""
    #version 330 core

    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec3 color;

    out vec3 frag_color;

    void main() {
        gl_Position = vec4(pos, 0.0f, 1.0f);
        frag_color = color;
    }
"""

FRAG_SHADER_SRC = r"""
    #version 330 core

    in vec3 frag_color;
    out vec4 output_color;

    void main() {
        output_color = vec4(frag_color, 1.0f);
    }
"""

def main():
    window = rdr.Window(WINDOW_WIDTH, WINDOW_HEIGHT, rdr.WindowBackend.TYPE_GLFW)

    vert_shader = rdr.Shader("basic_2d_vert", rdr.ShaderType.VERTEX, VERT_SHADER_SRC)

    frag_shader = rdr.Shader("basic_2d_frag", rdr.ShaderType.FRAGMENT, FRAG_SHADER_SRC)

    if vert_shader.compiled:
        print("Vertex Shader successfully compiled")

    if frag_shader.compiled:
        print("Fragment Shader successfully compiled")

    program = rdr.Program("basic_2d")
    program.AddShader(vert_shader)
    program.AddShader(frag_shader)
    program.LinkProgram()

    if program.linked:
        print("Shader Program successfully linked")

    while window.active:
        window.Begin()

        window.End()


if __name__ == '__main__':
    main()
