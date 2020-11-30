#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <string>
#include <vector>
#include <variant>
#include <iostream>

#include "project.h"

class UsbDevice 
{
public:
    explicit UsbDevice(std::string _name, std::string _manufacturer, std::string _chipset, char _connector_type, float _usb_version, int _size_in_megabytes, bool _is_flash_based) : name(_name), manufacturer(_manufacturer), chipset(_chipset), connector_type(_connector_type), usb_version(_usb_version), size_in_megabytes(_size_in_megabytes), is_flash_based(_is_flash_based) {};

    std::string get_name() const { return name; };
    std::string get_manufacturer() const { return manufacturer; };
    std::string get_chipset() const { return chipset; };
    char get_connector_type() const { return connector_type; };
    float get_usb_version() const { return usb_version; };

    int get_size_in_megabytes() const { return size_in_megabytes; };
    bool get_is_flash_based() const { return is_flash_based; };

    inline bool operator<(const UsbDevice &dev) const {
        return get_size_in_megabytes() < dev.get_size_in_megabytes();
    }

    inline bool operator>(const UsbDevice &dev) const {
        return get_size_in_megabytes() > dev.get_size_in_megabytes();
    }

    inline bool operator<=(const UsbDevice &dev) const {
        return get_size_in_megabytes() <= dev.get_size_in_megabytes();
    }

    inline bool operator>=(const UsbDevice &dev) const {
        return get_size_in_megabytes() >= dev.get_size_in_megabytes();
    }

private:
    std::string name = "";
    std::string manufacturer = "";
    std::string chipset = "";
    char connector_type = 'e'; // a b or c
    float usb_version = 0.0f; // 2.0, 3.0, 3.1, 3.12, etc

    int size_in_megabytes = 0;
    bool is_flash_based = false;
};

inline std::ostream& operator<<(std::ostream &os, const UsbDevice &dev) {
    std::string flashString;
    if (dev.get_is_flash_based())
        flashString = "Flash Memory";

    return os << "Name: " << dev.get_manufacturer() << ' ' << dev.get_name() << std::endl
            << "Interface: " << dev.get_connector_type() << ' ' << std::to_string(dev.get_usb_version()) << std::endl
            << "Chipset: " << dev.get_chipset() << std::endl
            << std::to_string(dev.get_size_in_megabytes()) << " MB " << flashString;
}


class DataModel : Project 
{
public:
    explicit DataModel();

    void sort_usb_devices_std(std::vector<UsbDevice> &dev_list);
    void sort_usb_devices_bubble(std::vector<UsbDevice> &dev_list);
    void sort_usb_devices_quick(std::vector<UsbDevice> &dev_list, int start, int end);
    void sort_usb_devices_quick(std::vector<UsbDevice> &dev_list);
    void sort_usb_devices_merge(std::vector<UsbDevice> &dev_list);
    void run() override;

private:
    std::vector<UsbDevice> usb_devices;
};


#endif // DATAMODEL_H