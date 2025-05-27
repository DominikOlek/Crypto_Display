#pragma once
#include <wx/wx.h>
#include <string>
#include <thread>
#include <vector>

class Queue;
class NetworkController;
class MainFrame;
class AllClasses;

class SearchPanel {
public:
    SearchPanel(MainFrame& gui, AllClasses& classes);
    std::vector<std::string> getCoins() const;
    std::vector<std::string> getCurrences() const;
    Queue* getQueue() const;
    void clear();
    void setCoin(std::string coin);
    void setCur(std::string cur);
    bool addToQueue(std::string cur);
    bool searchCoin(std::string coin) const;
    void addCoin(std::string coin);
    void addCurrency(std::string coin,std::string curr = "");
    std::string getSelectCurrency() const;
    std::string getSelectCoin() const ;
    ~SearchPanel();
private:
    std::vector<std::string> coins;
    std::unordered_map<std::string,std::vector<std::string>> currency;
    std::string coinSymbol;
    std::string currencySymbol;
    std::atomic<bool> stop_flag = false;
    std::thread refresh;
    NetworkController *network_;
    MainFrame* gui_;
    Queue* queue_;
};



