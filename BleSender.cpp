
#include "BleSender.h"
#include "Queue.h"

using namespace winrt;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Storage::Streams;

static const uint64_t deviceAddress = 1955954709318;


BleSender::BleSender(Queue &queue) {
    queue_ = &queue;
}


std::wstring BleSender::to_wstring(const std::string& stringToConvert) const
{
    std::wstring wideString =
        std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(stringToConvert);
    return wideString;
}

void BleSender::sendData(std::string data, bool flag) const {
    using json = nlohmann::json;
    auto j3 = json::parse(data);
    std::string size = "$ " +static_cast<std::string>(j3["symbol"]);
    std::string size2 = j3["price"];
    size += "\n" + size2.substr(0,12);
    BluetoothLEDevice device = BluetoothLEDevice::FromBluetoothAddressAsync(deviceAddress).get();

    if (device) {
        GattDeviceServicesResult servicesResult = device.GetGattServicesAsync().get();
        GattDeviceService foundService{ nullptr };
        for (GattDeviceService service : servicesResult.Services()) {
            if (service.Uuid() == guid(L"12345678-1234-5678-1234-56789abcdef0")) {
                foundService = service;
                break;
            }
        }
        if (foundService) {
            GattCharacteristicsResult characteristicsResult = foundService.GetCharacteristicsAsync().get();
            for (GattCharacteristic characteristic : characteristicsResult.Characteristics()) {
                if (flag && characteristic.Uuid() == guid(L"12345678-1234-5678-1234-56789abcdef2")) {
                    std::wstring message = to_wstring(size);
                    DataWriter writer;
                    writer.WriteString(message);
                    characteristic.WriteValueAsync(writer.DetachBuffer()).get();
                    std::wcout << L"Wyslano do Arduino: " << message << std::endl;
                }else if (characteristic.Uuid() == guid(L"12345678-1234-5678-1234-56789abcdef1")) {
                    characteristic.ValueChanged([this](GattCharacteristic const& sender, GattValueChangedEventArgs const& args) {
                    auto reader = DataReader::FromBuffer(args.CharacteristicValue());
                    std::wstring message = reader.ReadString(args.CharacteristicValue().Length()).c_str();
                        std::wcout << message<<std::endl;
                        switch (std::stoi(message)) {
                            case 1: queue_->next();break;
                            case -1: queue_->prev();break;
                            case 0: queue_->delPair();break;
                        }
                    });
                    auto status = characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(
                        GattClientCharacteristicConfigurationDescriptorValue::Notify
                    ).get();

                    if (status != GattCommunicationStatus::Success) {
                        std::wcout << L"Nie udało się włączyć notyfikacji." << std::endl;
                    }
                }

            }
        }
        else {
            throw std::runtime_error("Could not find characteristic.");
        }
    }
    else {
        throw std::runtime_error("Could not find service.");
    }
}
