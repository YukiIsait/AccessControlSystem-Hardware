#include "Network.h"

size_t ManagementNetwork::Configuration::printTo(Print& p) const {
    size_t n = 0;
    n += p.print("MAC: ");
    for (int i = 0; i < 6; i++) {
        n += p.print(mac[i], HEX);
        if (i < 5) {
            n += p.print(':');
        } else {
            n += p.println();
        }
    }
    n += p.print("IP: ");
    n += p.println(ip);
    n += p.print("Subnet: ");
    n += p.println(subnet);
    n += p.print("Gateway: ");
    n += p.println(gateway);
    n += p.print("DNS: ");
    n += p.println(dns);
    n += p.print("Server: ");
    n += p.println(server);
    n += p.print("Port: ");
    n += p.println(port);
    n += p.print("ID: ");
    n += p.print(id);
    return n;
}
ManagementNetwork::Configuration::Configuration() {}
ManagementNetwork::Configuration::Configuration(Storage::Config& config): server(config.server[0], config.server[1], config.server[2], config.server[3]),
                                                                          port(config.port),
                                                                          mac { config.mac[0], config.mac[1], config.mac[2], config.mac[3], config.mac[4], config.mac[5] },
                                                                          ip(config.ip[0], config.ip[1], config.ip[2], config.ip[3]),
                                                                          subnet(config.subnet[0], config.subnet[1], config.subnet[2], config.subnet[3]),
                                                                          gateway(config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3]),
                                                                          dns(config.dns[0], config.dns[1], config.dns[2], config.dns[3]),
                                                                          id(NString(reinterpret_cast<char*>(config.id), config.idSize)) {}

ManagementNetwork::ManagementNetwork() {}
ManagementNetwork::ManagementNetwork(const Configuration& config): config(config) {}

void ManagementNetwork::begin() {
    Ethernet.begin(config.mac, config.ip, config.dns, config.gateway, config.subnet);
    while (Ethernet.hardwareStatus() == EthernetNoHardware ||
           Ethernet.linkStatus() == LinkOFF) {
        delay(1000);
    }
}

const ManagementNetwork::Configuration& ManagementNetwork::getConfiguration() {
    return config;
}

void ManagementNetwork::setConfiguration(const Configuration& config) {
    this->config = config;
}

void ManagementNetwork::access(const String& userId) {
    while (!client.connect(config.server, config.port)) {
        delay(1000);
    }
    client.printf(payload, config.id.c_str(), userId.c_str());
    client.flush();
}

std::optional<bool> ManagementNetwork::poll() {
    if (client.status()) {
        if (client.available()) {
            String response = client.readString();
            if (response.indexOf("\"status\":200") != -1) {
                return response.indexOf("\"data\":true") != -1;
            }
        }
        if (!client.connected()) {
            client.stop();
        }
    }
    return std::nullopt;
}
