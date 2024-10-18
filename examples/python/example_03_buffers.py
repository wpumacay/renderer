#!/usr/bin/env python

import numpy as np
from OpenGL.GL import *  # type: ignore

import renderer as rdr

VERT_SHADER_SRC = r"""
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;

    out vec3 frag_color;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
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

    window = rdr.Window.CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_API)

    def key_callback(key: int, action: int, _: int) -> None:
        if key == rdr.Keys.KEY_ESCAPE and action == rdr.KeyAction.RELEASED:
            window.RequestClose()

    window.RegisterKeyboardCallback(key_callback)

    program = rdr.Program.CreateProgram(
        VERT_SHADER_SRC, FRAG_SHADER_SRC, GRAPHICS_API
    )
    program.Build()
    if not program.valid:
        print("There was an error building the shader program")
        return 1

    # fmt: off
    buffer_data = np.array([
        # |    pos    |    color    |#
           -0.5, -0.5, 1.0, 0.0, 0.0,  # noqa: E131
            0.5, -0.5, 0.0, 1.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 1.0,  # noqa: E131
           -0.5, -0.5, 1.0, 0.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 1.0,  # noqa: E131
           -0.5,  0.5, 1.0, 1.0, 1.0,  # noqa: E131
    ], dtype=np.float32)
    # fmt: on
    NUM_VERTICES = 6

    # TODO(wilbert): Add constructor for bufferlayout that uses list of elements
    layout = rdr.BufferLayout()
    layout.AddElement(
        rdr.BufferElement("position", rdr.ElementType.FLOAT_2, False)
    )
    layout.AddElement(
        rdr.BufferElement("color", rdr.ElementType.FLOAT_3, False)
    )

    while window.active:
        window.Begin()
        program.Bind()

        # glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES)

        program.Unbind()
        window.End()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
