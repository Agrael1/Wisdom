#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec4 color = { 1.0f, 0.5f, 0.5f, 1.0f };
};

// clang-format off
static constexpr inline std::array cube_vertices = {
    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},
    Vertex{.pos = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},
    Vertex{.pos = { 0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},
    Vertex{.pos = { 0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},
    Vertex{.pos = {-0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},
    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }},

    Vertex{.pos = {-0.5f, -0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},
    Vertex{.pos = { 0.5f, -0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},
    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},
    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},
    Vertex{.pos = {-0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},
    Vertex{.pos = {-0.5f, -0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }},

    Vertex{.pos = {-0.5f,  0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = {-0.5f,  0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = {-0.5f, -0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = {-0.5f,  0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},

    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = { 0.5f,  0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},
    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }},

    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f,  0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},
    Vertex{.pos = { 0.5f, -0.5f,  0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},
    Vertex{.pos = {-0.5f, -0.5f,  0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},
    Vertex{.pos = {-0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f,  0.0f }},

    Vertex{.pos = {-0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
    Vertex{.pos = { 0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
    Vertex{.pos = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
    Vertex{.pos = {-0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
    Vertex{.pos = {-0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  1.0f,  0.0f }},
};
// clang-format on
