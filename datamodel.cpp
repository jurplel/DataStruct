#include "datamodel.h"

#include <algorithm>
#include <chrono>

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

void DataModel::sort_usb_devices_std(std::vector<UsbDevice> &dev_list)
{
    std::sort(std::begin(dev_list), std::end(dev_list), [](UsbDevice dev0, UsbDevice dev1){
        return dev0 < dev1;
    });
}

void DataModel::sort_usb_devices_bubble(std::vector<UsbDevice> &dev_list)
{
    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        for (int i = 0; i < dev_list.size()-1; i++)
        {
            const auto &dev0 = dev_list[i];
            const auto &dev1 = dev_list[i+1];
            if (dev0 > dev1)
            {
                std::swap(dev_list[i], dev_list[i+1]);
                repeat = true;
            }
        }
    }
}

void DataModel::sort_usb_devices_quick(std::vector<UsbDevice> &dev_list, int start, int end)
{
    UsbDevice &pivot = dev_list[end];
    int split_point = start;

    for (int i = start+1; i < end; i++)
    {
        if (dev_list[i] < pivot)
        {
            std::swap(dev_list[i], dev_list[split_point]);
            split_point++;
        }
    }

    std::swap(pivot, dev_list[split_point]);
    if (end - start > 2)
    {
        sort_usb_devices_quick(dev_list, start, split_point);
        sort_usb_devices_quick(dev_list, split_point+1, end);
    }
}

void DataModel::sort_usb_devices_quick(std::vector<UsbDevice> &dev_list)
{
    sort_usb_devices_quick(dev_list, 0, dev_list.size()-1);
}

static std::vector<UsbDevice> merge(const std::vector<UsbDevice> &lv, const std::vector<UsbDevice> &rv)
{
    std::vector<UsbDevice> v;

    auto lvi = lv.begin();
    auto rvi = rv.begin();

    while (lvi != lv.end() && rvi != rv.end())
    {
        if (*lvi < *rvi)
        {
            v.push_back(std::move(*lvi));
            lvi++;
        }
        else
        {
            v.push_back(std::move(*rvi));
            rvi++;
        }
    }

    for (;lvi != lv.end(); lvi++)
    {
        v.push_back(std::move(*lvi));
    }

    for (;rvi != rv.end(); rvi++)
    {
        v.push_back(std::move(*rvi));
    }

    return v;
}

void DataModel::sort_usb_devices_merge(std::vector<UsbDevice> &dev_list)
{
    const int len = dev_list.size();
    const int split = len / 2;

    if (len <= 1)
        return;

    std::vector<UsbDevice> left_half = {dev_list.begin(), dev_list.begin() + split};
    std::vector<UsbDevice> right_half = {dev_list.begin() + split, dev_list.end()};

    sort_usb_devices_merge(left_half);
    sort_usb_devices_merge(right_half);

    dev_list = merge(left_half, right_half);
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
    UsbDevice drive1(
        "",
        "Micro Center",
        "Toshiba MLC",
        'A',
        3.0f,
        15112,
        true
    );
    UsbDevice drive2(
        "",
        "Dongcoh",
        "Western Digital",
        'A',
        3.0f,
        610352,
        false
    );

    // I don't own any of these devices below this point

    UsbDevice drive3(
        "Duo Plus",
        "Samsung",
        "",
        'C',
        3.1f,
        122070,
        true
    );
    UsbDevice drive4(
        "Attaché 3",
        "PNY",
        "",
        'A',
        3.0f,
        61035,
        true
    );
    for (int i = 0; i < 500; i++)
    {
        usb_devices.push_back(drive0);
        usb_devices.push_back(drive1);
        usb_devices.push_back(drive2);
        usb_devices.push_back(drive3);
        usb_devices.push_back(drive4);
    }

    unsigned long long bef = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    print_all_devices(usb_devices);

    std::cout << "-----------------------------------------------" << std::endl;

    sort_usb_devices_merge(usb_devices);

    unsigned long long aft = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    print_all_devices(usb_devices);

    std::cout << "Finished " << aft-bef << " ms" << std::endl;
}
