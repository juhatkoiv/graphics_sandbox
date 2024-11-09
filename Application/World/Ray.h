#pragma once

#include <glm/glm.hpp>

DECLARE1(class, window, Window);
DECLARE1(struct, ecs, Camera);

BEGIN_NAMESPACE1(world)

struct Ray
{
    struct Args 
    {
        glm::mat4 projMatrix{};
        glm::mat4 viewMatrix{};
        glm::vec3 viewPosition{};
        glm::vec2 windowSize{};
        glm::vec2 screenCoord{};
    };

    glm::vec3 origin;
    glm::vec3 dir;
    unsigned int id = 0;

    glm::vec3 GetRayEnd(float rayDistance) const
    {
        return origin + dir * rayDistance;
    }

    static Ray FindMouseToWorldRay(const ecs::Camera& camera, const glm::vec2& screenCoord);

    bool operator == (const Ray& other) const 
    {
        return id == other.id;
    }

    std::size_t operator()(const Ray& ray) const 
    {
        return std::hash<std::size_t>{}(ray.id);
    }
};

END_NAMESPACE1