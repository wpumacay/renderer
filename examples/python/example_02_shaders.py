#!/usr/bin/env python

import ctypes
from time import time as time_since_epoch

import numpy as np
from OpenGL.GL import *  # type: ignore

import renderer as rdr

VERT_SHADER_SRC = r"""
    #version 330 core

    layout (location = 0) in vec3 pos;

    void main() {
        gl_Position = vec4(pos, 1.0f);
    }
"""

FRAG_SHADER_SRC = r"""
    #version 330 core

    out vec4 output_color;

    uniform float u_time;

    void main() {
        output_color = vec4(abs(sin(u_time)), 0.0f, 0.0f, 1.0f);
    }
"""


def main() -> int:
    WINDOW_WIDTH = 1024
    WINDOW_HEIGHT = 768

    WINDOW_API = rdr.WindowBackend.TYPE_GLFW
    GRAPHICS_API = rdr.GraphicsAPI.OPENGL

    window = rdr.Window.CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_API)
    program = rdr.Program.CreateProgram(
        VERT_SHADER_SRC, FRAG_SHADER_SRC, GRAPHICS_API
    )

    program.Build()

    if program.valid:
        print("Shader Program successfully built")
    else:
        print("Shader Program got an error during building")
        return 1

    # fmt: off
    vertices = np.array([
        [-0.5, -0.5, 0.0],
        [ 0.5, -0.5, 0.0],
        [-0.5,  0.5, 0.0],
        [ 0.5, -0.5, 0.0],
        [ 0.5,  0.5, 0.0],
        [-0.5,  0.5, 0.0],
    ], dtype=np.float32)
    # fmt: on

    vao = glGenVertexArrays(1)
    vbo = glGenBuffers(1)

    # Prepare the layout for our data ------------------------------------------
    glBindVertexArray(vao)

    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.nbytes,
        vertices,
        GL_STATIC_DRAW,
    )

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, vertices[0].nbytes, ctypes.c_void_p(0)
    )
    glEnableVertexAttribArray(0)

    glBindVertexArray(0)
    # --------------------------------------------------------------------------

    t_start = time_since_epoch()
    while window.active:
        window.Begin()

        program.Bind()
        program.SetFloat("u_time", time_since_epoch() - t_start)
        glBindVertexArray(vao)
        glDrawArrays(GL_TRIANGLES, 0, 6)
        glBindVertexArray(0)
        program.Unbind()

        window.End()

    glDeleteBuffers(1, vbo)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
