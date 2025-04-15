# Crypto_Display
This project is a simple wxwidget application, that allow you to display the currently crypto value in app and on the display which connect by BLE.

Exactly, you can search and select crypto by short-name and then select the currency. When you do that, this combination will be added to the list. Value for selected element in list is showing in app, and also on display with around 1 second frequency. The device include small OLED screen, and IRL receiver so you can switching between value in list, and remove it by IRL controller.

From technical point of view, I used curl to connecting with api.binance.com, nlohmann/json to convert package, wxwidget to gui controller, and winrt to create connect between devices by BLE. The device was made on Arduino Nano 33BLE, so program for it was write also in C++ but for Arduino.

There's a bit of chaos in CMake file, because I used vcpkg to import curl, wxwidget was install at the same directory as project manually, and json was add by FetchContent.
