project('depth-raytracing', 'c',
  meson_version: '>= 0.40.0',
)

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

sources = [
#    'matrix.c',
    'ray.c',
    'voxelGrid.h',
#    'vector.c'
]

dependencies = [
    m_dep
]

executable('depth-raytracing', sources,
  dependencies: dependencies,
  install: true,
)

