#pragma once
#include <wx/wx.h>
#include "wx/gbsizer.h"
#include <wx/wx.h>
#include "Main.h"
#include "wx/gbsizer.h"
#include "wx/richtext/richtextbuffer.h"
#include <cstring>

class AllClasses;
class SearchPanel;

class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title, const wxSize& size,AllClasses& classes);
        void writeValue(const std::string &readBuffer);
        void setAllClasses(AllClasses* allClasses);
        void Load();
    private:
        void OnClose(wxCloseEvent& event);
        void onCrypSelect(wxCommandEvent& event);
        void onCoinSelect(wxCommandEvent& event);
        void changeCoins(const std::string& search="");
        void changeCrypto(const std::string& search="");
        void UpdateView();
        void SetLayout();
        void LoadSearched(wxCommandEvent& event);
        static std::unordered_map<std::string,std::pair<wxGBPosition,wxGBSpan>> items;
        wxPanel* mainView;
        wxGridBagSizer* sizer;
        wxBoxSizer* mainSizer;
        const int margin = FromDIP(10);
        void onSearchCrypto(wxCommandEvent &event);
        void onSearchCoin(wxCommandEvent &event);
        SearchPanel *searchPanel;
        void editQueue();
        void onQueueSelect(wxCommandEvent &event);
        void deleteElement(wxCommandEvent &event);
        friend SearchPanel;
        AllClasses* allClasses;
        wxMenuBar* menuBar = new wxMenuBar();
        wxMenu* fileMenu = new wxMenu();
        std::unordered_map<int, uint64_t> deviceMap;
        int nextId = 1000;
        void OnDeviceSelected(wxCommandEvent& event);
        void setTextStyle(wxWindow* label,int size=25);
};