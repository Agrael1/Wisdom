# Version History

- 0.6.7 Modules

  - Added modules for Wisdom and extensions
  - Modules are buildable by both windows and linux
  - Wisdom can now be built using Clang, GCC throws an error with the modules
  - Added clang build to the CI
  - Removed duplicate cmake files with functions
  - Replaced FUNC macro with a consteval function
  - Fixed raytracing buffer sizes not being aligned

- 0.6.6

  - #220 Added constants query for Vulkan and DirectX12 devices
  - #215 Added copy instructions for acceleration structures and query for max recursion depth
  - moved wis::detail::aligned_size to wis::aligned_size and made it public, made it deal with other integral types, result is converted to the same type as the first argument
  - Added UUID export for Vulkan, DX12 uses LUID

- 0.6.4-0.6.5 Cleanup

  - BREAKING: BeginRenderPass now accepts reference instead of pointer to the render pass. This is done to make the API more consistent with other methods.
  - Removed unnecessary defines that collided with other libraries
  - Target for header only library is now `wis::headers` instead of `wis::header-only`
  - Fixed DX12 Index buffer offset overflow the buffer size
  - Fixed Linker errors with Vulkan Raytracing
  - Fixed DX12 RTAS not being written past binding 0

- 0.6.3 Swapchain extension

  - Added support for VK_EXT_swapchain_maintenance1 extension
  - Added scaling to the swapchain creation. Default is 1:1

- 0.6.1 - 0.6.2 Nuget package update

  - Fixed Vulkan headers in the Nuget package

- 0.6.0 Raytracing

  - BREAKING: Revisited root signature and descriptor storage. Now it is possible to create roots with several same types of descriptors, binding tables are ordered accordingly.
  - BREAKING: Root signature compatibility is now based on the input `DescriptorBindingDesc`. It is possible to create a root signature with different descriptor types, but that may result in incompatibility between root and descriptor storage.
  - Added Raytracing support for DXR and VK_KHR_ray_tracing
  - Added compute pipeline and compute functions to the device and command list
  - Added more bindings to the Descriptor storage

- 0.5.0 API stabilization

  - Most of the API is now stable and will not change
  - There are 2 versions of return values. One is the ResultValue<> which was there from the beginning, and the other is RVO enabled return value. ResultValue<> is left for compatibility and local use, while RVO enabled return value is for inline construction and chaining. It is recommended to use RVO enabled return value for all new code.
  - Added documentation for extended allocation. With Vulkan 1.4 it has become a core feature.
  - API structure is consistent. Now all the structures are passed by const reference. This is done to enable language interop.
  - Consumed headers are now .hpp instead of .h. Implementation headers are still .h, since they are not intended to be used standalone anyway.

- 0.4.0 Major Update

  - Fixed Vulkan Swapchain on resize occasionally result in error messages
  - Revisited Examples and made them use SDL3 for window creation
  - Added common library for examples
  - Added Multiview support with example
  - Examples are now stored in examples/bin folder with compiled shaders and resources
  - Added support for Xlib without XCB for Vulkan
  - Moved DescriptorBuffer to its own extension. It is now only possible to use if both VK_EXT_mutable_descriptor_type and VK_EXT_descritpor_buffer are enabled
  - Added support for VK_EXT_descriptor_indexing and created DescriptorStorage for bindless descriptors. This is now the default way of creating descriptor tables
  - Lifted the restriction of mandatory VK_EXT_mutable_descriptor_type and VK_EXT_descritpor_buffer for Vulkan. It is now possible to use Wisdom with Vulkan 1.2 (with other mandatory extensions of course)
  - Renamed RootConstants to PushConstants because it is more common name
  - Added support for push descriptors. Maximum number of push descriptors is 8 for now.
  - Added new CreateRootSignature method for DX12 and Vulkan. It is now possible to create a root signatures with non-uniform spacing.

- Bugfixes:

  - Fixed textures not being destroyed correctly in DX12 on replacement (texture1 = std::move(texture2)) was resulting in leaks)
  - Fixed DX12 copy texture not working correctly with offsets

- 0.3.15

  - Fixed Vulkan default texture barriers
  - Fixed static analysis warning for blending enum, thanks to @jaw

- 0.3.14

  - Finally fixed swapchain stutters on VSync for Vulkan, thanks to Sascha Willems for the ship of theseus example
  - Fixed Validation on swapchain resize for Vulkan
  - Removed unnecessary fences for Vulkan swapchain, as well as dynamic VSync. It was bothering me for a long time, but now it is gone.

- 0.3.13

  - Fixed swapchain stability for Vulkan (in some cases Waiting for image was not finished before aquire next image)
  - Fixed CommandQueue WaitQueue for Vulkan not working correctly (wrong pointers)
  - Reverted change to DX12 command list reset, now it will reset the allocator again (Vulkan does not support resetting on-the-fly)

- 0.3.12

  - Hotfix for Vulkan not creating the pipeline correctly

