#ifndef __FASTAQ_HANDLER_H_INCLUDED__ // if fastaq_handler.h hasn't been included yet...
#define __FASTAQ_HANDLER_H_INCLUDED__

#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <exception>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <cstdio>
#include <zlib.h>
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

namespace logging = boost::log;

struct FastaqHandler {
private:
    bool closed;
    kseq_t* inbuf;

public:
    const std::string filepath;
    gzFile fastaq_file;
    std::string name;
    std::string read;
    uint32_t num_reads_parsed;

    FastaqHandler(const std::string);

    ~FastaqHandler();

    bool eof() const;

    void get_next();

    void get_nth_read(const uint32_t& idx);

    void close();

    bool is_closed() const;
};

#endif
