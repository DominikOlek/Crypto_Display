#pragma once
#include <string>
#include <unordered_map>

class SearchPanel;


class Queue {
    public:
        Queue(SearchPanel& searchPanel);
        void addPair(std::string crypto,std::string currency);
        void delPair();
        std::pair<std::string,std::string> getCurrent();
        void next();
        void prev();
        size_t getCurrentIndex();
        void setIndex(size_t ind);
        std::vector<std::pair<std::string,std::string>> getList() const;
        bool findPair(std::string crypto, std::string currency);
private:
    std::vector<std::pair<std::string,std::string>> list;
    size_t index = 0;
    SearchPanel* searchPanel_;
    void updateSearch();
};
