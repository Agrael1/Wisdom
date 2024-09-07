# Version History

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
