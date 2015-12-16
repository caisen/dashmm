#include <cstring>

#include <memory>

#include <hpx/hpx.h>

#include "include/array.h"
#include "include/expansion.h"
#include "include/method.h"
#include "include/methodref.h"
#include "include/node.h"
#include "include/types.h"


namespace dashmm {


/////////////////////////////////////////////////////////////////////
// Utility routines
/////////////////////////////////////////////////////////////////////


DomainGeometry cubify_domain(hpx_addr_t source_bounds,
                             hpx_addr_t target_bounds) {
  double s_bounds[6];
  hpx_lco_get(source_bounds, sizeof(double) * 6, s_bounds);

  double t_bounds[6];
  hpx_lco_get(target_bounds, sizeof(double) * 6, t_bounds);

  //TODO Can't I use the DomainGeometry Constructor here?
  //cubify the domain
  Point low{s_bounds[0] < t_bounds[0] ? s_bounds[0] : t_bounds[0],
            s_bounds[1] < t_bounds[1] ? s_bounds[1] : t_bounds[1],
            s_bounds[2] < t_bounds[2] ? s_bounds[2] : t_bounds[2]};
  Point high{s_bounds[3] > t_bounds[3] ? s_bounds[3] : t_bounds[3],
             s_bounds[4] > t_bounds[4] ? s_bounds[4] : t_bounds[4],
             s_bounds[5] > t_bounds[5] ? s_bounds[5] : t_bounds[5]};
  Point center = point_add(low.scale(0.5), high.scale(0.5));
  Point sizes = point_sub(high, low);
  double size = sizes.x() > sizes.y() ? sizes.x() : sizes.y();
  size = size > sizes.z() ? size : sizes.z();
  size *= 0.5001;
  Point offset{-size, -size, -size};
  low = point_add(center, offset);
  size *= 2.0;

  return DomainGeometry{low, size};
}


/////////////////////////////////////////////////////////////////////
// Actions
/////////////////////////////////////////////////////////////////////


struct PackDataResult {
  hpx_addr_t packed;
  int count;
};

int pack_sources_handler(hpx_addr_t user_data, int pos_offset, int q_offset) {
  //NOTE: SMP assumptions all over this function
  ArrayMetaData *meta{nullptr};
  assert(hpx_gas_try_pin(user_data, (void **)&meta));

  char *user{nullptr};
  assert(hpx_gas_try_pin(meta->data, (void **)&user));

  PackDataResult retval{};
  retval.packed =
      hpx_gas_alloc_local_at_sync(1, meta->count * sizeof(Source), 0, HPX_HERE);
  retval.count = meta->count;

  if (retval.packed != HPX_NULL) {
    Source *sources{nullptr};
    assert(hpx_gas_try_pin(retval.packed, (void **)&sources));

    for (size_t i = 0; i < meta->count; ++i) {
      void *pos_base = static_cast<void *>(&user[i * meta->size] + pos_offset);
      double *pos = static_cast<double *>(pos_base);
      void *q_base = static_cast<void *>(&user[i * meta->size] + q_offset);
      double *q = static_cast<double *>(q_base);
      sources[i].position = Point{pos[0], pos[1], pos[2]};
      sources[i].charge = *q;
    }

    hpx_gas_unpin(retval.packed);
  }

  hpx_gas_unpin(meta->data);
  hpx_gas_unpin(user_data);

  HPX_THREAD_CONTINUE(retval);
}
HPX_ACTION(HPX_DEFAULT, 0,
           pack_sources_action, pack_sources_handler,
           HPX_ADDR, HPX_INT, HPX_INT);


int pack_targets_handler(hpx_addr_t user_data, int pos_offset) {
  //NOTE: SMP assumptions
  ArrayMetaData *meta{nullptr};
  assert(hpx_gas_try_pin(user_data, (void **)&meta));

  char *user{nullptr};
  assert(hpx_gas_try_pin(meta->data, (void **)&user));

  PackDataResult retval{};
  retval.packed =
      hpx_gas_alloc_local_at_sync(1, meta->count * sizeof(Target), 0, HPX_HERE);
  retval.count = meta->count;
  if (retval.packed != HPX_NULL) {
    Target *targets{nullptr};
    assert(hpx_gas_try_pin(retval.packed, (void **)&targets));

    for (size_t i = 0; i < meta->count; ++i) {
      void *pos_base = static_cast<void *>(&user[i * meta->size] + pos_offset);
      double *pos = static_cast<double *>(pos_base);
      targets[i].position = Point{pos[0], pos[1], pos[2]};
      targets[i].index = i;
    }

    hpx_gas_unpin(retval.packed);
  }

  hpx_gas_unpin(meta->data);
  hpx_gas_unpin(user_data);

  HPX_THREAD_CONTINUE(retval);
}
HPX_ACTION(HPX_DEFAULT, 0,
           pack_targets_action, pack_targets_handler,
           HPX_ADDR, HPX_INT);


int find_source_domain_handler(hpx_addr_t packed) {
  //NOTE: SMP assumptions
  PackDataResult packed_data;
  hpx_lco_get(packed, sizeof(packed_data), &packed_data);
  //now pin the sources
  Source *sources{nullptr};
  assert(hpx_gas_try_pin(packed_data.packed, (void **)&sources));

  //These are the three low bounds, followed by the three high bounds
  double bounds[6]{1.0e34, 1.0e34, 1.0e34, -1.0e34, -1.0e34, -1.0e34};

  for (int i = 0; i < packed_data.count; ++i) {
    Point p = sources[i].position;
    if (p.x() < bounds[0]) bounds[0] = p.x();
    if (p.x() > bounds[3]) bounds[3] = p.x();
    if (p.y() < bounds[1]) bounds[1] = p.y();
    if (p.y() > bounds[4]) bounds[4] = p.y();
    if (p.z() < bounds[2]) bounds[2] = p.z();
    if (p.z() > bounds[5]) bounds[5] = p.z();
  }

  hpx_gas_unpin(packed_data.packed);

  hpx_thread_continue(bounds, sizeof(double) * 6);
}
HPX_ACTION(HPX_DEFAULT, 0,
           find_source_domain_action, find_source_domain_handler,
           HPX_ADDR);


int find_target_domain_handler(hpx_addr_t packed) {
  //NOTE: SMP assumptions
  PackDataResult packed_data;
  hpx_lco_get(packed, sizeof(packed_data), &packed_data);
  //now pin the sources
  Target *targets{nullptr};
  assert(hpx_gas_try_pin(packed_data.packed, (void **)&targets));

  //These are the three low bounds, followed by the three high bounds
  double bounds[6]{1.0e34, 1.0e34, 1.0e34, -1.0e34, -1.0e34, -1.0e34};

  for (int i = 0; i < packed_data.count; ++i) {
    Point p = targets[i].position;
    if (p.x() < bounds[0]) bounds[0] = p.x();
    if (p.x() > bounds[3]) bounds[3] = p.x();
    if (p.y() < bounds[1]) bounds[1] = p.y();
    if (p.y() > bounds[4]) bounds[4] = p.y();
    if (p.z() < bounds[2]) bounds[2] = p.z();
    if (p.z() > bounds[5]) bounds[5] = p.z();
  }

  hpx_gas_unpin(packed_data.packed);

  hpx_thread_continue(bounds, sizeof(double) * 6);
}
HPX_ACTION(HPX_DEFAULT, 0,
           find_target_domain_action, find_target_domain_handler,
           HPX_ADDR);


struct EvaluateParams {
  hpx_addr_t sources;
  int spos_offset;
  int q_offset;
  hpx_addr_t targets;
  int tpos_offset;
  int phi_offset;
  int refinement_limit;
  size_t method_size;
  size_t expansion_size;
  char data[];
};

int evaluate_handler(EvaluateParams *parms, size_t total_size) {
  //copy user data into internal data
  hpx_addr_t source_packed = hpx_lco_future_new(sizeof(PackDataResult));
  assert(source_packed != HPX_NULL);
  hpx_addr_t target_packed = hpx_lco_future_new(sizeof(PackDataResult));
  assert(target_packed != HPX_NULL);

  hpx_call(parms->sources, pack_sources_action, source_packed,
           &parms->sources, &parms->spos_offset, &parms->q_offset);
  hpx_call(parms->targets, pack_targets_action, target_packed,
           &parms->targets, &parms->tpos_offset);

  hpx_addr_t source_bounds = hpx_lco_future_new(sizeof(double) * 6);
  assert(source_bounds != HPX_NULL);
  hpx_addr_t target_bounds = hpx_lco_future_new(sizeof(double) * 6);
  assert(target_bounds != HPX_NULL);

  hpx_call_when(source_packed, parms->sources, find_source_domain_action,
                source_bounds, &source_packed);
  hpx_call_when(target_packed, parms->targets, find_target_domain_action,
                target_bounds, &target_packed);

  //create our method and expansion from the parameters
  MethodSerial *method_serial = reinterpret_cast<MethodSerial *>(parms->data);
  auto local_method = create_method(method_serial->type, method_serial);
  MethodRef method = globalize_method(std::move(local_method), HPX_HERE);

  char *expansion_base = parms->data + parms->method_size;
  int *type = reinterpret_cast<int *>(expansion_base + sizeof(int));
  auto local_expansion =
      interpret_expansion(*type, expansion_base, parms->expansion_size);
  ExpansionRef expansion =
      globalize_expansion(std::move(local_expansion), HPX_HERE);

  //collect results of actions
  PackDataResult res{};
  hpx_lco_get(source_packed, sizeof(res), &res);
  hpx_lco_delete_sync(source_packed);
  SourceRef sources{res.packed, res.count};

  hpx_lco_get(target_packed, sizeof(res), &res);
  hpx_lco_delete_sync(target_packed);
  TargetRef targets{res.packed, res.count};

  DomainGeometry root_vol = cubify_domain(source_bounds, target_bounds);
  hpx_lco_delete_sync(source_bounds);
  hpx_lco_delete_sync(target_bounds);

  //build trees/do work - NOTE the awkwardness with source reference... This
  // really ought to be improved.
  SourceNode source_root{root_vol, Index{0, 0, 0, 0}, method.data(), nullptr};
  Source *source_parts{nullptr};
  assert(hpx_gas_try_pin(sources.data(), (void **)&source_parts));
  hpx_addr_t partitiondone =
      source_root.partition(source_parts, sources.n(), parms->refinement_limit,
                            expansion.type(), expansion.data());
  hpx_gas_unpin(sources.data());

  TargetNode target_root{root_vol, Index{0, 0, 0, 0}, method.data(), nullptr};
  hpx_lco_wait(partitiondone);
  hpx_lco_delete_sync(partitiondone);
  bool same_sandt = (parms->sources == parms->targets);
  target_root.partition(targets.data(), targets.n(), parms->refinement_limit,
                        expansion, 0, same_sandt, std::vector<SourceNode>{});

  //copy results back into user data
  target_root.collect_results(parms->targets, parms->phi_offset);

  //clean up
  source_root.destroy();
  target_root.destroy();

  //return
  hpx_exit(HPX_SUCCESS);
}
HPX_ACTION(HPX_DEFAULT, HPX_MARSHALLED,
           evaluate_action, evaluate_handler,
           HPX_POINTER, HPX_SIZE_T);


/////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////


ReturnCode evaluate(ObjectHandle sources, int spos_offset, int q_offset,
                    ObjectHandle targets, int tpos_offset, int phi_offset,
                    int refinement_limit,
                    std::unique_ptr<Method> method,
                    std::unique_ptr<Expansion> expansion) {
  if (!method->compatible_with(expansion.get())) {
    return kIncompatible;
  }

  //Marshall our arguments
  size_t method_size = method->bytes();
  MethodSerial *method_serial = method->release();
  size_t expansion_size = expansion->bytes();
  char *expansion_serial = static_cast<char *>(expansion->release());
  size_t total_size = method_size + expansion_size + sizeof(EvaluateParams);
  EvaluateParams *args = static_cast<EvaluateParams *>(malloc(total_size));
  assert(args);
  args->sources = sources;
  args->spos_offset = spos_offset;
  args->q_offset = q_offset;
  args->targets = targets;
  args->tpos_offset = tpos_offset;
  args->phi_offset = phi_offset;
  args->refinement_limit = refinement_limit;
  args->method_size = method_size;
  args->expansion_size = expansion_size;
  memcpy(args->data, method_serial, method_size);
  memcpy(args->data + method_size, expansion_serial, expansion_size);

  if (HPX_SUCCESS != hpx_run(&evaluate_action, args, total_size)) {
    return kRuntimeError;
  }

  return kSuccess;
}


} // namespace dashmm