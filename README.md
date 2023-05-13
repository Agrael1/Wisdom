# Wisdom

**Low-level thin Graphics API layer**

# Why?

This library has several tasks to accomplish:

- Make a functional portable library for graphics programming without extra efforts in usage
- Make use of API similarities and provide implementation agnostic interface to the user for seamless experience
- Make transition to this library and making extensions seamless with existing codebase

# Details

The API is designed for game-ready and heavy computations use. It is done in low-level, with customizable logs. Underlying implementation is pure classes with minimal encapsulation.

The API is structured like this:
 - The basic types are defined, depending on platform of choice. They are **Factory**, **Adapter**, **Device** etc. They are directly implemented, this eliminates memory indirection and potential cache misses.
 - The the platform is selected the most suitable to the system: Windows - DirectX 12, MacOS - Metal __[TBD]__
 - All calls are done directly, without usage of interfaces/virtual functions. This eliminates call indirection and the projection is direct as if you wrote the code directly inside your functions.
 - Underlying accessibility, all of the internals are accessible using `GetInternal()` and can be used to bridge functionality or to create extensions. All the internal state is immutable for the stability of work between library and extensions. However it's not advised to use internal state directly, since it is platform dependent.

Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system with explicit types `wis::VKFactory`, `wis::VKDevice` etc. Vulkan can also be manually selected upon build with `FORCE_VULKAN` option on CMake configuration.

# Build

This is a CMake project, all the plugins are ensured to download beforehand, so it's enough to just configure the project, everything is going to be downloaded with respect to platform.
The later reconfigurations are not reloading the plugins for easy expansion of the library, but if the plugin reload is required, the cache deletion should be done, or change `PLUGINS_LOADED` CMakeCache entry to `FALSE`.

Right now the build works only on Windows, but others will be available soon.

# Roadmap and tasks

The project has trello page. Further directions are appreciated.
https://trello.com/b/qjrv97Bc/wisdom-veritas

