# Version History

- 0.3.6

  - Fixed inconsistency with Descriptor tables (bytes vs descriptors)
  - Removed non-existing barrier access Present
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
