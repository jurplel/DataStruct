#include "datamodel.h"

#include <algorithm>

DataModel::DataModel()
{

}

void DataModel::sort_usb_devices(std::vector<UsbDevice> &dev_list)
{
    std::sort(std::begin(dev_list), std::end(dev_list));
}

void DataModel::run()
{
    UsbDevice bt_adapter;
    bt_adapter.name = "BT-400";
    bt_adapter.manufacturer = "Premiertek";
    bt_adapter.chipset = "Broadcom BCM20702";
    bt_adapter.connector_type = 'A';
    bt_adapter.usb_version = 2.0f;

    UsbDevice::Bluetooth btdt;
    btdt.bt_version = 4.0;
    btdt.is_le = true;

    bt_adapter.device_type = btdt;
    usb_devices.push_back(bt_adapter);

    UsbDevice sandisk_usb_drive = {
        "Cruzer",
        "Sandisk",
        "",
        'A',
        2.0f,
        UsbDevice::Storage {
            58704,
            true
        }
    };
    usb_devices.push_back(sandisk_usb_drive);

    usb_devices.push_back({
        "F9L1001v1",
        "Belkin",
        "Realtek RTL8188SU",
        'A',
        2.0f,
        UsbDevice::Wifi {
            {'n', 'g', 'b'},
            0
        }
    });

    usb_devices.push_back({
        "G502 HERO SE",
        "Logitech",
        "Logitech Hero16k",
        'A',
        2.0f,
        UsbDevice::Peripheral {
            1000
        }
    });

    usb_devices.push_back({
        "ANHA-011PW",
        "Essential Products",
        "",
        'C',
        0.0,
        UsbDevice::Audio {
            true
        }
    });

    usb_devices.push_back({
        "",
        "Micro Center",
        "Toshiba MLC",
        'A',
        3.0f,
        UsbDevice::Storage {
            15112,
            true
        }
    });

    for (const auto &device : std::as_const(usb_devices))
    {
        std::cout << device << std::endl;
        std::cout << "------" << std::endl;
    }
}