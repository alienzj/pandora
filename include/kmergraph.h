#ifndef __KMERGRAPH_H_INCLUDED__   // if kmergraph.h hasn't been included yet...
#define __KMERGRAPH_H_INCLUDED__

class KmerNode;

#include <cstring>
#include <map>
#include <vector>
#include <iostream>
#include "path.h"
#include "kmernode.h"

class KmerGraph {
    uint32_t next_id;
    uint32_t k;
    float p;
  public:
    uint32_t num_reads;
    std::vector<KmerNode*> nodes; // representing nodes in graph
    KmerGraph();
    ~KmerGraph();
    void clear();
    void add_node (const Path&);
    void add_edge (const uint32_t&, const uint32_t&);
    void add_edge (const Path&, const Path&);
    //void copy_innodes (const Path&, const Path&);
    std::set<Path> get_innodes (const Path&);
    void check (uint);
    //std::vector<KmerNode*> get_node_order();
    float prob(uint, int);
    float find_max_path(int, float, std::vector<KmerNode*>&);
    void save (const std::string&);
    void load (const std::string&);
    bool operator == (const KmerGraph& y) const;
    friend std::ostream& operator<< (std::ostream & out, KmerGraph const& data);
    friend struct condition;
    friend class KmerGraphTest_findMaxPathSimple_Test;
    friend class KmerGraphTest_findMaxPath2Level_Test;
};

struct condition
{
    Path q;
    condition(const Path&);
    bool operator()(const KmerNode*) const;
};

#endif
