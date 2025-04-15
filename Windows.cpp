#include "Windows.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Queue.h"
#include "SearchPanel.h"

wxListBox* listCryp;
wxListBox* listCoin;
wxListBox* listQueue = nullptr;
std::string selectedCrypto = "";
std::string selectedCoin = "";
wxStaticText* value = nullptr;

std::unordered_map<std::string,std::pair<wxGBPosition,wxGBSpan>> MainFrame::items = {
    {"title",{{1,1},{1,7}}}, //title
    {"crypsearch",{{2,1},{1,1}}}, // crypsearch
    {"cryp",{{3,1},{1,1}}}, //cryp
    {"coinsearch",{{2,2},{1,1}}}, //coinsearch
    {"coin",{{3,2},{1,1}}}, //coin
    {"data",{{2,4},{1,2}}}, //data
    {"queue",{{3,4},{1,3}}},
{"delqueue",{{4,4},{1,3}}},
    {"size",{{4,8},{1,1}}}
};

void setStyleList(wxListBox& list) {
    list.SetBackgroundColour(wxColour(150,150,150));
    list.SetForegroundColour(wxColour(255,192,0));
    list.SetFont(wxFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD));
}

MainFrame::MainFrame(const wxString &title, const wxSize &size):wxFrame(nullptr,wxID_ANY,title,wxDefaultPosition,size) {
    searchPanel = new SearchPanel(*this);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainView = new wxPanel(this,wxID_ANY);
    sizer = new wxGridBagSizer(margin,margin);
    SetLayout();
    sizer->AddGrowableCol(1,2);
    sizer->AddGrowableCol(5,2);
    sizer->AddGrowableCol(3,2);
    sizer->AddGrowableCol(7,2);
    sizer->AddGrowableRow(3,4);
    sizer ->SetMinSize(FromDIP(size));
    changeCrypto();
    mainView->SetSizer(sizer);
    mainSizer->Add(mainView,1,wxEXPAND | wxALL,margin);
    this->SetSizerAndFit(mainSizer);
    changeCoins();
}

void MainFrame::SetLayout() {
    wxPanel* p = new wxPanel(mainView,wxID_ANY,wxDefaultPosition,sizer->GetEmptyCellSize());
    sizer->Add(p,items["size"].first,items["size"].second,wxEXPAND | wxALL,margin);
    wxStaticText* title = new wxStaticText(mainView,wxID_ANY,wxT("KRYPTORUCHY"));
    sizer->Add(title,items["title"].first,items["title"].second,wxEXPAND |wxALIGN_CENTER_HORIZONTAL| wxALL,margin);
    mainView->SetBackgroundColour(wxColour(65,65,65));
    value = new wxStaticText(mainView,wxID_ANY,wxT("Wybierz"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    sizer->Add(value,items["data"].first,items["data"].second,wxEXPAND  |wxALIGN_CENTER_HORIZONTAL | wxALL,margin);

    wxButton* delButton = new wxButton(mainView,wxID_ANY,wxT("Usun"));
    sizer->Add(delButton,items["delqueue"].first,items["delqueue"].second,wxEXPAND  |wxALIGN_CENTER_HORIZONTAL | wxALL,margin);
    delButton->Bind(wxEVT_BUTTON, &MainFrame::deleteElement, this);

    title->SetForegroundColour(wxColour(255,192,0));
    title->SetFont(wxFont(25, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD));
    value->SetForegroundColour(wxColour(255,192,0));
    value->SetFont(wxFont(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD));
    delButton->SetForegroundColour(wxColour(255,192,0));
    delButton->SetFont(wxFont(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_BOLD));

    wxTextCtrl* search = new wxTextCtrl(mainView,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize);
    search->Bind(wxEVT_TEXT,&MainFrame::onSearchCrypto,this);
    sizer->Add(search,items["crypsearch"].first,items["crypsearch"].second,wxALIGN_CENTER_HORIZONTAL | wxALL,margin);

    wxTextCtrl* searchCoins = new wxTextCtrl(mainView,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize);
    searchCoins->Bind(wxEVT_TEXT,&MainFrame::onSearchCoin,this);
    sizer->Add(searchCoins,items["coinsearch"].first,items["coinsearch"].second,wxALIGN_CENTER_HORIZONTAL | wxALL,margin);
}

void MainFrame::UpdateView() {
    Layout();
}

void MainFrame::OnClose(wxCloseEvent &event) {
    delete searchPanel;
    Destroy();
}
void MainFrame::onCrypSelect(wxCommandEvent &event) {
    int selection = listCryp->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString selectedText_ = listCryp->GetString(selection);
        selectedCrypto = selectedText_.ToStdString();
        searchPanel->setCoin(selectedCrypto);
        selectedCoin ="";
        changeCoins();
    }
}
void MainFrame::onSearchCrypto(wxCommandEvent &event) {
    wxString text = event.GetString();
    std::string key = std::string(text);
    for (int i = 0; i < key.length(); i++)
        key[i] = toupper(key[i]);
    changeCrypto(key);
}

void MainFrame::onSearchCoin(wxCommandEvent &event) {
    wxString text = event.GetString();
    std::string key = std::string(text);
    for (int i = 0; i < key.length(); i++)
        key[i] = toupper(key[i]);
    changeCoins(key);
}

void MainFrame::onCoinSelect(wxCommandEvent &event) {
    int selection = listCoin->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString selectedText = listCoin->GetString(selection);
        selectedCoin = selectedText.ToStdString();
        searchPanel->addToQueue(selectedCoin);
        editQueue();
    }
}
void MainFrame::changeCoins(const std::string& search) {
    wxArrayString choices;
    choices.clear();
    int elemID=-1,i =0;

    for (std::string coins: searchPanel->getCurrences()) {
        if (search.empty() || coins.find(search) != std::string::npos) {
            if (coins.compare(selectedCoin) == 0)
                elemID = i;
            choices.Add(coins);
            i++;
        }
    }
    if (listCoin != nullptr) {
        listCoin->Set(choices);
    }else {
        listCoin = new wxListBox(mainView,wxID_ANY,wxDefaultPosition,sizer->GetEmptyCellSize(),choices);
        setStyleList(*listCoin);
        listCoin->Bind(wxEVT_LISTBOX, &MainFrame::onCoinSelect, this);
        sizer->Add(listCoin,items["coin"].first,items["coin"].second,wxEXPAND | wxALL,margin);
        UpdateView();
    }
    if(elemID >= 0) listCoin->SetSelection(elemID);
}

