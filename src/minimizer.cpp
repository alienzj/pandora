#include <iostream>
#include <cassert>
#include <cmath>
#include "minimizer.h"
#include "prg/path.h"
#include "interval.h"

Minimizer::Minimizer(uint64_t s, uint32_t a, uint32_t b, bool c)
    : canonical_kmer_hash(s)
    , pos_of_kmer_in_read(Interval(a, b))
    , is_forward_strand(c)
{
    assert(s <= pow(4, pos_of_kmer_in_read.length)); // used to check kmer length same
                                                     // as interval length.
    // Can't any more but at least know if s is too big for a kmer of interval size to
    // have generated it.
}

Minimizer::~Minimizer()
{
    // if (path != NULL) {delete path;}
}

bool Minimizer::operator<(const Minimizer& y) const
{
    if (canonical_kmer_hash < y.canonical_kmer_hash) {
        return true;
    }
    if (y.canonical_kmer_hash < canonical_kmer_hash) {
        return false;
    }

    if (pos_of_kmer_in_read.start < y.pos_of_kmer_in_read.start) {
        return true;
    }
    if (y.pos_of_kmer_in_read.start < pos_of_kmer_in_read.start) {
        return false;
    }

    if (pos_of_kmer_in_read.length < y.pos_of_kmer_in_read.length) {
        return true;
    }
    if (y.pos_of_kmer_in_read.length < pos_of_kmer_in_read.length) {
        return false;
    }

    if (is_forward_strand < y.is_forward_strand) {
        return false;
    }
    if (y.is_forward_strand < is_forward_strand) {
        return true;
    }

    // if both are completely equal (based on strict weak ordering)
    // then just return false since equality doesn't yield less than
    return false;
}

bool Minimizer::operator==(const Minimizer& y) const
{
    if (canonical_kmer_hash != y.canonical_kmer_hash) {
        return false;
    }
    if (!(pos_of_kmer_in_read == y.pos_of_kmer_in_read)) {
        return false;
    }
    if (is_forward_strand != y.is_forward_strand) {
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, Minimizer const& m)
{
    out << "(" << m.canonical_kmer_hash << ", " << m.pos_of_kmer_in_read << ", "
        << m.is_forward_strand << ")";
    return out;
}
