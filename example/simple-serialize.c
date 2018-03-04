
#include "xml-glib.h"
#include <stdio.h>
#include "simple-gobject.h"

void
test_serialize_simple ()
{
  xmlNodePtr cur = NULL;
  SimpleGobject *obj = simple_gobject_new (50, "Hello XML");

  xmlNodePtr node = xml_gobject_serialize (G_OBJECT (obj));

  g_assert_cmpstr ((gchar *)node->name, ==, "SimpleGobject");
  char *testdata[] = {"50", "Hello XML"};
  gint i = 0;
  for (cur = node->children; cur != NULL; cur = cur->next)
    {
      xmlChar *content = xmlNodeGetContent (cur);

      g_assert_cmpstr ((gchar *)content, ==, testdata[i]);
      i += 1;
    }
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/xml/serialize_simple", test_serialize_simple);

  return g_test_run ();
}
