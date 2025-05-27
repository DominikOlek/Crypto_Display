#pragma once
#include <wx/wx.h>
#include <string>
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class Queue;
class SearchPanel;
class BleSender;
class MainFrame;
class AllClasses;

class NetworkController {
private:
    void getPossibility();
    bool getData(std::string url,std::string& response) const;
    CURL* curl;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    SearchPanel& searchPanel_;
    BleSender *bleClass_;
    MainFrame* gui_;
public:
    NetworkController(SearchPanel& searchPanel, MainFrame& gui,Queue& elementQueue,AllClasses& classes);
    void getCurrent(std::atomic<bool>& stop_flag) const;
    std::thread RunTask(std::atomic<bool>& stop_flag) const;
    ~NetworkController();
};

