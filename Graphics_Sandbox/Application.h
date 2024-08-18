#pragma once

class Application
{
public:
    virtual ~Application() = default;
    
    virtual void update(float deltaTime) = 0;
    virtual void render(float deltaTime) = 0;
};
