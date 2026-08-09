#pragma once

#include <vector>
#include <string>

#include "RelativeIndex.h"
#include "InvertedIndex.h"

class SearchServer {
public:
    explicit SearchServer(
        InvertedIndex& idx,
        size_t max_responses = 5
    );

    std::vector<std::vector<RelativeIndex>> search(
        const std::vector<std::string>& queries_input
    );

private:
    InvertedIndex& index;
    size_t max_responses;
};