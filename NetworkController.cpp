#include "NetworkController.h"
#include "BleSender.h"
#include "Queue.h"
#include "SearchPanel.h"
#include "Windows.h"

size_t NetworkController::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

NetworkController::NetworkController(SearchPanel& searchPanel,MainFrame& gui,Queue& elementQueue): searchPanel_(searchPanel), gui_(&gui) {
    bleClass_ = new BleSender(elementQueue);
    getPossibility();
}

NetworkController::~NetworkController() {
    curl_easy_cleanup(curl);
    delete bleClass_;
};

void NetworkController::getPossibility() {
    curl = curl_easy_init();
    std::string readBuffer;
    using json = nlohmann::json;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "D:/test/cacert.pem");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        if (getData("https://api.binance.com/api/v3/exchangeInfo",readBuffer)) {
            auto j3 = json::parse(readBuffer);
            int size = j3["symbols"].size();
            for (int i = 0; i < size; i++) {
                std::string coi = j3["symbols"][i]["baseAsset"].get<std::string>();
                if (searchPanel_.searchCoin(coi)) {
                    searchPanel_.addCoin(coi);
                    searchPanel_.addCurrency(coi);
                    searchPanel_.setCoin(coi);
                }
                std::string curr = j3["symbols"][i]["quoteAsset"].get<std::string>();
                searchPanel_.addCurrency(coi,curr);
            }
        }else {
            std::cerr << "Try again later";
        }
    }
}


bool NetworkController::getData(std::string url, std::string& response) const {
    long http_code;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (res == CURLE_OK && http_code >= 200 && http_code < 300) {
        return true;
    } else {
        return false;
    }
}


void NetworkController::getCurrent(std::atomic<bool>& stop_flag) const{
    int step = 6;
    try {
        std::string readBuffer = "{\"symbol\":\"NONE\",\"price\":\"0\"}";
        while (!stop_flag.load()) {
            if (step > 5) readBuffer.clear();
            if (step > 5 && getData("https://api.binance.com/api/v3/ticker/price?symbol="+searchPanel_.getSelectCoin()+searchPanel_.getSelectCurrency() ,readBuffer)) {
                std::cout << readBuffer << std::endl;
                gui_->writeValue(readBuffer);
                bleClass_->sendData(readBuffer,true);
                step = 0;
            }
            step++;
            bleClass_->sendData(readBuffer,false);
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

std::thread NetworkController::RunTask(std::atomic<bool>& stop_flag) const {
    return std::thread([this, &stop_flag]() {
        getCurrent(stop_flag);
    });
}