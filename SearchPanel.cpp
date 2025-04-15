#pragma once
#include "SearchPanel.h"
#include "NetworkController.h"
#include "Queue.h"
#include "Windows.h"

SearchPanel::SearchPanel(MainFrame& gui) {
    gui_ = &gui;
    queue_ = new Queue(*this);
    network_ = (new NetworkController(*this,*gui_,*queue_));
};


std::vector<std::string> SearchPanel::getCoins() const {
    return coins;
}

std::vector<std::string> SearchPanel::getCurrences() const {
    return currency.at(coinSymbol);
}

void SearchPanel::setCoin(std::string coin) {
    coinSymbol = coin;
    stop_flag.store(true);
    if (refresh.joinable()) {
        refresh.join();
    }
}
void SearchPanel::setCur(std::string cur) {
    try {
        currencySymbol = cur;
        if (!coinSymbol.empty() && stop_flag.load()) {
            stop_flag.store(false);
            refresh = network_->RunTask(stop_flag);
            gui_->editQueue();
        }
    }catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void SearchPanel::addToQueue(std::string cur) {
    currencySymbol = cur;
    queue_->addPair(coinSymbol, currencySymbol);
    setCur(cur);
}

bool SearchPanel::searchCoin(std::string coin) const{
    return find(coins.begin(), coins.end(), coin) == coins.end();
}

void SearchPanel::addCoin(std::string coin) {
    coins.push_back(coin);
}

void SearchPanel::addCurrency(std::string coin, std::string curr) {
    if (curr == "") {
        currency.insert({coin,std::vector<std::string>()});
    }else {
        currency.at(coin).push_back(curr);
    }
}

std::string SearchPanel::getSelectCoin() const {
    return coinSymbol;
}

std::string SearchPanel::getSelectCurrency() const {
    return currencySymbol;
}

Queue* SearchPanel::getQueue() const {
    return queue_;
}

SearchPanel::~SearchPanel() {
    std::cout<<"clear"<<std::endl;
    stop_flag.store(true);
    if (refresh.joinable()) {
        refresh.join();
    }
    coins.clear();
    currency.clear();
    delete network_;
    //coins = std::vector<std::string>();
    //currency = std::unordered_map<std::string,std::vector<std::string>>();
}



