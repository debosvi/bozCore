
#include "MyDBusClassServer.hpp"

namespace Morpho {
namespace MA5G {

const char *MyDBusClassServer::SERVER_NAME = "Morpho.MA5G.MyDBusClass";
const char *MyDBusClassServer::SERVER_PATH = "/Morpho/MA5G/MyDBusClass";

MyDBusClassServer::MyDBusClassServer(DBus::Connection& connection) : 
    DBus::ObjectAdaptor(connection, SERVER_PATH) {

}

MyDBusClassServer::~MyDBusClassServer() {

}

int32_t MyDBusClassServer::Random() {
    return 0;
}


} // namespace MA5G
} // namespace Morpho
