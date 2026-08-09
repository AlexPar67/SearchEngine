#pragma once

#include <cstddef>

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator == (const RelativeIndex& others) const {
        return doc_id == others.doc_id &&
               rank == others.rank;
    }
};