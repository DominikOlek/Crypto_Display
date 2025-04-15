#pragma once
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <codecvt>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Storage.Streams.h>
#include <iostream>
class Queue;

class NetworkController;


class BleSender {
public:
    BleSender(Queue& queue);
    void sendData(std::string data,bool flag) const;
private:
    std::wstring to_wstring(const std::string& stringToConvert) const;
    Queue* queue_;
};

