#pragma once

struct GLFWwindow;

class Window
{
public:
    Window(int width, int height, const char* title);

    bool ShouldClose() const;
    bool IsValid() const;
    void Close();
    void Present();

private:
    GLFWwindow* _window = nullptr;

};
