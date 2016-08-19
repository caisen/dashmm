#include "tree.h"

// TODO - this will all change

hpx_action_t Node::partition_node_ = HPX_ACTION_NULL;
hpx_action_t DualTree::domain_geometry_init_ = HPX_ACTION_NULL;
hpx_action_t DualTree::domain_geometry_op_ = HPX_ACTION_NULL;
hpx_action_t DualTree::set_domain_geometry_ = HPX_ACTION_NULL;
hpx_action_t DualTree::init_partition_ = HPX_ACTION_NULL;
hpx_action_t DualTree::merge_points_ = HPX_ACTION_NULL;
hpx_action_t DualTree::recv_points_ = HPX_ACTION_NULL;
hpx_action_t DualTree::send_points_ = HPX_ACTION_NULL;
hpx_action_t DualTree::recv_node_ = HPX_ACTION_NULL;
hpx_action_t DualTree::send_node_ = HPX_ACTION_NULL;
hpx_action_t DualTree::create_dual_tree_ = HPX_ACTION_NULL;
hpx_action_t DualTree::finalize_partition_ = HPX_ACTION_NULL;

void registrar() {
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      Node::partition_node_,
                      Node::partition_node_handler,
                      HPX_POINTER, HPX_POINTER, HPX_INT);

  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::set_domain_geometry_,
                      DualTree::set_domain_geometry_handler,
                      HPX_ADDR, HPX_ADDR, HPX_ADDR);
  HPX_REGISTER_ACTION(HPX_FUNCTION, HPX_ATTR_NONE,
                      DualTree::domain_geometry_init_,
                      DualTree::domain_geometry_init_handler,
                      HPX_POINTER, HPX_SIZE_T);
  HPX_REGISTER_ACTION(HPX_FUNCTION, HPX_ATTR_NONE,
                      DualTree::domain_geometry_op_,
                      DualTree::domain_geometry_op_handler,
                      HPX_POINTER, HPX_POINTER, HPX_SIZE_T);

  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::init_partition_,
                      DualTree::init_partition_handler,
                      HPX_ADDR, HPX_ADDR, HPX_INT, HPX_ADDR);

  // tree creation stuff goes here
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::merge_points_,
                      DualTree::merge_points_handler,
                      HPX_POINTER, HPX_POINTER, HPX_INT, HPX_INT, HPX_CHAR,
                      HPX_ADDR);
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_MARSHALLED,
                      DualTree::recv_points_,
                      DualTree::recv_points_handler,
                      HPX_POINTER, HPX_SIZE_T);
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::send_points_,
                      DualTree::send_points_handler,
                      HPX_INT, HPX_POINTER, HPX_POINTER, HPX_POINTER,
                      HPX_POINTER, HPX_POINTER, HPX_POINTER, HPX_ADDR);
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_MARSHALLED,
                      DualTree::recv_node_,
                      DualTree::recv_node_handler,
                      HPX_POINTER, HPX_SIZE_T);
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::send_node_,
                      DualTree::send_node_handler,
                      HPX_POINTER, HPX_POINTER, HPX_INT, HPX_INT, HPX_ADDR);
  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::create_dual_tree_,
                      DualTree::create_dual_tree_handler,
                      HPX_ADDR, HPX_ADDR, HPX_ADDR);

  HPX_REGISTER_ACTION(HPX_DEFAULT, HPX_ATTR_NONE,
                      DualTree::finalize_partition_,
                      DualTree::finalize_partition_handler,
                      HPX_ADDR);
}
