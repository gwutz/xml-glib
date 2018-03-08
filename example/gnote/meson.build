gnote_sources = [
  'main.c',
  'gn-note.c',
  'gn-text.c',
]

gnote_example_deps = [
  dependency('glib-2.0', version: '>= 2.50'),
  dependency('gobject-2.0'),
  dependency('libxml-2.0'),
]

executable ('gnote-example',
            gnote_sources,
            link_with: [xml_glib_lib],
            include_directories: [xml_glib_inc],
            dependencies: xml_glib_deps,
            )