Implicit actions:

- If the you need to create or update structure/enum/variant that is located in generated/ directories:
- 1. Update the structure in .xml file that is named the same as extension folder: example video/generated/c_api -> video.xml
- 2. Run the generator target with xml name as argument: example: video.xml -> generator video

If the change is from core API, (under include folder) use generator without argument: include/generated/c_api -> generator

Set of rules to the code:

- Don't use STL containers. Only usage of wis::span and wis::unique_ptr is allowed. Occasional use of std::string_view is allowed.
- Don't use C++ exceptions.
- Don't use C++ RTTI (typeid, dynamic_cast, etc.).
- No virtual functions.
- API is C compatible, so all API facing functions must have C linkage WIS_EXTERN_C.
- Internal functions are allowed to use C++20 features, but not in header files.
- If several allocations are required, use a single allocation and place all data in it. This is to minimize the number of allocations and deallocations, which can be expensive.
- If allocation is required, try assuming the reasonable size of the buffer and use stack allocation.
- If the buffer size is unknown, use a two-pass approach: first call the function with a null buffer to get the required size, then allocate the buffer and call the function again to fill it.
- If the allocation is absolutely required in command list api, use provided bump allocator, but try to avoid it as much as possible.
- If the allocation is required in class handle, use unique_ptr + release().
- If the class has allocation, fill in destroy function that will free the memory.