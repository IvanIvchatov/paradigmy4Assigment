#pragma once
#include <string>
#include <vector>
#include "Line.h"

class Text {
    std::vector<Line*> lines;

public:
    ~Text();

    void addLine(Line* line);
    void clear();
    int size() const;

    void printAll() const;

    std::string serializeAll() const;
    void deserializeAll(const std::string& data);

    static Line* createFromSerialized(const std::string& s);
};
