
#include "MyDBusClassClient.hpp"

namespace Morpho {
namespace MA5G {

MyDBusClassClient::MyDBusClassClient(DBus::Connection& connection, const char *path, const char *name) : 
    DBus::ObjectProxy(connection, path, name) {

}

MyDBusClassClient::~MyDBusClassClient() {

}

} // namespace MA5G
} // namespace Morpho
