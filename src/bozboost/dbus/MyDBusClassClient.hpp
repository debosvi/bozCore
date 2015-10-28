
#ifndef _MY_DBUS_CLASS_H_
#define _MY_DBUS_CLASS_H_

#include "MyDBusClass-proxy.hpp"

using namespace DBus;

namespace Morpho {
namespace MA5G {

class MyDBusClassClient : 
    public Morpho::MA5G::MyDBusClass_proxy,
    public DBus::IntrospectableProxy,
    public DBus::ObjectProxy
{
public:
    MyDBusClassClient(DBus::Connection& connection, const char *path, const char *name);
    ~MyDBusClassClient();

private:

};

} // namespace MA5G
} // namespace Morpho

#endif // _MY_DBUS_CLASS_H_
