#pragma once
#include <iostream>
#include "Line.h"

class TextLine : public Line {
    std::string text;

public:
    TextLine(const std::string& t) : text(t) {}

    void print() const override {
        std::cout << text << std::endl;
    }

    std::string serialize() const override {
        return std::string("T|") + text;
    }

    char type() const override { return 'T'; }
};
