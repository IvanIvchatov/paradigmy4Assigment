#include "Text.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include <sstream>

Text::~Text() {
    clear();
}

void Text::addLine(Line* line) {
    lines.push_back(line);
}

void Text::clear() {
    for (size_t i = 0; i < lines.size(); ++i) {
        delete lines[i];
    }
    lines.clear();
}

int Text::size() const {
    return (int)lines.size();
}

void Text::printAll() const {
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << i << ": ";
        lines[i]->print();
    }
}

std::string Text::serializeAll() const {
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i) {
        result += lines[i]->serialize();
        result += "\n";
    }
    return result;
}

void Text::deserializeAll(const std::string& data) {
    clear();
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        Line* obj = createFromSerialized(line);
        if (obj != nullptr) {
            lines.push_back(obj);
        }
    }
}

Line* Text::createFromSerialized(const std::string& s) {
    if (s.size() < 2 || s[1] != '|') {
        return nullptr;
    }

    char kind = s[0];

    if (kind == 'T') {
        return new TextLine(s.substr(2));
    }

    if (kind == 'C') {
        std::string rest = s.substr(2);
        std::stringstream ss(rest);
        std::string name, surname, email;
        std::getline(ss, name, '|');
        std::getline(ss, surname, '|');
        std::getline(ss, email, '|');
        return new ContactLine(name, surname, email);
    }

    if (kind == 'K') {
        bool checked = (s[2] == '1');
        std::string item;
        size_t bar = s.find('|', 2);
        if (bar != std::string::npos) {
            item = s.substr(bar + 1);
        }
        return new ChecklistLine(item, checked);
    }

    return nullptr;
}
