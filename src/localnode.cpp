#include <iostream>
#include <algorithm>
#include "localnode.h"
#include "interval.h"
#include "utils.h" // for pointer_values_equal

LocalNode::LocalNode(std::string s, Interval p, uint32_t i)
    : seq(s)
    , pos(p)
    , id(i)
    , covg(p.length)
    , sketch_next(pos.start)
    , skip(false)
{
}

std::ostream& operator<<(std::ostream& out, LocalNode const& n)
{
    out << "(" << n.id << " " << n.pos << " " << n.seq << ")";
    return out;
}

bool LocalNode::operator==(const LocalNode& y) const
{
    if (seq != y.seq) {
        return false;
    }
    if (id != y.id) {
        return false;
    }
    if (outNodes.size() != y.outNodes.size()) {
        return false;
    }
    for (uint32_t i = 0; i != outNodes.size(); ++i) {
        spointer_values_equal<LocalNode> eq = { outNodes[i] };
        if (find_if(y.outNodes.begin(), y.outNodes.end(), eq) == y.outNodes.end()) {
            return false;
        }
    }
    return true;
}
