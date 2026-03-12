
// Basic compute fill the buffer with a color
RWStructuredBuffer<float4> outputBuffer : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (index >= 1024) return; // Avoid out of bounds
    // Fill the buffer with a color (e.g., red)
    outputBuffer[index] = float4(1.0, 0.0, 0.0, 1.0); // RGBA
}