#pragma once
#include <vector>
#include "Text.h"
#include "Cipher.h"

class TextEditor {
    std::vector<Text*> tabs;
    int activeTab;

    Text& active();
    CipherType chooseCipherType();

    void printMenu();
    void addTextLine();
    void addContact();
    void addChecklist();
    void encryptAndSave();
    void loadAndDecrypt();
    void newTab();
    void switchTab();
    void closeTab();

public:
    TextEditor();
    ~TextEditor();
    void run();
};
