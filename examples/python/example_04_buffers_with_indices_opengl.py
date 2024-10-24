#!/usr/bin/env python

import numpy as np
from OpenGL.GL import *  # type: ignore

import renderer as rdr
from renderer.opengl import (
    OpenGLBufferElement,
    OpenGLBufferLayout,
    OpenGLProgram,
    OpenGLVertexArray,
    OpenGLVertexBuffer,
    OpenGLIndexBuffer,
)

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

    win_config = rdr.WindowConfig()
    win_config.backend = WINDOW_API
    win_config.width = WINDOW_WIDTH
    win_config.height = WINDOW_HEIGHT
    win_config.title = "Example 03 - OpenGL Buffers"
    win_config.gl_version_major = 3
    win_config.gl_version_minor = 3

    window = rdr.Window.CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_API)

    def keycallback(key, action, mods):
        if key == rdr.Keys.KEY_ESCAPE:
            window.RequestClose()

    window.RegisterKeyboardCallback(keycallback)

    program = OpenGLProgram(VERT_SHADER_SRC, FRAG_SHADER_SRC)
    program.Build()

    if program.valid:
        print("Shader Program successfully built")
    else:
        print("Shader Program got an error during building")
        return 1

    # fmt: off
    buffer_data = np.array([
        # |  pos  | -   color-    |#
        -0.5, -0.5, 1.0, 0.0, 0.0,  # noqa: E131
         0.5, -0.5, 0.0, 1.0, 0.0,  # noqa: E131
         0.5,  0.5, 0.0, 0.0, 1.0,  # noqa: E131
        -0.5,  0.5, 1.0, 1.0, 1.0,  # noqa: E131
    ], dtype=np.float32)
    # fmt: on
    NUM_INDICES = 6

    buffer_indices = np.array([0, 1, 2, 0, 2, 3], dtype=np.uint32)

    layout = OpenGLBufferLayout()
    layout.AddElement(
        OpenGLBufferElement("position", rdr.ElementType.FLOAT_2, False)
    )
    layout.AddElement(
        OpenGLBufferElement("color", rdr.ElementType.FLOAT_3, False)
    )

    vbo = OpenGLVertexBuffer(layout, rdr.BufferUsage.STATIC, buffer_data)
    ibo = OpenGLIndexBuffer(rdr.BufferUsage.STATIC, buffer_indices)

    vao = OpenGLVertexArray()
    vao.AddVertexBuffer(vbo)
    vao.SetIndexBuffer(ibo)

    while window.active:
        window.Begin()
        program.Bind()
        
        vao.Bind()
        glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, None)
        vao.Unbind()
        
        program.Unbind()
        window.End()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
