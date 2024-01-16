#pragma once

#include "generator.hpp"

#include <memory>
#include <utility>

// This is a naïve implementation of binary search tree.

template <class K, class V> struct bst_node;

template <class K, class V> using bst = std::shared_ptr<bst_node<K, V>>;

template <class K, class V> struct bst_node {
  const bst<K, V> lhs, rhs;
  const K key;
  const V value;
};

template <class K, class V> bst<K, V> empty() { return nullptr; }

template <class K, class V>
bst<K, V> put(const bst<K, V> &tree, K &&key, V &&value) {
  if (tree)
    if (key < tree->key)
      return std::make_shared<bst_node<K, V>>(
          put(tree->lhs, std::forward<K>(key), std::forward<V>(value)),
          tree->rhs,
          tree->key,
          tree->value);
    else if (tree->key < key)
      return std::make_shared<bst_node<K, V>>(
          tree->lhs,
          put(tree->rhs, std::forward<K>(key), std::forward<V>(value)),
          tree->key,
          tree->value);
    else
      return std::make_shared<bst_node<K, V>>(
          tree->lhs, tree->rhs, std::forward<K>(key), std::forward<V>(value));
  else
    return std::make_shared<bst_node<K, V>>(
        nullptr, nullptr, std::forward<K>(key), std::forward<V>(value));
}

// Using a recursive generator it is now trivial to provide traversals over a
// BST.

template <class K, class V>
generator<std::pair<K, V>> in_order(const bst<K, V> &node) {
  if (node) {
    co_yield in_order(node->lhs);
    co_yield std::pair{node->key, node->value};
    co_yield in_order(node->rhs);
  }
}

template <class K, class V>
generator<std::pair<K, V>> pre_order(const bst<K, V> &node) {
  if (node) {
    co_yield std::pair{node->key, node->value};
    co_yield pre_order(node->lhs);
    co_yield pre_order(node->rhs);
  }
}

template <class K, class V>
generator<std::pair<K, V>> post_order(const bst<K, V> &node) {
  if (node) {
    co_yield post_order(node->lhs);
    co_yield post_order(node->rhs);
    co_yield std::pair{node->key, node->value};
  }
}
