#pragma once

#include <string>
#include <vector>

#include "RelativeIndex.h"

class ConverterJSON {
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponseLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(
        const std::vector<std::vector<RelativeIndex>> & answers
        );
};