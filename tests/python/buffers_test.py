import numpy as np
import pytest
from OpenGL import GL

import renderer as rdr


@pytest.fixture
def window():
    return rdr.Window(800, 600, rdr.WindowBackend.TYPE_GLFW)


@pytest.fixture
def layout_1():
    return rdr.BufferLayout(
        [
            ["position", rdr.ElementType.FLOAT_2, False],
            ["color", rdr.ElementType.FLOAT_3, False],
        ]
    )


@pytest.fixture
def layout_2():
    return rdr.BufferLayout(
        [
            ["position", rdr.ElementType.FLOAT_3, False],
            ["normal", rdr.ElementType.FLOAT_3, True],
            ["texcoord", rdr.ElementType.FLOAT_2, False],
        ]
    )


@pytest.fixture
def data_1():
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
    return buffer_data


@pytest.fixture
def data_2():
    # fmt: off
    buffer_data = np.array([
        # |    position    |   normal   |-- texcoord --|#
           -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,  # noqa: E131
            0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  # noqa: E131
           -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  # noqa: E131
           -0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0,  # noqa: E131
    ], dtype=np.float32)
    # fmt: on
    return buffer_data


def test_vertex_buffer_attrs():
    assert hasattr(rdr.VertexBuffer, "layout")
    assert hasattr(rdr.VertexBuffer, "size")
    assert hasattr(rdr.VertexBuffer, "usage")
    assert hasattr(rdr.VertexBuffer, "opengl_id")


def test_vertex_buffer_ctor(
    window: rdr.Window,
    layout_1: rdr.BufferLayout,
    layout_2: rdr.BufferLayout,
    data_1: np.ndarray,
    data_2: np.ndarray,
) -> None:
    vbo_1 = rdr.VertexBuffer(layout_1, rdr.BufferUsage.STATIC, data_1)
    vbo_2 = rdr.VertexBuffer(layout_2, rdr.BufferUsage.STATIC, data_2)
    assert vbo_1.opengl_id != 0
    assert vbo_2.opengl_id != 0

    assert vbo_1.size == data_1.size * data_1.itemsize
    assert vbo_2.size == data_2.size * data_2.itemsize

    assert len(vbo_1.layout) == 2 and len(vbo_2.layout) == 3


def test_vertex_buffer_bind_methods(
    window: rdr.Window,
    layout_1: rdr.BufferLayout,
    layout_2: rdr.BufferLayout,
    data_1: np.ndarray,
    data_2: np.ndarray,
) -> None:
    vbo_1 = rdr.VertexBuffer(layout_1, rdr.BufferUsage.STATIC, data_1)
    vbo_2 = rdr.VertexBuffer(layout_2, rdr.BufferUsage.STATIC, data_2)

    vbo_1.Bind()
    assert GL.glGetIntegerv(GL.GL_ARRAY_BUFFER_BINDING) == vbo_1.opengl_id
    vbo_1.Unbind()
    assert GL.glGetIntegerv(GL.GL_ARRAY_BUFFER_BINDING) == 0

    vbo_2.Bind()
    assert GL.glGetIntegerv(GL.GL_ARRAY_BUFFER_BINDING) == vbo_2.opengl_id
    vbo_2.Unbind()
    assert GL.glGetIntegerv(GL.GL_ARRAY_BUFFER_BINDING) == 0


def test_vertex_buffer_resize_method(
    window: rdr.Window, layout_1: rdr.BufferLayout, data_1: np.ndarray
) -> None:
    vbo = rdr.VertexBuffer(layout_1, rdr.BufferUsage.DYNAMIC, data_1)

    vbo.Bind()
    EXPECTED_SIZE = data_1.size * data_1.itemsize
    GPU_SIZE = GL.glGetBufferParameteriv(GL.GL_ARRAY_BUFFER, GL.GL_BUFFER_SIZE)
    assert EXPECTED_SIZE == GPU_SIZE
    vbo.Unbind()

    NEW_SIZE = 200
    vbo.Resize(NEW_SIZE)
    vbo.Bind()
    NEW_GPU_SIZE = GL.glGetBufferParameteriv(
        GL.GL_ARRAY_BUFFER, GL.GL_BUFFER_SIZE
    )
    assert NEW_SIZE == NEW_GPU_SIZE
    vbo.Unbind()


def test_vertex_buffer_update_method(
    window: rdr.Window, layout_1: rdr.BufferLayout, data_1: np.ndarray
) -> None:
    vbo = rdr.VertexBuffer(layout_1, rdr.BufferUsage.DYNAMIC, data_1)

    # fmt: off
    new_buffer_data = np.array([
        # |    pos    |    color    |#
           -0.5, -0.5, 1.0, 0.0, 0.0,  # noqa: E131
            0.5, -0.5, 0.0, 1.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 1.0,  # noqa: E131
           -0.5, -0.5, 1.0, 0.0, 0.0,  # noqa: E131
            0.5,  0.5, 0.0, 0.0, 1.0,  # noqa: E131
           -0.5,  0.5, 1.0, 1.0, 1.0,  # noqa: E131
    ], dtype=np.float32)
    # fmt: on

    vbo.UpdateData(new_buffer_data)
