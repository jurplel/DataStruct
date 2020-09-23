#include "datamodel.h"

#include <algorithm>

DataModel::DataModel()
{

}

static void print_all_devices(std::vector<UsbDevice> &dev_list)
{
    for (const auto &device : std::as_const(dev_list))
    {
        std::cout << device << std::endl;
        std::cout << "------" << std::endl;
    }
}

void DataModel::sort_usb_devices(std::vector<UsbDevice> &dev_list)
{
    std::sort(std::begin(dev_list), std::end(dev_list), [](UsbDevice dev0, UsbDevice dev1){
        return dev0.get_size_in_megabytes() < dev1.get_size_in_megabytes();
    });
}

void DataModel::run()
{
    UsbDevice drive0(
        "Cruzer",
        "Sandisk",
        "",
        'A',
        2.0f,
        58704,
        true
    );
    usb_devices.push_back(drive0);

    UsbDevice drive1{
        "",
        "Micro Center",
        "Toshiba MLC",
        'A',
        3.0f,
        15112,
        true
    };
    usb_devices.push_back(drive1);

    UsbDevice drive2{
        "",
        "Dongcoh",
        "Western Digital",
        'A',
        3.0f,
        610352,
        true
    };
    usb_devices.push_back(drive2);
    

    print_all_devices(usb_devices);

    std::cout << "-----------------------------------------------\n";

    sort_usb_devices(usb_devices);

    print_all_devices(usb_devices);
}