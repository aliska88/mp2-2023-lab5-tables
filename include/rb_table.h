#pragma once
#include "table.h"

class RBTable : public ITable {
  enum Color { RED, BLACK };
  struct Node {
    std::string key;
    Polynomial value;
    Color color;
    Node *left, *right, *parent;
    Node(const std::string& k, const Polynomial& v, Color c, Node* nil)
        : key(k), value(v), color(c), left(nil), right(nil), parent(nil) {}
  };

  Node* nil_;
  Node* root_;
  int size_;

  void rotL(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil_) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil_)
      root_ = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rotR(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nil_) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nil_)
      root_ = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void fixInsert(Node* z) {
    while (z->parent->color == RED) {
      if (z->parent == z->parent->parent->left) {
        Node* y = z->parent->parent->right;
        if (y->color == RED) {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        } else {
          if (z == z->parent->right) {
            z = z->parent;
            rotL(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          rotR(z->parent->parent);
        }
      } else {
        Node* y = z->parent->parent->left;
        if (y->color == RED) {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        } else {
          if (z == z->parent->left) {
            z = z->parent;
            rotR(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          rotL(z->parent->parent);
        }
      }
    }
    root_->color = BLACK;
  }

  void transplant(Node* u, Node* v) {
    if (u->parent == nil_)
      root_ = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    v->parent = u->parent;
  }

  Node* minimum(Node* x) {
    while (x->left != nil_) x = x->left;
    return x;
  }

  void fixDelete(Node* x) {
    while (x != root_ && x->color == BLACK) {
      if (x == x->parent->left) {
        Node* w = x->parent->right;
        if (w->color == RED) {
          w->color = BLACK;
          x->parent->color = RED;
          rotL(x->parent);
          w = x->parent->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK) {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->right->color == BLACK) {
            w->left->color = BLACK;
            w->color = RED;
            rotR(w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          w->right->color = BLACK;
          rotL(x->parent);
          x = root_;
        }
      } else {
        Node* w = x->parent->left;
        if (w->color == RED) {
          w->color = BLACK;
          x->parent->color = RED;
          rotR(x->parent);
          w = x->parent->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK) {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->left->color == BLACK) {
            w->right->color = BLACK;
            w->color = RED;
            rotL(w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          w->left->color = BLACK;
          rotR(x->parent);
          x = root_;
        }
      }
    }
    x->color = BLACK;
  }

  void freeTree(Node* n) {
    if (n == nil_) return;
    freeTree(n->left);
    freeTree(n->right);
    delete n;
  }

  void collectKeys(Node* n, std::vector<std::string>& ks) const {
    if (n == nil_) return;
    collectKeys(n->left, ks);
    ks.push_back(n->key);
    collectKeys(n->right, ks);
  }

 public:
  RBTable() : size_(0) {
    nil_ = new Node("", Polynomial(), BLACK, nullptr);
    nil_->left = nil_->right = nil_->parent = nil_;
    root_ = nil_;
  }

  ~RBTable() {
    freeTree(root_);
    delete nil_;
  }

  std::string name() const override { return "Красно-чёрное дерево"; }
  int size() const override { return size_; }

  void insert(const std::string& key, const Polynomial& val,
              int& ops) override {
    ops = 0;
    Node* cur = root_;
    Node* par = nil_;
    while (cur != nil_) {
      ops++;
      par = cur;
      if (key < cur->key)
        cur = cur->left;
      else if (key > cur->key)
        cur = cur->right;
      else {
        cur->value = val;
        ops++;
        return;
      }
    }
    Node* z = new Node(key, val, RED, nil_);
    z->parent = par;
    if (par == nil_)
      root_ = z;
    else if (key < par->key)
      par->left = z;
    else
      par->right = z;
    size_++;
    ops++;
    fixInsert(z);
  }

  bool find(const std::string& key, Polynomial& val, int& ops) override {
    ops = 0;
    Node* cur = root_;
    while (cur != nil_) {
      ops++;
      if (key < cur->key)
        cur = cur->left;
      else if (key > cur->key)
        cur = cur->right;
      else {
        val = cur->value;
        return true;
      }
    }
    return false;
  }

  bool remove(const std::string& key, int& ops) override {
    ops = 0;
    Node* z = root_;
    while (z != nil_) {
      ops++;
      if (key < z->key)
        z = z->left;
      else if (key > z->key)
        z = z->right;
      else
        break;
    }
    if (z == nil_) return false;

    Node* y = z;
    Node* x;
    Color origColor = y->color;

    if (z->left == nil_) {
      x = z->right;
      transplant(z, z->right);
    } else if (z->right == nil_) {
      x = z->left;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      origColor = y->color;
      x = y->right;
      if (y->parent == z) {
        x->parent = y;
      } else {
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    delete z;
    size_--;
    ops++;
    if (origColor == BLACK) fixDelete(x);
    return true;
  }

  std::vector<std::string> get_keys() const override {
    std::vector<std::string> ks;
    collectKeys(root_, ks);
    return ks;
  }
};
