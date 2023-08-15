#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_state_builder.h>
#include <iostream>
#include <fstream>
#include <format>


void GenerateStructures(std::ostream& ostream)
{
    ostream << std::format(R"(
#ifndef WISDOM_STRUCTURES_H
#define WISDOM_STRUCTURES_H
#include <stdint.h>

// Stack allocated structures for Wisdom objects
typedef uint8_t wisVKCFactory[{}];
typedef uint8_t wisVKCAdapter[{}];
typedef uint8_t wisVKCDevice[{}];
typedef uint8_t wisVKCCommandQueue[{}];
typedef uint8_t wisVKCSwapChain[{}];
typedef uint8_t wisVKCCommandList[{}];
typedef uint8_t wisVKCFence[{}]; 
typedef uint8_t wisVKCBuffer[{}]; 
typedef uint8_t wisVKCRenderTargetView[{}];
typedef uint8_t wisVKCShader[{}];
typedef uint8_t wisVKCRootSignature[{}];
typedef uint8_t wisVKCPipelineState[{}];
typedef uint8_t wisVKCResourceAllocator[{}];
typedef uint8_t wisVKCGraphicsPipelineDesc[{}];
typedef uint8_t wisVKCVertexBufferView[{}];
typedef uint8_t wisVKCRenderPass[{}];
typedef uint8_t wisVKCDescriptorHeap[{}];
typedef uint8_t wisVKCDescriptorSetLayout[{}];
typedef uint8_t wisVKCDescriptorSet[{}];
typedef uint8_t wisVKCTexture[{}];
typedef uint8_t wisVKCDepthStencilView[{}];
typedef uint8_t wisVKCSampler[{}];

#endif // WISDOM_STRUCTURES_H
)", 
    sizeof(wis::VKFactory),
	sizeof(wis::VKAdapter),
	sizeof(wis::VKDevice),
	sizeof(wis::VKCommandQueue),
	sizeof(wis::VKSwapChain),
	sizeof(wis::VKCommandList),
	sizeof(wis::VKFence),
	sizeof(wis::VKBuffer),
	sizeof(wis::VKRenderTargetView),
	sizeof(wis::VKShader),
	sizeof(wis::VKRootSignature),
	sizeof(wis::VKPipelineState),
	sizeof(wis::VKResourceAllocator),
	sizeof(wis::VKGraphicsPipelineDesc),
	sizeof(wis::VKVertexBufferView),
	sizeof(wis::VKRenderPass),
	sizeof(wis::VKDescriptorHeap),
	sizeof(wis::VKDescriptorSetLayout),
	sizeof(wis::VKDescriptorSet),
	sizeof(wis::VKTexture),
	sizeof(wis::VKDepthStencilView),
	sizeof(wis::VKSampler)
);
}

int main(int argc, char** argv)
{
    std::ostream* ostream = nullptr;
    if (argc == 1) {
        ostream = &std::cout;
    } else if (argc == 2) {
        ostream = new std::ofstream(argv[1]);
    } else {
        std::cerr << "Usage: " << argv[0] << " [output file]\n";
        return 1;
    }
    if (!ostream || ostream->bad()) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }



    if (ostream != &std::cout) {
        delete ostream;
    }
    return 0;
}