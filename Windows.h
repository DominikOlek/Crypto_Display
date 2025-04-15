#pragma once
#include <wx/wx.h>
#include "wx/gbsizer.h"
#include <wx/wx.h>
#include "Main.h"
#include "wx/gbsizer.h"
#include "wx/richtext/richtextbuffer.h"
#include <cstring>

class SearchPanel;

class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title, const wxSize& size);
        void writeValue(const std::string &readBuffer);
    private:
        void OnClose(wxCloseEvent& event);
        void onCrypSelect(wxCommandEvent& event);
        void onCoinSelect(wxCommandEvent& event);
        void changeCoins(const std::string& search="");
        void changeCrypto(const std::string& search="");
        void UpdateView();
        void SetLayout();
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
};