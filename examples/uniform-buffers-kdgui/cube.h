#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec4 color = { 1.0f, 0.5f, 0.5f, 1.0f };
};

constexpr float side = 0.5f;

static constexpr std::array cube_vertices_indexed{
    Vertex{ .pos = { -side, -side, -side }, .color = { 1, 0, 0, 1 } },
    Vertex{ .pos = { side, -side, -side }, .color = { 1, 1, 0, 1 } },
    Vertex{ .pos = { -side, side, -side }, .color = { 1, 1, 1, 1 } },
    Vertex{ .pos = { side, side, -side }, .color = { 0, 1, 0, 1 } },
    Vertex{ .pos = { -side, -side, side }, .color = { 0, 0, 0, 1 } },
    Vertex{ .pos = { side, -side, side }, .color = { 0, 1, 1, 1 } },
    Vertex{ .pos = { -side, side, side }, .color = { 0, 0, 1, 1 } },
    Vertex{ .pos = { side, side, side }, .color = { 0, 1, 1, 1 } },
};

// clang-format off
static constexpr inline std::array<uint16_t, 36> cube_indices{
    0, 2, 1,    2, 3, 1,
    1, 3, 5,    3, 7, 5,
    2, 6, 3,    3, 6, 7,
    4, 5, 7,    4, 7, 6,
    0, 4, 2,    2, 4, 6,
    0, 1, 4,    1, 5, 4
};


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
