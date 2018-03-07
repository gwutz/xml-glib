#include <glib.h>
#include "xml-address.h"
#include "xml-addresslist.h"
#include "xml-glib.h"

const gchar *address_xml_text =
  "<?xml version=\"1.0\"?>\n"
  "<XmlAddress>\n"
  "  <house-number>4</house-number>\n"
  "  <street-name>Rohini</street-name>\n"
  "  <city>Delhi</city>\n"
  "</XmlAddress>\n";

const gchar *addresslist_xml_text =
  "<?xml version=\"1.0\"?>\n"
  "<XmlAddresslist>\n"
  "  <XmlAddress>\n"
  "    <house-number>4</house-number>\n"
  "    <street-name>Rohini</street-name>\n"
  "    <city>Delhi</city>\n"
  "  </XmlAddress>\n"
  "  <XmlAddress>\n"
  "    <house-number>14</house-number>\n"
  "    <street-name>Blaue-Lilien-Gasse</street-name>\n"
  "    <city>Regensburg</city>\n"
  "  </XmlAddress>\n"
  "</XmlAddresslist>\n";

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

void
test_addresslist_serialization ()
{
  XmlAddresslist *list = xml_addresslist_new ();

  XmlAddress *address = xml_address_new (4, "Rohini", "Delhi");
  xml_addresslist_add_address (list, address);
  address = xml_address_new (14, "Blaue-Lilien-Gasse", "Regensburg");
  xml_addresslist_add_address (list, address);

  xmlNodePtr node = xml_gobject_serialize (G_OBJECT (list));
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  xmlDocSetRootElement (doc, node);

  xmlChar *memorydump;

  xmlDocDumpFormatMemory (doc, &memorydump, NULL, 1);

  g_assert_cmpstr ((gchar *) memorydump, ==, addresslist_xml_text);
}

gint
main (gint   argc,
      gchar *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/xml/address/serialization", test_address_serialization);
  g_test_add_func ("/xml/addresslist/serialization", test_addresslist_serialization);

  return g_test_run ();
}
