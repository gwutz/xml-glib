api_version = '0.1'

xml_glib_sources = [
  'xml-gobject.c',
  'xml-serializable.c',
]

xml_glib_headers = [
  'xml-glib.h',
  'xml-gobject.h',
  'xml-serializable.h',
]

xml_glib_inc = include_directories('.')

version_split = meson.project_version().split('.')
MAJOR_VERSION = version_split[0]
MINOR_VERSION = version_split[1]
MICRO_VERSION = version_split[2]

version_conf = configuration_data()
version_conf.set('VERSION', meson.project_version())
version_conf.set('MAJOR_VERSION', MAJOR_VERSION)
version_conf.set('MINOR_VERSION', MINOR_VERSION)
version_conf.set('MICRO_VERSION', MICRO_VERSION)

configure_file(
  input: 'xml-glib-version.h.in',
  output: 'xml-glib-version.h',
  configuration: version_conf,
  install: true,
  install_dir: join_paths(get_option('includedir'), 'xml-glib')
)

xml_glib_deps = [
  dependency('glib-2.0', version: '>= 2.50'),
  dependency('gobject-2.0'),
  dependency('libxml-2.0'),
]

xml_glib_lib = shared_library('xml-glib-' + api_version,
  xml_glib_sources,
  dependencies: xml_glib_deps,
  install: true,
)

xml_glib_dep = declare_dependency(link_with: xml_glib_lib,
  include_directories: root_dir,
  dependencies: xml_glib_deps,
  )

install_headers(xml_glib_headers, subdir: 'xml-glib')

pkg = import('pkgconfig')

pkg.generate(
  description: 'A shared library for ...',
    libraries: xml_glib_lib,
         name: 'xml-glib',
     filebase: 'xml-glib-' + api_version,
      version: meson.project_version(),
      subdirs: 'xml-glib',
     requires: 'glib-2.0',
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)
