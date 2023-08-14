import numpy as np

import renderer as rdr

DEFAULT_WIDTH = 1024
DEFAULT_HEIGHT = 768
DEFAULT_CLEAR_COLOR = np.array([0.2, 0.2, 0.2, 1.0], dtype=np.float32)
DEFAULT_WINDOW_TITLE = "Application Window"
DEFAULT_GL_VERSION_MAJOR = 3
DEFAULT_GL_VERSION_MINOR = 3


def test_window_config_default() -> None:
    config = rdr.WindowConfig()
    assert config.backend == rdr.WindowBackend.TYPE_NONE
    assert config.width == DEFAULT_WIDTH and config.height == DEFAULT_HEIGHT
    assert config.title == DEFAULT_WINDOW_TITLE
    assert np.allclose(config.clear_color, DEFAULT_CLEAR_COLOR)
    assert config.gl_version_major == 3
    assert config.gl_version_minor == 3


def test_window_config_setters_getters() -> None:
    config = rdr.WindowConfig()
    config.backend = rdr.WindowBackend.TYPE_GLFW
    config.width = 1600
    config.height = 900
    config.title = "Example Window"
    config.clear_color.x = 0.7
    config.clear_color.y = 0.5
    config.clear_color.z = 0.3
    config.clear_color.w = 1.0
    config.gl_version_major = 4
    config.gl_version_minor = 3

    assert config.backend == rdr.WindowBackend.TYPE_GLFW
    assert config.width == 1600
    assert config.height == 900
    assert config.title == "Example Window"
    assert np.allclose(
        config.clear_color, np.array([0.7, 0.5, 0.3, 1.0], dtype=np.float32)
    )
    assert config.gl_version_major == 4
    assert config.gl_version_minor == 3


def test_window_constructor_from_config() -> None:
    config = rdr.WindowConfig()
    config.backend = rdr.WindowBackend.TYPE_GLFW
    config.width = 800
    config.height = 600
    config.title = "Test Window Constructors - from config"
    config.gl_version_major = 4
    config.gl_version_minor = 3

    window = rdr.Window(config)
    assert window.active is True
    assert window.backend == rdr.WindowBackend.TYPE_GLFW
    assert window.width == 800 and window.height == 600
    assert window.title == "Test Window Constructors - from config"
    assert np.allclose(window.clear_color, config.clear_color)


def test_window_constructor_from_args() -> None:
    window = rdr.Window(800, 600, rdr.WindowBackend.TYPE_GLFW)

    assert window.active is True
    assert window.backend == rdr.WindowBackend.TYPE_GLFW
    assert window.width == 800 and window.height == 600
    assert window.title == "Application Window"
    assert np.allclose(
        window.clear_color, np.array([0.2, 0.2, 0.2, 1.0], dtype=np.float32)
    )
