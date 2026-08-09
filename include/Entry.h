#pragma once

#include <cstddef>

struct  Entry {
    size_t doc_id;
    size_t count;

    bool operator == (const Entry& others) const {
        return doc_id == others.doc_id &&
               count == others.count;
    }
};