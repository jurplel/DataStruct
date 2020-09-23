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

    int size_in_megabytes;
    bool is_flash_based;
};

inline std::ostream& operator<<(std::ostream &o, const UsbDevice dev)
{
    std::string flashString;
    if (dev.is_flash_based)
        flashString = "Flash Memory";

    return o << "Name: " << dev.manufacturer << ' ' << dev.name << std::endl
            << "Connector: " << dev.connector_type << ' ' << std::to_string(dev.usb_version) << std::endl
            << "Chipset: " << dev.chipset << std::endl
            << std::to_string(dev.size_in_megabytes) << " MB " << flashString;
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