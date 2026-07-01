#pragma once
#include <iostream>
#include "Line.h"

class ContactLine : public Line {
    std::string name;
    std::string surname;
    std::string email;

public:
    ContactLine(const std::string& n, const std::string& s, const std::string& e)
        : name(n), surname(s), email(e) {}

    void print() const override {
        std::cout << "Contact: " << name << " " << surname
                  << " <" << email << ">" << std::endl;
    }

    std::string serialize() const override {
        return std::string("C|") + name + "|" + surname + "|" + email;
    }

    char type() const override { return 'C'; }
};
