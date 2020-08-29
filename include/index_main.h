#ifndef PANDORA_INDEX_MAIN_H
#define PANDORA_INDEX_MAIN_H

#include <omp.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "utils.h"
#include "localPRG.h"
#include "CLI11.hpp"

/// Collection of all options of index subcommand.
struct IndexOptions {
    std::string prgfile;
    uint32_t window_size { 14 };
    uint32_t kmer_size { 15 };
    uint32_t threads { 1 };
    uint32_t id_offset { 0 };
    std::string outfile;
    uint8_t verbosity { 0 };
};

void setup_index_subcommand(CLI::App& app);
int pandora_index(IndexOptions const& opt);

#endif // PANDORA_INDEX_MAIN_H
