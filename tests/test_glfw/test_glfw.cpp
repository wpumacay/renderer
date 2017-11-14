
#include <iostream>

using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if ( !glfwInit() )
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );
    if ( !window )
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent( window );

	if ( glewInit() != GLEW_OK )
	{
		cout << "failed initializing glew" << endl;
		return -1;
	}

	GLuint vbo;
	glGenBuffers( 1, &vbo );

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose( window ) )
    {
        /* Render here */
        glClear( GL_COLOR_BUFFER_BIT );

        /* Swap front and back buffers */
        glfwSwapBuffers( window );

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}