#include "TextEditor.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include "Cipher.h"
#include <iostream>
#include <fstream>
#include <sstream>

TextEditor::TextEditor() {
    tabs.push_back(new Text());
    activeTab = 0;
}

TextEditor::~TextEditor() {
    for (size_t i = 0; i < tabs.size(); ++i) {
        delete tabs[i];
    }
}

Text& TextEditor::active() {
    return *tabs[activeTab];
}

CipherType TextEditor::chooseCipherType() {
    std::cout << "Cipher (1 = Caesar, 2 = Vigenere): ";
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');
    return (choice == 1) ? CipherType::Caesar : CipherType::Vigenere;
}

void TextEditor::printMenu() {
    std::cout << "\n--- TEXT EDITOR  [Tab " << (activeTab + 1)
              << "/" << tabs.size() << ", " << active().size() << " lines] ---\n";
    std::cout << "1. Add text line\n";
    std::cout << "2. Add contact\n";
    std::cout << "3. Add checklist item\n";
    std::cout << "4. Print all\n";
    std::cout << "5. Encrypt and save to file\n";
    std::cout << "6. Load and decrypt from file\n";
    std::cout << "7. New tab\n";
    std::cout << "8. Switch tab\n";
    std::cout << "9. Close tab\n";
    std::cout << "10. Exit\n";
    std::cout << "Choose the command: ";
}

void TextEditor::addTextLine() {
    std::cout << "Enter text: ";
    std::string t;
    std::getline(std::cin, t);
    active().addLine(new TextLine(t));
}

void TextEditor::addContact() {
    std::string name, surname, email;
    std::cout << "Name: ";    std::getline(std::cin, name);
    std::cout << "Surname: "; std::getline(std::cin, surname);
    std::cout << "Email: ";   std::getline(std::cin, email);
    active().addLine(new ContactLine(name, surname, email));
}

void TextEditor::addChecklist() {
    std::string item, status;
    std::cout << "Item: "; std::getline(std::cin, item);
    std::cout << "Done? (y/n): "; std::getline(std::cin, status);
    bool checked = (!status.empty() && (status[0] == 'y' || status[0] == 'Y'));
    active().addLine(new ChecklistLine(item, checked));
}

void TextEditor::encryptAndSave() {
    CipherType type = chooseCipherType();

    std::string path, key;
    std::cout << "Output file path: "; std::getline(std::cin, path);
    std::cout << (type == CipherType::Caesar ? "Key (number): " : "Key (word): ");
    std::getline(std::cin, key);

    Cipher cipher;
    if (!cipher.isLoaded()) {
        std::cout << "Error: cipher.dll not loaded.\n";
        return;
    }

    std::string data = active().serializeAll();
    std::string encrypted = cipher.encrypt(data, key, type);

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        std::cout << "Error: cannot open file for writing.\n";
        return;
    }
    out << encrypted;
    out.close();
    std::cout << "Saved encrypted text to: " << path << "\n";
}

void TextEditor::loadAndDecrypt() {
    CipherType type = chooseCipherType();

    std::string path, key;
    std::cout << "Input file path: "; std::getline(std::cin, path);
    std::cout << (type == CipherType::Caesar ? "Key (number): " : "Key (word): ");
    std::getline(std::cin, key);

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cout << "Error: cannot open file for reading.\n";
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string encrypted = buffer.str();
    in.close();

    Cipher cipher;
    if (!cipher.isLoaded()) {
        std::cout << "Error: cipher.dll not loaded.\n";
        return;
    }

    std::string data = cipher.decrypt(encrypted, key, type);
    active().deserializeAll(data);

    std::cout << "Loaded and decrypted text:\n";
    active().printAll();
}

void TextEditor::newTab() {
    tabs.push_back(new Text());
    activeTab = (int)tabs.size() - 1;
    std::cout << "Created tab " << (activeTab + 1) << ", switched to it.\n";
}

void TextEditor::switchTab() {
    std::cout << "Tabs:\n";
    for (size_t i = 0; i < tabs.size(); ++i) {
        std::cout << "  " << (i + 1) << ": " << tabs[i]->size() << " lines"
                  << (i == (size_t)activeTab ? "  (active)" : "") << "\n";
    }
    std::cout << "Switch to tab number: ";
    int n;
    if (!(std::cin >> n)) { std::cin.clear(); std::cin.ignore(10000, '\n'); return; }
    std::cin.ignore(10000, '\n');

    if (n >= 1 && n <= (int)tabs.size()) {
        activeTab = n - 1;
        std::cout << "Switched to tab " << n << ".\n";
    } else {
        std::cout << "No such tab.\n";
    }
}

void TextEditor::closeTab() {
    if (tabs.size() <= 1) {
        std::cout << "Cannot close the last tab.\n";
        return;
    }
    delete tabs[activeTab];
    tabs.erase(tabs.begin() + activeTab);
    if (activeTab >= (int)tabs.size()) {
        activeTab = (int)tabs.size() - 1;
    }
    std::cout << "Tab closed. Now on tab " << (activeTab + 1) << ".\n";
}

void TextEditor::run() {
    int choice = 0;
    while (true) {
        printMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input! Enter a number 1-10.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case 1: addTextLine();    break;
            case 2: addContact();     break;
            case 3: addChecklist();   break;
            case 4: active().printAll(); break;
            case 5: encryptAndSave(); break;
            case 6: loadAndDecrypt(); break;
            case 7: newTab();         break;
            case 8: switchTab();      break;
            case 9: closeTab();       break;
            case 10: return;
            default: std::cout << "Unknown command! Choose 1-10.\n"; break;
        }
    }
}