void MainFrame::changeCrypto(const std::string& search) {
    wxArrayString choices;
    choices.clear();
    int elemID=-1,i =0;

    for (std::string coins: searchPanel->getCoins()) {
        if (search.empty() || coins.find(search) != std::string::npos) {
            if (coins.compare(selectedCrypto) == 0)
                elemID = i;
            choices.Add(coins);
            i++;
        }
    }
    if (listCryp != nullptr) {
        listCryp->Set(choices);
    }
    else {
        listCryp = new wxListBox(mainView,wxID_ANY,wxDefaultPosition,sizer->GetEmptyCellSize(),choices);
        setStyleList(*listCryp);
        listCryp->Bind(wxEVT_LISTBOX, &MainFrame::onCrypSelect, this);
        sizer->Add(listCryp,items["cryp"].first,items["cryp"].second,wxEXPAND | wxALL,margin);
        UpdateView();
    }
    if(elemID >= 0) listCryp->SetSelection(elemID);
}

void MainFrame::writeValue(const std::string &readBuffer) {
    using json = nlohmann::json;
    auto data = json::parse(readBuffer);
    std::string symbol = data["symbol"];
    std::string price = data["price"];
    value->SetLabel(wxString::FromUTF8(symbol+"\n"+price));
}

void MainFrame::onQueueSelect(wxCommandEvent &event) {
    int selection = listQueue->GetSelection();
    if (selection != wxNOT_FOUND) {
        searchPanel->getQueue()->setIndex(selection);
    }
}

void MainFrame::deleteElement(wxCommandEvent &event) {
    try {
        searchPanel->getQueue()->delPair();
        editQueue();
    }catch (std::exception &e) {
    }
}

void MainFrame::editQueue() {
    wxArrayString queue;
    queue.clear();

    for (std::pair<std::string,std::string> pair: searchPanel->getQueue()->getList()) {
        queue.Add(pair.first + pair.second);
    }
    std::cout << queue.size() << std::endl;
    if (listQueue != nullptr) {
        listQueue->Set(queue);
    }
    else {
        listQueue = new wxListBox(mainView,wxID_ANY,wxDefaultPosition,sizer->GetEmptyCellSize(),queue);
        setStyleList(*listQueue);
        listQueue->Bind(wxEVT_LISTBOX, &MainFrame::onQueueSelect, this);
        sizer->Add(listQueue,items["queue"].first,items["queue"].second,wxEXPAND | wxALL,margin);
        UpdateView();
    }
    listQueue->SetSelection(searchPanel->getQueue()->getCurrentIndex());
}
