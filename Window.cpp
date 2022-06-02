#include "Precompiled.h"
#include "Window.h"

Window::Window(int width, int height, const char* title)
{
    _window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {

        });

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}

void Window::Close()
{
    glfwDestroyWindow(_window);
}

bool Window::IsValid() const
{
    return _window != nullptr;
}

void Window::Present()
{
    glfwSwapBuffers(_window);
}


