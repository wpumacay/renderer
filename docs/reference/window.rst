Window
######

A window abstracts away the required setup for creating a Rendering Context
according to the user requirements. For example, we can create a window for
``OpenGL`` support using ``GLFW`` as backend. It's also possible to create a
window for headless rendering, in which case we could use ``EGL`` or ``OSMESA``
as backend. Notice that for now we're just supporting ``OpenGL`` as graphics API
and ``GLFW``, ``EGL``, and ``OSMESA`` as windowing backends. In future updates
we'll add support for windows that use the ``Win32`` windowing API in order to
create a context for ``DirectX`` as graphics API.

Creating a Window
-----------------

In order to create a Window, use the provided ``window_t`` class exposed through
the ``renderer/window/window_t.hpp`` header file, as follows:

.. code-block:: cpp

  // Include the window_t.hpp header
  #include <renderer/window/window_t.hpp>

  // Create a window_t object, specifying the desired backend
  constexpr int WINDOW_WIDTH = 1024;
  constexpr int WINDOW_HEIGHT = 768;
  constexpr auto WINDOW_BACKEND = renderer::eWindowBackend::TYPE_GLFW;
  auto window = std::make_unique<renderer::Window>(WINDOW_WIDTH,
                                                   WINDOW_HEIGHT,
                                                   WINDOW_BACKEND);


If you're using the Python Bindings, then just use the ``renderer.Window``
class, as follows:

.. code-block:: python

  # Import the rendering library
  import renderer as rdr

  # Create a Window object
  WINDOW_WIDTH = 1024
  WINDOW_HEIGHT = 768
  WINDOW_BACKEND = rdr.WindowBackend.TYPE_GLFW
  window = rdr.Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BACKEND)

The Render Loop
---------------

Once we have created our window object, we can construct our ``Render Loop``
by using the ``Begin`` and ``End`` methods. We just define our render logic
in between calls to these methods and internally the window will handle the
required setup to have something rendered on our screen.

.. code-block:: cpp

  // Render Loop (keep active while window is alive)
  while (window->active) {
    // Clears the screen, configures internals of graphics API, etc.
    window->Begin();

    // Rendering steps can go here, as either raw calls to the Graphics API or
    // by using other rendering primitives we expose in the library
    // ...

    // Swaps the FrameBuffer, etc.
    window->End();
  }

Similarly, using Python we can do the same as follows:

.. code-block:: python

  # Render Loop (keep active while window is alive)
  while window.active:
    # Clears the screen, configures internals of graphics API, etc.
    window.Begin()

    # Rendering steps can go here, as either raw calls to the Graphics API or
    # by using other rendering primitives we expose in the library
    # ...

    # Swaps the FrameBuffer, etc.
    window.End()

Registering Events
------------------

We can register for listening to events from inputs like the keyboard and mouse,
as well to other events associated to the window itself using the appropriate
``RegisterXYZCallback`` methods. We just have to pass the required callback to
this methods, following the specific function signature_. An example of how to
register for an event is shown below:

.. _signature: https://github.com/wpumacay/renderer/blob/dev/include/renderer/input/callbacks.hpp

.. code-block:: cpp

  // Register a callback for keyboard events (can just pass a lambda as well)
  window->RegisterKeyboardCallback([&](int key, int action, int mods) {
    // Handle this event in some useful way
    // ...
  });

  // Register a callback for mouse-button events (can just pass a lambda as well)
  window->RegisterMouseButtonCallback([&](int button, int action, double xpos, double ypos) {
    // Handle this event in some useful way
    // ...
  });

The Python counterpart is similar to configure:

.. code-block:: python

  # Define the callback to be used for listening to keyboard events
  def myKeyCallback(key: int, action: int, mods: int) -> None:
    # Handle this event in some useful way
    ...

  # Register the callback to start listening to keyboard events
  window.RegisterKeyboardCallback(myKeyCallback)


A Minimal Example
-----------------

TODO

API Reference
-------------

.. doxygenclass:: renderer::Window
   :members:
