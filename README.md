# XML Serialization for GObject

This library is intended to make it easier to serialize and deserialize
GObject instances to XML files and back. It uses a similar serialization
strategy then JSON-glib. If you don't specify special properties, then it is
already capable to serialize without intervention. But you are able to
implement an interface for special properties like lists, arrays and so on.

xml-glib utilizes libxml2 tree structures to build up a xml representation
of your object. Therefore you have to interact with some structures from libxml2.
I plan to add a dump method to handly only a string without interacting with
libxml2. This is not decided at the moment.

## Installation

This library gets installed typical with

```
# git clone https://github.com/gwutz/xml-glib.git
# cd xml-glib
# mkdir build && cd build
# meson ..
# ninja
# ninja install
```

## Usage

The basic usage is to use 

```c
GObject *myobj = my_super_object_new ();

xmlNodePtr root = xml_gobject_serialize (myobj);
xmlDocPtr doc = xmlNewDoc ("1.0");
xmlDocSetRootElement (doc, root);

xmlDocDump (stdout, doc);
```

## Special properties

If you want to control the output or you have special properties like a List
of items, we have to implement the **XmlSerializable** interface to convert
the property correctly (C cannot guess which pointers getting stored in a
linked list - so we have to tell him)

```c
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
  const gchar *(* get_root_name)      (XmlSerializable *serializable);
};
```

The `serialize_property` method gets used if the interface is implemented.
For standard properties you can still use the default behaviour from xml-glib.
Just call `xml_serializable_default_serialize_property` at the end of `serialize_property`
if you filtered your special properties. A typical implementation could look
like (you can find the whole example under test/xml-addresslist.c/.h and address-test.c):

```c
static xmlNodePtr
xml_addresslist_serialize_property (XmlSerializable *serializable,
                                    const gchar     *property_name,
                                    const GValue    *value,
                                    GParamSpec      *pspec)
{
  if (g_strcmp0 (property_name, "addresses") == 0) {
    GPtrArray *arr = g_value_get_boxed (value);
    xmlNodePtr first;

    for (int i = 0; i < arr->len; i++) {
      XmlAddress *a = g_ptr_array_index (arr, i);

      xmlNodePtr n = xml_gobject_serialize (G_OBJECT (a));
      if (i == 0) first = n;
      else xmlAddSibling (first, n);
    }

    return first;
  }

  return xml_serializable_default_serialize_property (serializable, property_name, value, pspec);
}
```

