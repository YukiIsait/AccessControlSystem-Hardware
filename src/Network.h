#pragma once
#include <Ethernet.h>
#include "Storage.h"
#include "NString.h"
#include <optional>

class ManagementNetwork {
public:
    struct Configuration: public Printable {
        IPAddress server;
        uint16_t port;

        uint8_t mac[6];
        IPAddress ip;
        IPAddress subnet;
        IPAddress gateway;
        IPAddress dns;

        String id;

        virtual size_t printTo(Print& p) const;

        Configuration();
        Configuration(Storage::Config& config);
    };

    ManagementNetwork();
    ManagementNetwork(const Configuration& config);

    void begin();
    const Configuration& getConfiguration();
    void setConfiguration(const Configuration& config);
    void access(const String& userId);
    std::optional<bool> poll();

private:
    Configuration config;
    EthernetClient client;
    const char* payload = "GET /device/accessibility/access?deviceId=%s&userId=%s HTTP/1.0\r\nConnection: close\r\n\r\n";
};
