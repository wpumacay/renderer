#!/usr/bin/env python
import os

import numpy as np
from OpenGL import GL

import renderer as rdr

WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
RESOURCES_PATH = os.path.join(CURRENT_DIR, "..", "resources")

VERT_SHADER_SRC = r"""
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texcoord;

    out vec2 vTexCoord;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
        vTexCoord = texcoord;
    }
"""

FRAG_SHADER_SRC = r"""
    #version 330 core

    in vec2 vTexCoord;
    out vec4 output_color;

    uniform sampler2D u_texture;

    void main() {
        output_color = texture(u_texture, vTexCoord);
    }
"""


def main() -> int:
    IMAGE_PATH = os.path.join(RESOURCES_PATH, "images", "container.jpg")

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
        # |--pos--|-texture-|#
        -0.5, -0.5, 0.0, 0.0,  # noqa: E131
         0.5, -0.5, 2.0, 0.0,  # noqa: E131
         0.5,  0.5, 2.0, 2.0,  # noqa: E131
        -0.5,  0.5, 0.0, 2.0,  # noqa: E131
    ], dtype=np.float32)

    NUM_VERTICES = 6

    buffer_indices = np.array([
        0, 1, 2,
        0, 2, 3
    ], dtype=np.uint32)
    # fmt: on

    layout = rdr.BufferLayout(
        [
            ["position", rdr.ElementType.FLOAT_2, False],
            ["texcoord", rdr.ElementType.FLOAT_2, False],
        ]
    )

    vbo = rdr.VertexBuffer(
        layout,
        rdr.BufferUsage.STATIC,
        buffer_data,
    )

    ibo = rdr.IndexBuffer(rdr.BufferUsage.STATIC, buffer_indices)

    vao = rdr.VertexArray()
    vao.AddVertexBuffer(vbo)
    vao.SetIndexBuffer(ibo)

    texture = rdr.Texture(os.path.abspath(IMAGE_PATH))

    while window.active:
        window.Begin()
        program.Bind()
        texture.Bind()
        vao.Bind()

        GL.glDrawElements(
            GL.GL_TRIANGLES, NUM_VERTICES, GL.GL_UNSIGNED_INT, None
        )

        vao.Unbind()
        texture.Unbind()
        program.Unbind()
        window.End()

    return 0


if __name__ == "__main__":
    SystemExit(main())
