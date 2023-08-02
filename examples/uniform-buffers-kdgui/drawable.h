#pragma once
#include <wisdom/wisdom.h>
#include "graphics.h"
#include <glm/glm.hpp>


class Cube
{
public:
    Cube(const Graphics& gfx)
    {

    }

public:
    glm::mat4 transform;

    wis::RootSignature root;
    wis::PipelineState pipeline;
    wis::VertexBufferView vb;

    wis::Buffer vertex_buffer;
    wis::Buffer index_buffer;
    wis::Texture texture;
    
    wis::Buffer constant_buffer;
    wis::DescriptorSet constants_set;

    wis::Shader vs;
    wis::Shader ps;
};