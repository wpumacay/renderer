import os

import numpy as np
from OpenGL import GL

import renderer as rdr

WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
SHADERS_DIR = os.path.join(CURRENT_DIR, "..", "resources", "shaders")


def main() -> int:
    window = rdr.Window(
        WINDOW_WIDTH, WINDOW_HEIGHT, rdr.WindowBackend.TYPE_GLFW
    )

    def keyboard_callback(key, action, mods) -> None:
        if key == rdr.Keys.KEY_ESCAPE and action == rdr.KeyAction.RELEASED:
            window.RequestClose()

    window.RegisterKeyboardCallback(keyboard_callback)

    shader_manager = rdr.ShaderManager()

    program = shader_manager.LoadProgram(
        "basic3d",
        os.path.join(SHADERS_DIR, "basic3d_vert.glsl"),
        os.path.join(SHADERS_DIR, "basic3d_frag.glsl"),
    )

    # fmt: off
    buffer_data = np.array([
        # |----pos----|
        -1.0, 0.0, -1.0,  # noqa: E131
         1.0, 0.0, -1.0,  # noqa: E131
         0.0, 0.0,  1.0,  # noqa: E131
    ], dtype=np.float32)
    # fmt: on

    layout = rdr.BufferLayout([["position", rdr.ElementType.FLOAT_3, False]])

    vbo = rdr.VertexBuffer(layout, rdr.BufferUsage.STATIC, buffer_data)

    vao = rdr.VertexArray()
    vao.AddVertexBuffer(vbo)

    camera = rdr.Camera([0.0, -3.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 1.0])
    color = np.array([0.1, 0.2, 0.7], dtype=np.float32)

    while window.active:
        window.Begin()
        program.Bind()
        vao.Bind()

        program.SetMat4("u_model_matrix", np.identity(4, dtype=np.float32))
        program.SetMat4("u_view_matrix", camera.view_matrix())
        program.SetMat4("u_proj_matrix", camera.proj_matrix())
        program.SetVec3("u_color", color)

        GL.glDrawArrays(GL.GL_TRIANGLES, 0, 3)

        vao.Unbind()
        program.Unbind()
        window.End()

    return 0


if __name__ == "__main__":
    SystemExit(main())
