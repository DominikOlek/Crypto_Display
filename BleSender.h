#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <stdexcept>
#include <nlohmann/json.hpp>
#include <codecvt>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <iostream>
#include "wx/event.h"
#include <map>

class Queue;

class NetworkController;
class AllClasses;

using namespace winrt;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Storage::Streams;
using namespace Windows::Devices::Bluetooth::Advertisement;

class BleSender {
public:
    BleSender(Queue& queue,AllClasses& classes);
    void sendData(std::string data,bool flag) const;
    friend class MainFrame;
private:
    AllClasses* allClasses;
    uint64_t deviceAddress = 0;
    std::map<uint64_t,hstring> devices;
    std::wstring to_wstring(const std::string& stringToConvert) const;
    Queue* queue_;
    void OnAdvertisementReceived(BluetoothLEAdvertisementWatcher sender, BluetoothLEAdvertisementReceivedEventArgs args);
    void searchDevice();
    void RunSearch(wxCommandEvent& event);
};

