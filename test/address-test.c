#include <glib.h>
#include "xml-address.h"
#include "xml-glib.h"

const gchar *address_xml_text =
  "<?xml version=\"1.0\"?>\n"
  "<XmlAddress>\n"
  "  <house-number>4</house-number>\n"
  "  <street-name>Rohini</street-name>\n"
  "  <city>Delhi</city>\n"
  "</XmlAddress>\n";

void
test_address_serialization ()
{
  XmlAddress *address = xml_address_new (4, "Rohini", "Delhi");

  xmlNodePtr node = xml_gobject_serialize (G_OBJECT (address));
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  xmlDocSetRootElement (doc, node);

  xmlChar *memorydump;

  xmlDocDumpFormatMemory (doc, &memorydump, NULL, 1);

  g_assert_cmpstr ((gchar *)memorydump, ==, address_xml_text);
}

gint
main (gint   argc,
      gchar *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/xml/address/serialization", test_address_serialization);

  return g_test_run ();
}
