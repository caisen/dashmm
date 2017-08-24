// =============================================================================
//  Dynamic Adaptive System for Hierarchical Multipole Methods (DASHMM)
//
//  Copyright (c) 2015-2017, Trustees of Indiana University,
//  All rights reserved.
//
//  This software may be modified and distributed under the terms of the BSD
//  license. See the LICENSE file for details.
//
//  This software was created at the Indiana University Center for Research in
//  Extreme Scale Technologies (CREST).
// =============================================================================


/// \file
/// \brief FMM97Distro implementation


#include "builtins/fmm97distro.h"

#include <algorithm>
#include <map>
#include <limits>


namespace dashmm {


void FMM97Distro::compute_distribution(DAG &dag) {
  std::sort(dag.target_nodes.begin(), dag.target_nodes.end(),
            color_comparison);
  for (size_t i = 0; i < dag.target_nodes.size(); ++i) {
    if (dag.target_nodes[i]->is_interm()) {
      compute_locality(dag.target_nodes[i]);
    }
  }
}

void FMM97Distro::assign_for_source(DAGInfo &dag, int locality) {
  DAGNode *normal = dag.normal();
  if (normal != nullptr) {
    normal->locality = locality;
  }

  DAGNode *interm = dag.interm();
  if (interm != nullptr) {
    interm->locality = locality;
  }
}

void FMM97Distro::assign_for_target(DAGInfo &dag, int locality) {
  DAGNode *normal = dag.normal();
  DAGNode *interm = dag.interm();

  if (normal != nullptr) {
    normal->locality = locality;
    normal->color = 2 * normal->index().level();
  }

  if (interm != nullptr) {
    interm->color = 2 * interm->index().level() + 1;
  }
}

bool FMM97Distro::color_comparison(const DAGNode *a, const DAGNode *b) {
  return a->color > b->color;
}

void FMM97Distro::compute_locality(DAGNode *node) {
  // It already has a locality
  if (node->locality >= 0) return;

  // A rare case where a node does not enter into the computation. So pick
  // at random. A better idea might be to wait until all others are placed
  // ignorning this node, and then pick the best given the localities of the
  // upstream nodes, but for now, we do something simple.
  if (node->out_edges.size() == 0) {
    node->locality = 0;
  }

  // The typical case; count up weights to each locality
  int n_ranks = hpx_get_num_ranks();
  std::vector<int> bins(n_ranks, 0);    // Is there a better choice here?
  for (size_t i = 0; i < node->out_edges.size(); ++i) {
    int loc = node->out_edges[i].target->locality;
    assert(loc >= 0 && loc < n_ranks);
    bins[loc] += node->out_edges[i].weight;
  }

  // Find max - currently, the lowest locality in a tie will win.
  int maxval{bins[0]};
  size_t maxidx{0};
  for (int i = 1; i < n_ranks; ++i) {
    if (bins[i] > maxval) {
      maxval = bins[i];
      maxidx = i;
    }
  }

  // Set this node to have locality that matches the bulk of outgoing work
  node->locality = maxidx;
}

} // dashmm
