#ifndef __VCFRECORD_H_INCLUDED__   // if vcfrecord.h hasn't been included yet...
#define __VCFRECORD_H_INCLUDED__

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include "sampleinfo.h"

struct VCFRecord {
private:
    std::string infer_SVTYPE() const;


public:
    //#CHROM POS ID REF ALT QUAL FILTER INFO FORMAT
    std::string chrom;
    uint32_t pos;
    std::string id; // not used
    std::string ref;
    std::vector<std::string> alts;
    std::string qual; // not used
    std::string filter; // not used
    std::string info;
    std::vector<std::string> format; //e.g. "GT"
    SamplesInfos sampleIndex_to_sampleInfo;


    VCFRecord(const std::string &chrom, uint32_t pos, const std::string &ref, const std::string &alt,
              const std::string &info=".", const std::string &graph_type_info="");
    VCFRecord();
    VCFRecord(const VCFRecord &) = default;
    VCFRecord &operator=(const VCFRecord &) = default;
    ~VCFRecord() = default;



    inline void clear() {
        *this = VCFRecord();
    }

    void clear_sample(uint32_t);

    void add_formats(const std::vector<std::string> &);

    void set_format(const uint32_t&, const std::string&, const std::vector<uint16_t>&);

    void set_format(const uint32_t&, const std::string&, const std::vector<float>&);

    void set_format(const uint32_t&, const std::string&, const uint16_t&);

    void set_format(const uint32_t&, const std::string&, const uint32_t&);

    void set_format(const uint32_t&, const std::string&, const float&);

    void append_format(const uint32_t&, const std::string&, const uint16_t&);

    void append_format(const uint32_t&, const std::string&, const uint32_t&);

    void append_format(const uint32_t&, const std::string&, const float&);

    std::vector<uint16_t> get_format_u(const uint32_t&, const std::string&);

    std::vector<float> get_format_f(const uint32_t&, const std::string&);

    bool contains_dot_allele() const;

    bool operator==(const VCFRecord &y) const;

    bool operator!=(const VCFRecord &y) const;

    bool operator<(const VCFRecord &y) const;

    std::string to_string() const;
    friend std::ostream &operator<<(std::ostream &out, const VCFRecord &m);

    friend std::istream &operator>>(std::istream &in, VCFRecord &m);


    inline bool graph_type_is_simple() const {
        return this->info.find("GRAPHTYPE=SIMPLE") != std::string::npos;
    }
    inline bool graph_type_is_nested() const {
        return this->info.find("GRAPHTYPE=NESTED") != std::string::npos;
    }
    inline bool graph_type_has_too_many_alts() const {
        return this->info.find("GRAPHTYPE=TOO_MANY_ALTS") != std::string::npos;
    }
    inline bool svtype_is_SNP() const {
        return this->info.find("SVTYPE=SNP") != std::string::npos;
    }
    inline bool svtype_is_indel() const {
        return this->info.find("SVTYPE=INDEL") != std::string::npos;
    }
    inline bool svtype_is_PH_SNPs() const {
        return this->info.find("SVTYPE=PH_SNPs") != std::string::npos;
    }
    inline bool svtype_is_complex() const {
        return this->info.find("SVTYPE=COMPLEX") != std::string::npos;
    }
    inline bool has_the_same_position(const VCFRecord &other) const {
        return this->chrom==other.chrom and this->pos==other.pos;
    }
    inline bool has_non_null_reference () const {
        return this->ref != "." and this->ref != "";
    }

    size_t get_longest_allele_length() const;



    // MERGING-RELATED METHODS
    inline void merge_record_into_this(const VCFRecord &other) {
        this->sampleIndex_to_sampleInfo.merge_other_samples_infos_into_this(other.sampleIndex_to_sampleInfo);
        merge_gt(other);
        add_alts(other);
    }

private:
    void merge_gt(const VCFRecord &other);
    inline void add_alts(const VCFRecord &other) {
        this->alts.insert(this->alts.end(), other.alts.begin(), other.alts.end());
    }
    // END OF MERGING-RELATED METHODS
};

#endif
