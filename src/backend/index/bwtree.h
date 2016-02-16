//===----------------------------------------------------------------------===//
//
//                         PelotonDB
//
// BWTree.h
//
// Identification: src/backend/index/BWTree.h
//
// Copyright (c) 2015, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

namespace peloton {
namespace index {

typedef enum node_type {
  INTERNAL_BW_NODE,
  LEAF_BW_NODE,
  INSERT,
  UPDATE,
  DELETE,
  SPLIT,
  MERGE,
  NODE_DELETE,
  SPLIT_INDEX,
  DELETE_INDEX
} node_type_t;

// Look up the stx btree interface for background.
// peloton/third_party/stx/btree.h
template <typename KeyType, typename ValueType, class KeyComparator>
class BWTree {

  // Add your declarations here

};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class InternalBWNode {
  private:
  epoch_t generation;
  std::vector<std::pair<KeyType, uint64_t>> key_list; // all keys have children
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  uint64_t id;
  uint64_t sibling_id;
  InternalBWNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type;
  bool insert(uint64_t id, KeyType split_key, KeyType boundary_key, uint64_t new_node_id);
  bool split(uint64_t id, uint64_t *path, uint64_t index, KeyType split_key, KeyType boundary_key, uint64_t new_node_id);
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class LeafBWNode {
  private:
  epoch_t generation;
  uint64_t sibling_id;
  std::vector<std::pair<KeyType, ValueType>> kv_list; // all key value pairs
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to
  node_type_t type;

  public:
  uint64_t id;
  LeafBWNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t get_node_type();
  uint64_t get_size();
  bool insert(KeyType key, ValueType value);
  bool split_node(uint64_t id, uint64_t path, uint64_t index, KeyType key, ValueType value);
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class SimpleDeltaNode {
  private:
  epoch_t generation;
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  uint64_t id;
  void *next; // can be delta node or internal_bw_node or leaf_bw_node
  KeyType key;
  ValueType val;
  SimpleDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type; // delete / insert / update
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class SplitIndexDeltaNode {
	private:
  epoch_t generation;
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  SplitIndexDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  uint64_t id;
  node_type_t type;
  KeyType split_key, boundary_key;
  void *next; // can be delta node or internal_bw_node
  uint64_t new_split_node_id;
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class DeleteIndexDeltaNode {
  private:
  epoch_t generation;
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to
 
  public:
  uint64_t id;
  DeleteIndexDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type;
  KeyType deleted_key;
  void *next; // can be delta node or internal_bw_node
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class SplitDeltaNode {
  epoch_t generation;
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  uint64_t id;
  SplitDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type;
  KeyType split_key;
  void *next; // can be delta node or internal_bw_node or leaf_bw_node
  uint64_t target_node_id; // pointer to target node for preventing blocking
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class RemoveDeltaNode {
  epoch_t generation;
  KeyType deleted_key; 
  void *node_to_be_removed; // can be delta node or internal_bw_node or leaf_bw_node
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  uint64_t id;
  RemoveDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type;
};

template <typename KeyType, typename ValueType, class KeyComparator, class KeyEqualityChecker>
class MergeDeltaNode {
  epoch_t generation;
  BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *my_tree; // reference of the tree I belong to

  public:
  uint64_t id;
  MergeDeltaNode(BWTreeIndex<KeyType, ValueType, KeyComparator, KeyEqualityChecker> *bwt, uint64_t id);
  node_type_t type;
  KeyType MergeKey;
  void *next; // can be delta node or internal_bw_node or leaf_bw_node
  void *node_to_be_merged;
};
}  // End index namespace
}  // End peloton namespace
