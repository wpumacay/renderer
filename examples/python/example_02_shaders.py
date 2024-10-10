#!/usr/bin/env python

import renderer as rdr

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


def main() -> int:
    WINDOW_WIDTH = 1024
    WINDOW_HEIGHT = 768

    WINDOW_API = rdr.WindowBackend.TYPE_GLFW
    GRAPHICS_API = rdr.GraphicsAPI.OPENGL

    _ = rdr.Window.CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_API)
    program = rdr.Program.CreateProgram(
        VERT_SHADER_SRC, FRAG_SHADER_SRC, GRAPHICS_API
    )

    program.Build()

    if program.valid:
        print("Shader Program successfully built")
    else:
        print("Shader Program got an error during building")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
