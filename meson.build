project('Engine', 'c', 'cpp', version: '0.1', default_options: ['default_library=static', 'cpp_std=c++23', 'c_std=c23', 'prefer_static=true'])

# Specify source files
source_files = files('SpriteLight/src/main.c', 'SpriteLight/src/glad.c', 'SpriteLight/src/resource_mgr.c', 'SpriteLight/src/definitions.c', 'SpriteLight/src/shader.c', 'SpriteLight/src/texture.c', 'SpriteLight/src/lights.c', 'SpriteLight/src/input.c', 'SpriteLight/src/buffers.c', 'SpriteLight/src/bloom.c', 'SpriteLight/src/init.c', 'SpriteLight/src/update.c', 'SpriteLight/src/draw.c')

# Define dependencies
cglm_dep = dependency('cglm')
freetype_dep = dependency('freetype2')
b_sanitize = ['address', 'undefined']

# Define shared library
shared_lib = shared_library('sprite_light_shared', source_files,
    dependencies : [cglm_dep, freetype_dep],
    c_args : ['-ggdb', '-Bdynamic', '-Wno-switch'],
    link_args : ['-lm', '-lmsdfgl', 'lSDL3', '-lpthread'],
    include_directories: include_directories('SpriteLight/include'),
)

# Define static library
static_lib = static_library('sprite_light_static', source_files,
    dependencies : [cglm_dep, freetype_dep],
    c_args : ['-ggdb', '-Bdynamic', '-Wno-switch'],
    link_args : ['-lm', '-lmsdfgl', '-lSDL3', '-lpthread'],
    include_directories: include_directories('SpriteLight/include'),
)

# Define executables
executable('engine_test_shared',
    'src/main.c',
    link_with : shared_lib,
    c_args : ['-ggdb', '-Wno-missing-braces'],
    link_args : ['-lSDL3', '-lmsdfl'],
    dependencies : [cglm_dep, freetype_dep],
    include_directories : include_directories('include')
)

executable('engine_test_static',
    'src/main.c',
    link_with : static_lib,
    c_args : ['-ggdb', '-Wno-missing-braces'],
    link_args : ['-lSDL3', '-lmsdfgl'],
    dependencies : [cglm_dep, freetype_dep],
    include_directories : include_directories('include')
)
