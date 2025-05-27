#pragma once
#include "Main.h"
wxIMPLEMENT_APP(App);
#include "allClasses.h"


bool App::OnInit() {
    wxSize* size = new wxSize(800,600);
    AllClasses* classes = new AllClasses();
    MainFrame* window = new MainFrame("KryptoRuchy",*size,*classes);
    window->Show(true);
    window->setAllClasses(classes);
    return true;
}