- 0.3.11

  - Resolved inconsistencies with API generation of struct members (ignoring const)
  - Added offset member to the Vertex buffer binding. Default is 0.
  - Fixed DX12 Unbounded descriptor array feature not being documented and not working correctly
  - Added forgotten dynamic rendering to Vulkan pipeline creation
  - Added unbounded descriptor array feature to Vulkan
  - Vulkan now supports unused render attachments if possible
  - Added forgotten setting pipeline state call to both command list APIs
  - Removed brutal reset from DX12 command list, now it will not reset the allocator which may have lead to UB if list was executed currently
  - Subresource ranges were annoying part of the TextureBarrier api. Now if left empty, it will be filled with the whole resource.

- 0.3.7-0.3.10

  - Fixed annoying warning about class/struct mismatch in C++ API
  - Fixed render pass for DX12 always clearing, despite of value
  - Added specific function for creating depth stencil views
  - Fixed missing DXGI lib dependency for DX12 under NuGet

- 0.3.6

  - Fixed inconsistency with Descriptor tables (bytes vs descriptors)
  - Removed non-existing barrier access Present
  - Removed result value from the Descriptor Table device methods
  - Fixed examples
  - Preparing for refactoring of descriptor tables

- 0.3.5

  - Made internal function for swapchain recreation in Vulkan accept a pNext chain
  - Fixed cross-adapter example not finishing correctly
  - Fixed swapchain deadlocking on resize for Vulkan

- 0.3.4

  - Made a lot of views CPP API only, that makes C API more lightweight
  - Finished documentation for C and CPP APIs
  - Fixed Present2 for Vulkan not working correctly with moved swapchain
  - Added a present fence to the swapchain for Vulkan, makes resizing slightly faster, works if the DynamicVSync is enabled, otherwise it is ignored

- 0.3.3

  - Fixed Extension creation for C API yet again
  - Added device and allocator documentation for C and C++ API
  - C example now uses the new extension creation
  - Fixed several API functions having different names in C++ (AllocateTextureMemory, GetDescriptorTableAlignment)
  - Made functions use const correctness and accept const references where possible

- 0.3.2

  - Fixed Extension creation for C API
  - Added code generation for extension methods for C API

- 0.3.1

  - Added more functions to the C API
  - Test now iterates over all adapters in your system and creates a factory debug extension
  - Fixed bug with C CreateFactory crashing on extension creation
  - Fixed documentation not showing up on several C functions

- 0.3.0

  - Changed generation of the API
  - Added support for extensions in C API
  - Added documentation nodes for methods and functions for C API
  - Added documentation and staged inline functions for C++ API (only free for now, methods will come as separate generated classes)
  - CreateDevice and CreateFactory now have extensions and default arguments to them. No more need for 4 separate functions for each case.

- 0.2.8

  - Fixed shared fence for DX12
  - Shared fence is now explicit for all implementations (not all resources and fences are sharable, there is now a flag for that)
  - Updated readme, Wisdom is direct translation layer

- 0.2.7

  - Fixed shared heap allocations for DX12
  - MemoryFlags::Exportable is now limited to memory allocations. It is not possible to export resources directly (at least in Vulkan).

- 0.2.6

  - Added documentation for structures
  - Added Present level VSync swaitching without recreating the swapchain under DynamicVSync device feature

- 0.2.5

  - Added querying device features
  - Added documentation for bitmasks
  - Added force device creation. That allows the user to create a device with a GPU, that does not implement all core features.
  - Removed DX flags from adapter flags. Those may be queried directly with the internals.

- 0.2.4

  - Added index buffer support
  - Added VK_KHR_maintenance5 extension for Vulkan as an optional extension

- 0.2.3

  - Fixed bug with external memory + Host Copy buffer not allocating image correctly
  - Now exportable memory is explicit for all implementations (not all resources and memory are sharable, there is now a flag for that)

- 0.2.2

  - Fixed upload and readback buffer not being created with mapping flags with Vulkan
  - Discovered bug with external memory + Host Copy buffer not allocating image correctly

- 0.2.1

  - Fixed Extended allocation on Vulkan not loading functions correctly and DX12 throwing an exception on device destruction
  - Added basic documentation for enumerations, that is generated for C and CPP APIs

- 0.2.0

  - Added allocation info, O(1) memory allocation, and memory and semaphore API interop
  - Fixed the issue with AMD drivers not supporting wait for present

- 0.1.8

  - Fixed property pages for nuget

- 0.1.7

  - Added debug build to packages
  - Added additional macro definitions
  - Using CPack for Zip build

- 0.1.6

  - Update for extensions creation
  - Added convenient functions for fptr querying
  - Reduced presence of static FactoryGlobals
  - Fixed error with running out of memory on instance creation

- 0.1.3 - 0.1.4

  - Fixed layers and extensions for VkInstance not being unique
  - Fixed nuget package not being generated correctly

- 0.1.2

  - Added support for separate Depth and Stencil attachments

- 0.1.0

  - Refactored code to be more modular

- 0.0.1
  - Initial Release
