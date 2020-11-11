

luminous_render_add_plugin(constant_float3 CATEGORY texture SOURCES constant_texture.cpp)
target_compile_definitions(luminous-texture-constant_float3 PRIVATE FLOAT3)

luminous_render_add_plugin(constant_float4 CATEGORY texture SOURCES constant_texture.cpp)
target_compile_definitions(luminous-texture-constant_float4 PRIVATE FLOAT4)

