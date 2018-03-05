subdir('xml')
gnome = import('gnome')

private_headers = [
  'xml-gobject-private.h',
]

glib_prefix = dependency('glib-2.0').get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')
docpath = join_paths(xml_datadir, 'gtk-doc', 'html')

gnome.gtkdoc('xml-glib',
             main_xml: 'xml-glib-docs.xml',
             src_dir: [
               join_paths(meson.source_root(), 'xml-glib'),
               join_paths(meson.build_root(), 'xml-glib'),
             ],
             dependencies: xml_glib_dep,
             gobject_typesfile: 'xml-glib.types',
             scan_args: [
               '--rebuild-types',
               '--ignore-headers=' + ' '.join(private_headers),
             ],
             fixxref_args: [
               '--html-dir=@0@'.format(docpath),
               '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
               '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
               '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
             ],
             install: true)