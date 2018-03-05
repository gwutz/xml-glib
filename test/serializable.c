#include <glib-object.h>
#include <glib/gprintf.h>
#include "xml-glib.h"
#include "simple-gobject.h"

#define G_OBJECT_TYPE_SERIALIZABLE (g_object_serializable_get_type ())
G_DECLARE_FINAL_TYPE (GObjectSerializable, g_object_serializable, G_OBJECT, SERIALIZABLE, GObject)

struct _GObjectSerializable
{
  GObject parent_instance;

  SimpleGobject *obj;
};

static void xml_serializable_iface_init (gpointer iface);

G_DEFINE_TYPE_WITH_CODE (GObjectSerializable, g_object_serializable, G_TYPE_OBJECT,
               G_IMPLEMENT_INTERFACE (XML_TYPE_SERIALIZABLE, xml_serializable_iface_init))

enum {
  PROP_0,
  PROP_SIMPLE_GOBJECT,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

GObjectSerializable *
g_object_serializable_new (void)
{
  SimpleGobject *child = simple_gobject_new (100, "Serialize Example");
  return g_object_new (G_OBJECT_TYPE_SERIALIZABLE, "simple-gobject", child, NULL);
}

static void
g_object_serializable_finalize (GObject *object)
{
  GObjectSerializable *self = (GObjectSerializable *)object;

  g_clear_object (&self->obj);

  G_OBJECT_CLASS (g_object_serializable_parent_class)->finalize (object);
}

static void
g_object_serializable_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  GObjectSerializable *self = G_OBJECT_SERIALIZABLE (object);

  switch (prop_id)
    {
    case PROP_SIMPLE_GOBJECT:
      g_value_set_object (value, self->obj);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
g_object_serializable_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  GObjectSerializable *self = G_OBJECT_SERIALIZABLE (object);

  switch (prop_id)
    {
    case PROP_SIMPLE_GOBJECT:
      g_clear_object (&self->obj);
      self->obj = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
g_object_serializable_class_init (GObjectSerializableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_object_serializable_finalize;
  object_class->get_property = g_object_serializable_get_property;
  object_class->set_property = g_object_serializable_set_property;

  properties[PROP_SIMPLE_GOBJECT] = g_param_spec_object ("simple-gobject", "SimpleGobject", "A simple object", SIMPLE_TYPE_GOBJECT, G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static xmlNodePtr
g_object_serializable_serialze_property (XmlSerializable *serializable,
                                         const gchar     *property_name,
                                         const GValue    *value,
                                         GParamSpec      *pspec)
{
  g_print ("%s\n", property_name);

  if (g_strcmp0 (property_name, "simple-gobject") == 0)
    {
      SimpleGobject *obj = g_value_get_object (value);
      gchar tmp[100];
      xmlNodePtr objnode = xmlNewNode (NULL, BAD_CAST "child");

      gint integer;
      gchar *string;
      g_object_get (obj, "integer", &integer, "string", &string, NULL);
      g_sprintf (tmp, "%d", integer);
      xmlNodePtr c1 = xmlNewDocNode (NULL, NULL, BAD_CAST "integer", BAD_CAST tmp);
      xmlNodePtr c2 = xmlNewDocNode (NULL, NULL, BAD_CAST "string", BAD_CAST string);

      xmlAddChild (objnode, c1);
      xmlAddChild (objnode, c2);
      return objnode;
    }

  return xml_serializable_default_serialize_property (serializable, property_name, value, pspec);
}

gboolean
g_object_serializable_deserialze_property (XmlSerializable *serialize,
                                           const gchar     *property_name,
                                           GValue          *value,
                                           GParamSpec      *pspec,
                                           xmlNodePtr       node)
{

  return FALSE;
}

const gchar *
g_object_serializable_get_root_name (XmlSerializable *serializable)
{
  return "TestObject";
}

static void
xml_serializable_iface_init (gpointer g_iface)
{
  XmlSerializableInterface *iface = g_iface;

  iface->serialize_property = g_object_serializable_serialze_property;
  iface->deserialize_property = g_object_serializable_deserialze_property;
  iface->get_root_name = g_object_serializable_get_root_name;
}

static void
g_object_serializable_init (GObjectSerializable *self)
{
}

void
test_serializable ()
{
  GObjectSerializable *obj = g_object_serializable_new ();

  xmlNodePtr node = xml_gobject_serialize (G_OBJECT (obj));
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  xmlDocSetRootElement (doc, node);

  xmlDocFormatDump (stdout, doc, 1);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/xml/serializable", test_serializable);

  return g_test_run ();
}
