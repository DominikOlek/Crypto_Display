#pragma once
#include "BleSender.h"
#include "NetworkController.h"
#include "Queue.h"
#include "SearchPanel.h"

class AllClasses {
  public:
    MainFrame *mainFrame;
    SearchPanel *searchPanel;
    NetworkController *network;
    Queue* queue;
    BleSender *bleClass;
};