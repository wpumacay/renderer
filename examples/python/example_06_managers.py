#!/usr/bin/env python

import os
from typing import Optional

import numpy as np
from OpenGL import GL

import renderer as rdr

WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
SHADERS_DIR = os.path.join(CURRENT_DIR, "..", "resources", "shaders")
IMAGES_DIR = os.path.join(CURRENT_DIR, "..", "resources", "images")


class Engine:
    def __init__(self):
        self.window: Optional[rdr.Window] = None
        self.input_manager: Optional[rdr.InputManager] = None
        self.shader_manager: Optional[rdr.ShaderManager] = None
        self.texture_manager: Optional[rdr.TextureManager] = None


g_engine = Engine()


def main() -> int:
    g_engine.window = rdr.Window(
        WINDOW_WIDTH, WINDOW_HEIGHT, rdr.WindowBackend.TYPE_GLFW
    )
    g_engine.input_manager = rdr.InputManager()
    g_engine.shader_manager = rdr.ShaderManager()
    g_engine.texture_manager = rdr.TextureManager()

    if g_engine.window is None:
        print("There was an error initializing the window")
        return 1

    if g_engine.input_manager is None:
        print("There was an error initializing the input manager")
        return 1

    if g_engine.shader_manager is None:
        print("There was an error initializing the shader manager")
        return 1

    if g_engine.texture_manager is None:
        print("There was an error initializing the texture manager")
        return 1

    def keyboard_callback(key: int, action: int, _: int) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackKey(key, action)

    def mouse_button_callback(
        button: int, action: int, x: float, y: float
    ) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackMouseButton(button, action, x, y)

    def mouse_move_callback(x: float, y: float) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackMouseMove(x, y)

    def scroll_callback(xOff: float, yOff: float) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackScroll(xOff, yOff)

    g_engine.window.RegisterKeyboardCallback(keyboard_callback)
    g_engine.window.RegisterMouseButtonCallback(mouse_button_callback)
    g_engine.window.RegisterMouseMoveCallback(mouse_move_callback)
    g_engine.window.RegisterScrollCallback(scroll_callback)

    program = g_engine.shader_manager.LoadProgram(
        "basic2d",
        os.path.join(SHADERS_DIR, "basic2d_vert.glsl"),
        os.path.join(SHADERS_DIR, "basic2d_frag.glsl"),
    )

    texture = g_engine.texture_manager.LoadTexture(
        "container", os.path.join(IMAGES_DIR, "container.jpg")
    )

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

    program.Bind()
    color = np.array([0.2, 0.4, 0.8], dtype=np.float32)
    radius = 0.1
    program.SetFloat("u_time", 0.0)
    program.SetFloat("u_radius", radius)
    program.SetVec3("u_color", color)
    program.Unbind()

    while g_engine.window.active:
        g_engine.window.Begin()

        if g_engine.input_manager.IsKeyDown(rdr.Keys.KEY_ESCAPE):
            g_engine.window.RequestClose()

        cursor_position = g_engine.input_manager.GetCursorPosition()
        print(f"CursorPosition: ({cursor_position.x}, {cursor_position.y})")
        print(
            "MouseButtonLeft: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_LEFT)
            )
        )
        print(
            "MouseButtonMiddle: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_MIDDLE)
            )
        )
        print(
            "MouseButtonRight: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_RIGHT)
            )
        )

        # print("Window: ")
        # print(g_engine.window)
        # print("InputManager: ")
        # print(g_engine.input_manager)
        # print("ShaderManager: ")
        # print(g_engine.shader_manager)
        # print("TextureManager: ")
        # print(g_engine.texture_manager)

        program.Bind()
        texture.Bind()
        vao.Bind()

        GL.glDrawElements(
            GL.GL_TRIANGLES, NUM_VERTICES, GL.GL_UNSIGNED_INT, None
        )

        vao.Unbind()
        texture.Unbind()
        program.Unbind()
        g_engine.window.End()

    return 0


if __name__ == "__main__":
    SystemExit(main())
