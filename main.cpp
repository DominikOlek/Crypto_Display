#pragma once
#include "Main.h"
wxIMPLEMENT_APP(App);


bool App::OnInit() {
    wxSize* size = new wxSize(800,600);
    MainFrame* window = new MainFrame("KryptoRuchy",*size);
    window->Show(true);
    return true;
}



