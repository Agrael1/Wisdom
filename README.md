# Wisdom

**Low-level bloat free Graphics API layer**

# Details

The API is designed for game-ready and heavy computations use. It is done in low-level, with customizable logs.

The API has strong restrictions:
 - The basic types are defined, depending on platform. They are:
    - **Factory** 
    - **Adapter**
    - othres will be added later
 - The code always prioritizes the platform specific API: Windows - DirectX 12, MacOS - Metal [TBD]
  - No virtual functions/shared pointers/pointerfs to objects. This one is enforced due to incompatibility with each other e.g. `VKDevice` can't be created using `DX12Adapter`
 - Less indirection, extensive use of templates
 - Underlying accessibility, all of the internals are accessible using `GetInternal()` and can be used to bridge functionality or to create extensions.
 
Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system, but only in explicit manner, using explicit types `wis::VKFactory`, `wis::VKDevice` etc. [TBD]

 
# TBD
 - [ ] DirectX 12 implementation
 - [ ] Vulkan implementation
 - [ ] Metal implementation
 - [ ] Examples
