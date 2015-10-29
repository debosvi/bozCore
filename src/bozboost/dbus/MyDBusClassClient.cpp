
#include "MyDBusClassClient.hpp"

namespace Morpho {
namespace MA5G {

MyDBusClassClient::MyDBusClassClient(DBus::Connection& connection, const char *path, const char *name) : 
    DBus::ObjectProxy(connection, path, name) {

}

MyDBusClassClient::~MyDBusClassClient() {

}

void MyDBusClassClient::valueChanged(const std::string& source_name) {
    std::cerr << __PRETTY_FUNCTION__ << ", name: " << source_name << std::endl;
}

} // namespace MA5G
} // namespace Morpho
