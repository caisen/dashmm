// =============================================================================
//  Dynamic Adaptive System for Hierarchical Multipole Methods (DASHMM)
//
//  Copyright (c) 2015-2016, Trustees of Indiana University,
//  All rights reserved.
//
//  This software may be modified and distributed under the terms of the BSD
//  license. See the LICENSE file for details.
//
//  This software was created at the Indiana University Center for Research in
//  Extreme Scale Technologies (CREST).
// =============================================================================


#include "builtins/singlelocdistro.h"


namespace dashmm {


void SingleLocality::compute_distribution(DAG &dag) {
  for (auto i: dag.source_nodes) {
    i->locality = 0;
  }

  for (auto i: dag.target_nodes) {
    i->locality = 0;
  }
}


}
