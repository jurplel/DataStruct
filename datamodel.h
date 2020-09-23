#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <string>
#include <vector>
#include <variant>
#include <iostream>

#include "project.h"

struct UsbDevice 
{
public:
    std::string name;
    std::string manufacturer;
    std::string chipset;
    char connector_type; // a b or c
    float usb_version; // 2.0, 3.0, 3.1, 3.12, etc

    struct Storage 
    { 
        int size_in_megabytes;
        bool is_flash_based;
    };
    struct Wifi 
    { 
        std::vector<char> protocols;
        int antenna_count;
    };
    struct Bluetooth 
    {
        float bt_version;
        bool is_le;
    };
    struct Audio 
    {
        bool is_trrs;
    };
    struct Peripheral 
    {
        int polling_rate;
    };
    struct PowerDelivery 
    {
        bool is_outgoing; // if false, power is incoming
        int wattage;
    };
    struct Other {};

    std::variant<Storage,
        Wifi,
        Bluetooth,
        Audio,
        Peripheral,
        PowerDelivery,
        Other> device_type;
};

inline std::ostream& operator<<(std::ostream &o, const UsbDevice dev)
{
    std::string type_string;
    if (auto pval = std::get_if<UsbDevice::Storage>(&dev.device_type)) {
        std::string flashString;
        if (pval->is_flash_based)
            flashString = "Flash Memory";
        type_string = std::to_string(pval->size_in_megabytes) + " MB " + flashString;
    } else if (auto pval = std::get_if<UsbDevice::Wifi>(&dev.device_type)) {
        type_string = std::to_string(pval->antenna_count) + " Antenna(s) ";
        for (const auto &protocol : std::as_const(pval->protocols)) {
            type_string += protocol;
            type_string += '/';
        }
    } else if (auto pval = std::get_if<UsbDevice::Bluetooth>(&dev.device_type)) {
        type_string = "Bluetooth " + std::to_string(pval->bt_version);
        if (pval->is_le)
            type_string += " w/ LE";
    } else if (auto pval = std::get_if<UsbDevice::Audio>(&dev.device_type)) {
        type_string = "TRRS: ";
        if (pval->is_trrs) {
            type_string += "true";
        } else {
            type_string += "false";
        }
    } else if (auto pval = std::get_if<UsbDevice::Peripheral>(&dev.device_type)) {
        type_string = std::to_string(pval->polling_rate) + " Hz";
    } else if (auto pval = std::get_if<UsbDevice::PowerDelivery>(&dev.device_type)) {
        type_string = "Mode: ";
        if (pval->is_outgoing) {
            type_string += "Sending ";
        } else {
            type_string += "Receiving ";
        }
        type_string += std::to_string(pval->wattage);
        type_string += " W";
    } else if (auto pval = std::get_if<UsbDevice::Other>(&dev.device_type)) {
        type_string = "Device details unknown";
    }

    return o << "Name: " << dev.manufacturer << ' ' << dev.name << std::endl
            << "Connector: " << dev.connector_type << ' ' << std::to_string(dev.usb_version) << std::endl
            << "Chipset: " << dev.chipset << std::endl
            << type_string;
}

class DataModel : Project 
{
public:
    explicit DataModel();

    void sort_usb_devices(std::vector<UsbDevice> &dev_list);
    void run() override;

private:
    std::vector<UsbDevice> usb_devices;
};


#endif // DATAMODEL_H