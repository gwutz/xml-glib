#include "xml-glib.h"
#include <stdio.h>
#include <string.h>
#include "simple-gobject.h"

const gchar *testxml =
  "<?xml version=\"1.0\"?>"
  "<SimpleGobject>"
  " <integer>50</integer>"
  " <string>Hello XML</string>"
  "</SimpleGobject>";

void
test_deserialize_simple ()
{
  xmlDocPtr doc = xmlParseMemory (testxml, strlen (testxml));
  xmlNodePtr root = xmlDocGetRootElement (doc);

  SimpleGobject *obj = SIMPLE_GOBJECT (xml_gobject_deserialize (SIMPLE_TYPE_GOBJECT, root));

  g_assert (obj != NULL);

  gint integer;
  gchar *string;
  g_object_get (obj, "integer", &integer, "string", &string, NULL);

  g_assert_cmpint (integer, ==, 50);
  g_assert_cmpstr (string, ==, "Hello XML");
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/xml/deserialize_simple", test_deserialize_simple);

  return g_test_run ();
}
