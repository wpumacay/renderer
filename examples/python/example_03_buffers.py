#!/usr/bin/env python

import numpy as np
from OpenGL import GL

import renderer as rdr

WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768


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
    window = rdr.Window(
        WINDOW_WIDTH, WINDOW_HEIGHT, rdr.WindowBackend.TYPE_GLFW
    )

    def key_callback(key: int, action: int, _: int) -> None:
        if key == rdr.Keys.KEY_ESCAPE and action == rdr.KeyAction.RELEASED:
            window.RequestClose()

    window.RegisterKeyboardCallback(key_callback)

    program = rdr.Program("basic_2d", VERT_SHADER_SRC, FRAG_SHADER_SRC)

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

    layout = rdr.BufferLayout(
        [
            ["position", rdr.ElementType.FLOAT_2, False],
            ["color", rdr.ElementType.FLOAT_3, False],
        ]
    )

    vbo = rdr.VertexBuffer(
        layout,
        rdr.BufferUsage.STATIC,
        buffer_data.size * buffer_data.itemsize,
        buffer_data,
    )

    vao = rdr.VertexArray()
    vao.AddVertexBuffer(vbo, False)

    while window.active:
        window.Begin()
        program.Bind()
        vao.Bind()

        GL.glDrawArrays(GL.GL_TRIANGLES, 0, NUM_VERTICES)

        vao.Unbind()
        program.Unbind()
        window.End()

    return 0


if __name__ == "__main__":
    SystemExit(main())
