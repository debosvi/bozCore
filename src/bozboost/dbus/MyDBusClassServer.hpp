
#ifndef _MY_DBUS_CLASS_H_
#define _MY_DBUS_CLASS_H_

#include "MyDBusClass-adaptor.hpp"

using namespace DBus;

namespace Morpho {
namespace MA5G {

class MyDBusClassServer : 
    public Morpho::MA5G::MyDBusClass_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor 
{
public:
    MyDBusClassServer(DBus::Connection& connection);
    ~MyDBusClassServer();

    virtual int32_t Random();

    static const char *SERVER_NAME;
    static const char *SERVER_PATH;

private:

};

} // namespace MA5G
} // namespace Morpho

#endif // _MY_DBUS_CLASS_H_
