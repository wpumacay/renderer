#!/usr/bin/env python
from typing import Optional

import renderer as rdr

WINDOW_WIDTH = 1024
WINDOW_HEIGHT = 768


class Engine:
    def __init__(self):
        self.window: Optional[rdr.Window] = None
        self.input_manager: Optional[rdr.InputManager] = None


g_engine = Engine()


def main() -> int:
    g_engine.window = rdr.Window(
        WINDOW_WIDTH, WINDOW_HEIGHT, rdr.WindowBackend.TYPE_GLFW
    )
    g_engine.input_manager = rdr.InputManager()

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

    if g_engine.window is None:
        print("There was an error initializing the window")
        return 1

    if g_engine.input_manager is None:
        print("There was an error initializing the input manager")
        return 1

    while g_engine.window.active:
        g_engine.window.Begin()

        if g_engine.input_manager.IsKeyDown(rdr.Keys.KEY_ESCAPE):
            g_engine.window.RequestClose()

        print("Window: ")
        print(g_engine.window)
        print("InputManager: ")
        print(g_engine.input_manager)

        g_engine.window.End()

    return 0


if __name__ == "__main__":
    SystemExit(main())
