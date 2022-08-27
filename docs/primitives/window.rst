Window
======

This primitive allows to create a window where we can render stuff into. It can
be used simply as follows:

.. code-block:: cpp

  constexpr int WIDTH = 800;
  constexpr int HEIGHT = 600;
  window = std::make_unique<loco::renderer::Window>(WIDTH, HEIGHT);

A basic rendering loop can be constructed using the ``window`` API, as shown
in the snippet below:

.. code-block:: cpp

  while(window->active()) {
      window->Begin();

      // Do your rendering steps here
      // ...
      // ...

      window->End();
  }

===================
C++ API Reference
===================

.. doxygenclass:: loco::renderer::Window
   :members:
