#pragma once
#include <iostream>
#include "Line.h"

class ChecklistLine : public Line {
    std::string item;
    bool checked;

public:
    ChecklistLine(const std::string& i, bool c) : item(i), checked(c) {}

    void print() const override {
        std::cout << "[" << (checked ? "x" : " ") << "] " << item << std::endl;
    }

    std::string serialize() const override {
        return std::string("K|") + (checked ? "1" : "0") + "|" + item;
    }

    char type() const override { return 'K'; }
};
