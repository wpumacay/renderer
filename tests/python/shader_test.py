import pytest

import renderer as rdr


@pytest.fixture
def window():
    return rdr.Window(800, 600, rdr.WindowBackend.TYPE_GLFW)


def test_shader_ctor_from_good_source(window: rdr.Window) -> None:
    GOOD_VERTEX_SHADER_SRC = r"""
        # version 330 core

        layout (location = 0) in vec2 pos;
        layout (location = 1) in vec3 color;

        out vec3 frag_color;

        void main() {
            gl_Position = vec4(pos, 0.0f, 1.0f);
            frag_color = color;
        }
    """

    shader = rdr.Shader(
        "test_good_basic_2d", rdr.ShaderType.VERTEX, GOOD_VERTEX_SHADER_SRC
    )

    assert shader.name == "test_good_basic_2d"
    assert shader.type == rdr.ShaderType.VERTEX
    assert shader.source == GOOD_VERTEX_SHADER_SRC
    assert shader.opengl_id > 0
    assert shader.compiled is True


def test_shader_ctor_from_bad_source(window: rdr.Window) -> None:
    BAD_VERTEX_SHADER_SRC = r"""
        # version 330 core

        varying in vec2 pos
        varying in vec3 color

        out frag_color;

        void main() {
            gl_Position = pos;
            frag_color = color;
        }
    """

    shader = rdr.Shader(
        "test_bad_basic_2d", rdr.ShaderType.VERTEX, BAD_VERTEX_SHADER_SRC
    )

    assert shader.name == "test_bad_basic_2d"
    assert shader.type == rdr.ShaderType.VERTEX
    assert shader.source == BAD_VERTEX_SHADER_SRC
    assert shader.opengl_id <= 0
    assert shader.compiled is False
