#ifndef __DASHMM_FMM_METHOD_H__
#define __DASHMM_FMM_METHOD_H__

/// \file include/fmm_method.h
/// \brief Declaration of FMM Method

#include "include/ids.h"
#include "include/expansionref.h"
#include "include/method.h"
#include "include/node.h"
#include "include/index.h"

namespace dashmm {

/// A Method to implement classic FMM
///
class FMM : public Method {
 public:
  FMM() : local_{nullptr} {
    local_ = static_cast<MethodSerial *>(malloc(sizeof(MethodSerial)));
    assert(local_); 
    local_->type = kMethodFMM;
    local_->size = 0;
  }

  ~FMM() {
    if (local_) {
      free(local_); 
      local_ = nullptr;
    }
  }
   
  MethodSerial *release() override {
    MethodSerial *retval = local_; 
    local_ = nullptr;
    return retval;
  }

  size_t bytes() const override {
    return sizeof(MethodSerial);
  }

  int type() const override {return kMethodFMM;}

  bool compatible_with(const Expansion *expand) const override {
    return expand->provides_L(); 
  }

  void generate(SourceNode &curr, const ExpansionRef expand) const override; 
  void aggregate(SourceNode &curr, const ExpansionRef expand) const override;
  void inherit(TargetNode &curr, const ExpansionRef expand, 
               size_t which_child) const override;
  void process(TargetNode &curr, std::vector<SourceNode> &consider, 
               bool curr_is_leaf) const override;
  
  bool refine_test(bool same_sources_and_targets, const TargetNode &curr, 
                   const std::vector<SourceNode> &consider) const override;

  bool well_sep_test_asymmetric(Index smaller, Index larger) const; 
  bool well_sep_test(Index source, Index target) const;

  void proc_coll_recur(TargetNode &T, SourceNode &S) const; 

private: 
  MethodSerial *local_; 
}; 

} // namespace dashmm

#endif // __DASHMM_FMM_METHOD_H__