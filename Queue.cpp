
#include "Queue.h"
#include "SearchPanel.h"

#include <stdexcept>

Queue::Queue(SearchPanel &searchPanel) {
    searchPanel_ = &searchPanel;
}


void Queue::addPair(std::string crypto, std::string currency) {
    list.emplace_back(crypto,currency);
    index = list.size()-1;
}

std::pair<std::string, std::string> Queue::getCurrent() {
    return list[index];
}

void Queue::delPair() {
    if(list.size() <= 1)
        throw std::invalid_argument("Queue is empty");
    list.erase(list.begin()+index);
    if (index >= list.size()) {
        index--;
    }
    updateSearch();
}

void Queue::next() {
    index >= list.size()-1 ? index = 0 : index++;
    updateSearch();
}

void Queue::prev() {
    index <=0 ? index = list.size() -1 : index--;
    updateSearch();
}

void Queue::setIndex(size_t ind) {
    index = ind;
    updateSearch();
}
size_t Queue::getCurrentIndex() {
    return index;
}

void Queue::updateSearch() {
    searchPanel_->setCoin(getCurrent().first);
    searchPanel_->setCur(getCurrent().second);
}

std::vector<std::pair<std::string, std::string> > Queue::getList() const {
    return list;
}



