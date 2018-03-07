xml_glib_deps = [
  dependency('glib-2.0', version: '>= 2.50'),
  dependency('gobject-2.0'),
  dependency('libxml-2.0'),
]

tests = [
  'simple-serialize',
  'simple-deserialize',
  'serializable',
]

foreach t: tests

  exe = executable (t, ['@0@.c'.format(t), 'simple-gobject.c'],
    link_with: [xml_glib_lib],
    include_directories: [xml_glib_inc],
    dependencies: xml_glib_deps,
    install: true,
  )

  test(t, exe)
endforeach

address_tests = [
  'address-test',
]

foreach t: address_tests

  exe = executable (t, ['@0@.c'.format(t), 'xml-address.c', 'xml-addresslist.c'],
    link_with: [xml_glib_lib],
    include_directories: [xml_glib_inc],
    dependencies: xml_glib_deps,
    install: true,
  )

  test(t, exe)
endforeach