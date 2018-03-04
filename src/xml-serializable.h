#pragma once

#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

#define XML_TYPE_SERIALIZABLE (xml_serializable_get_type ())

G_DECLARE_INTERFACE (XmlSerializable, xml_serializable, XML, SERIALIZABLE, GObject)

struct _XmlSerializableInterface
{
  GTypeInterface parent;

  xmlNodePtr (* serialize_property)   (XmlSerializable *serializable,
                                       const gchar     *property_name,
                                       const GValue    *value,
                                       GParamSpec      *pspec);
  gboolean   (* deserialize_property) (XmlSerializable *serializable,
                                       const gchar     *property_name,
                                       GValue          *value,
                                       GParamSpec      *pspec,
                                       xmlNodePtr       node);
};

xmlNodePtr xml_serializable_serialize_property (XmlSerializable *serializable,
                                              const gchar     *property_name,
                                              const GValue    *value,
                                              GParamSpec      *pspec);

gboolean xml_serializable_deserialize_property (XmlSerializable *serializable,
                                                const gchar     *property_name,
                                                GValue          *value,
                                                GParamSpec      *pspec,
                                                xmlNodePtr       node);
G_END_DECLS
