/*multiset.h*/

//
// Implements a multiset with duplicates.
//
// Ishan Mukherjee
// CS 211
// Northwestern University
//
// Original template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

// my note: this note still applies to std::multiset
//
// NOTE: because our set has the same name as std::set
// in the C++ standard template library (STL), we cannot
// do the following:
// 
//   using namespace std;
//
// This implies refernces to the STL will need to use
// the "std::" prefix, e.g. std::cout, std::endl, and
// std::vector.
//
#pragma once

#include <iostream>
#include <vector>
#include <utility>  // std::pair
#include <cassert>


template <typename TKey>
class multiset
{
public:
  // #################################################################
  //
  // A node in the search tree:
  //
  class NODE {
  private:
    vector<TKey> Keys;
    bool  isThreaded : 1; // 1 bit
    NODE* Left;
    NODE* Right;

  public:
    // constructor:
    NODE(TKey key)
      : isThreaded(false), Left(nullptr), Right(nullptr)
    {
      this->Keys.push_back(key);
    }

    // getters:
    TKey         get_RepresentativeKey() { return this->Keys[0]; }
    vector<TKey> get_Keys() { return this->Keys; }
    bool         get_isThreaded() { return this->isThreaded; }
    NODE*        get_Left() { return this->Left; }

    // NOTE: this ignores the thread, call to perform "normal" traversals
    NODE* get_Right() {
      if (this->isThreaded)
        return nullptr;
      else
        return this->Right;
    }

    // always gets the node to the right
    NODE* get_Thread() {
      return this->Right;
    }

    // setters:
    void set_isThreaded(bool threaded) { this->isThreaded = threaded; }
    void set_Left(NODE* left) { this->Left = left; }
    void set_Keys(vector<TKey> keys) { this->Keys = keys; }
    void set_Right(NODE* right) { this->Right = right; }
    void addToKeys(TKey key) { this->Keys.push_back(key); }
  };


  // #################################################################
  //
  // set data members:
  //
  NODE* Root; // pointer to root node
  int Size;   // # of nodes in tree


  // #################################################################
  //
  // set methods:
  //
public:
  //
  // default constructor:
  //
  multiset()
    : Root(nullptr), Size(0)
  { }

  // 
  // copy constructor:
  //
private:
  void _copy(NODE* other)
  {
    if (other == nullptr)
      return;
    else {
      //
      // we make a copy using insert so that threads
      // are recreated properly in the copy:
      //
      vector<TKey> keys = other->get_Keys();
      for (TKey key : keys) {
        this->insert(key);
      }
      
      // code from the age of vanilla sets
      // this->insert(other->get_RepresentativeKey());

      _copy(other->get_Left());
      _copy(other->get_Right());
    }
  }

public:
  multiset(const multiset& other)
    : Root(nullptr), Size(0)
  {
    _copy(other.Root);
  }

  //
  // destructor:
  //
private:
  void _destroy(NODE* cur)
  {
    if (cur == nullptr)
      ;
    else {
      _destroy(cur->get_Left());
      _destroy(cur->get_Right());
      delete cur;
    }
  }

public:
  ~multiset()
  {
    //
    // NOTE: this is commented out UNTIL you are ready. The last 
    // step is to uncomment this and check for memory leaks.
    //
    _destroy(this->Root);
  }

  //
  // size 
  // 
  // Returns # of elements in the multiset
  //
  int size()
  {
    return this->Size;
  }

  //
  // contains
  // 
  // Returns true if multiset contains key, false if not
  //
private:
  bool _contains(NODE* cur, TKey key)
  {
    if (cur == nullptr)
      return false;
    else {

      if (key < cur->get_RepresentativeKey()) // search left:
        return _contains(cur->get_Left(), key);
      else if (cur->get_RepresentativeKey() < key) // search right:
        return _contains(cur->get_Right(), key);
      else // must be equal, found it!
        return true;
    }
  }

public:
  bool contains(TKey key)
  {
    return _contains(this->Root, key);
  }

  int count(TKey key) {
    NODE* cur = this->Root;

    while (cur != nullptr) {
      if (key < cur->get_RepresentativeKey()) {
        cur = cur->get_Left();
      }
      else if (cur->get_RepresentativeKey() < key) {
        cur = cur->get_Right();
      }
      else {
        return cur->get_Keys().size();
      }
    }
    return 0;
  }

  //
  // insert
  // 
  // Inserts the given key into the multiset.
  //
  void insert(TKey key)
  {
    NODE* parent = nullptr;
    NODE* cur = this->Root;
  
    //
    // 1. Search for key, return if found:
    //
    while (cur != nullptr) {
      if (key < cur->get_RepresentativeKey()) { // left:
        parent = cur;
        cur = cur->get_Left();
      }
      else if (cur->get_RepresentativeKey() < key) { // right:
        parent = cur;
        cur = cur->get_Right();
      }
      else { // must be equal => already in tree
        cur->addToKeys(key);
        this->Size++;
        return;
      }
    }
  
    //
    // 2. If not found, insert where we
    //    fell out of the tree:
    //
    NODE* n = new NODE(key);
  
    if (parent == nullptr) {
      //
      // tree is empty, insert at root:
      //
      this->Root = n;
    }
    else if (key < parent->get_RepresentativeKey()) {
      //
      // we are to the left of our parent:
      //
      parent->set_Left(n);
      n->set_isThreaded(true);
      n->set_Right(parent);
    }
    else {
      //
      // we are to the right of our parent:
      //
      // inherit parent's thread
      n->set_isThreaded(parent->get_isThreaded());
      n->set_Right(parent->get_Thread());
      // update parent's thread
      parent->set_isThreaded(false);
      parent->set_Right(n);
    }
  
    //
    // STEP 3: update size and return
    //
    this->Size++;
    return;
  }

  //
  // toPairs
  //
  // Returns pairs of elements: <element, threaded element>.
  // If a node is not threaded: <element, no_element value>.
  //
private:
  void _toPairs(NODE* cur, std::vector<std::pair<TKey, TKey>>& P, TKey no_element) {
    if (cur == nullptr) {
      return;
    } else {
      //
      // we want them in order, so go left, then
      // middle, then right:
      //
      
      _toPairs(cur->get_Left(), P, no_element);
      
      // determine threadValue
      TKey threadValue; 
      if (cur->get_isThreaded()) {
        threadValue = cur->get_Thread()->get_RepresentativeKey();
      } else {
        threadValue = no_element;
      }

      P.push_back(std::make_pair(cur->get_RepresentativeKey(), threadValue));
      
      _toPairs(cur->get_Right(), P, no_element);
    }
  }
public:
  std::vector<std::pair<TKey, TKey>> toPairs(TKey no_element)
  {
    std::vector<std::pair<TKey, TKey>> P;

    _toPairs(this->Root, P, no_element);

    return P;
  }

  //
  // []
  // 
  // Returns true if multiset contains key, false if not.
  //
  bool operator[](TKey key)
  {
    return this->contains(key);
  }

  //
  // toVector
  //
  // Returns the elements of the multiset, in order, 
  // in a vector.
  //
private:
  void _toVector(NODE* cur, std::vector<TKey>& V) {
    if (cur == nullptr)
      return;
    else {
      //
      // we want them in order, so go left, then
      // middle, then right:
      //
      _toVector(cur->get_Left(), V);
      
      // old code for no duplicates:
      // V.push_back(cur->get_Key()); (should be cur->get_RepresentativeKey() though)
      
      // append keys to V
      vector<TKey> keys = cur->get_Keys();
      V.insert(V.end(), keys.begin(), keys.end());
      _toVector(cur->get_Right(), V);
    }
  }

public:
  std::vector<TKey> toVector()
  {
    std::vector<TKey> V;

    _toVector(this->Root, V);

    return V;
  }


  // #################################################################
  //
  // class iterator:
  //
private:
  class iterator
  {
  private:
    NODE* Ptr;
    int KeyIdx;

  public:
  
    iterator(NODE* ptr) : Ptr(ptr), KeyIdx(0) {}

    //
    // !=
    //
    // Returns true if the given iterator is not equal to
    // this iterator.
    //
    bool operator!=(iterator other) {
      return (this->Ptr != other.Ptr || this->KeyIdx != other.KeyIdx);
    }

    //
    // ++
    //
    // Advances the iterator to the next ordered element of
    // the multiset; if the iterator cannot be advanced, ++ has
    // no effect.
    //
    void operator++() {
      // if vector of keys still not exhausted
      if (this->KeyIdx + 1 < (int) this->Ptr->get_Keys().size()) {
        this->KeyIdx++;
        return;
      }

      // else
      this->KeyIdx = 0;
      if (Ptr->get_isThreaded()) {
        this->Ptr = Ptr->get_Thread();
      } else {
        this->Ptr = leftmost(Ptr->get_Right());
      }
      return;
    }

    //
    // *
    //
    // Returns the key denoted by the iterator; this
    // code will throw an out_of_range exception if 
    // the iterator does not denote an element of the 
    // multiset.
    //
    TKey operator*()
    {
      if (this->Ptr == nullptr)
        throw std::out_of_range("multiset::iterator:operator*");

      return this->Ptr->get_Keys()[this->KeyIdx];
    }

    //
    // ==
    //
    // Returns true if the given iterator is equal to
    // this iterator.
    //
    bool operator==(iterator other)
    {
      return (this->Ptr == other.Ptr && this->KeyIdx == other.KeyIdx);
    }
  };

private:
  // helper function to return leftmost child of subtree
  static NODE* leftmost(NODE* root) {
    NODE* current = root;
    while (current && current->get_Left()) {
      current = current->get_Left();
    }
    return current; 
  }
  static NODE* rightmost(NODE* root) {
    NODE* current = root;
    while (current && current->get_Right()) {
      current = current->get_Right();
    }
    return current; 
  }

public:

  iterator begin() const {
    return iterator(leftmost(this->Root));
  }

  // #################################################################
  //
  // find:
  //
  // If the multiset contains key, then an iterator denoting this
  // element is returned. If the multiset does not contain key, 
  // then multiset.end() is returned.
  //
public:
  iterator find(TKey key)
  {
    NODE* cur = this->Root;

    while (cur != nullptr) {
      if (key < cur->get_RepresentativeKey()) { // search left:
        cur = cur->get_Left();
      }
      else if (cur->get_RepresentativeKey() < key) { // search right:
        cur = cur->get_Right();
      }
      else { // must be equal, found it!
        return iterator(cur);
      }
    }

    // if get here, not found
    return iterator(nullptr);
  }

  void erase(TKey key) {
    NODE* parent = nullptr;
    NODE* cur = this->Root;
  
    //
    // 1. Search for key, set cur to it if found:
    //
    while (cur != nullptr) {
      if (key < cur->get_RepresentativeKey()) { // left:
        parent = cur;
        cur = cur->get_Left();
      }
      else if (cur->get_RepresentativeKey() < key) { // right:
        parent = cur;
        cur = cur->get_Right();
      }
      else { // must be equal => already in tree
        break;
      }
    }
  
    //
    // 2. If found, delete cur:
    //
  
    if (parent == nullptr || cur == nullptr) {
      //
      // tree is empty or not found, return:
      //
      return;
    }

    // else
    this->Size -= cur->get_Keys().size();

    NODE* new_child = nullptr;

    if (!cur->get_Left() && !cur->get_Right()) {
      // cur has no children
      // child remains nullptr
    }
    else if (cur->get_Left() && !cur->get_Right()) {
      // left child but no right child
      NODE* rightmost_of_left_subtree = rightmost(cur->get_Left());
      // set thread to parent of cur
      rightmost_of_left_subtree->set_Right(cur->get_Thread());
      new_child = cur->get_Left();
    }
    else if (!cur->get_Left() && cur->get_Right()) {
      // right child but no left child
      new_child = cur->get_Right();
    }
    else {
      // both left and right children
      
      // find leftmost of right subtree (cur_r_st)
      NODE* leftmost_of_right_subtree = leftmost(cur->get_Left());
      cur->set_Keys(leftmost_of_right_subtree->get_Keys());
      erase(leftmost_of_right_subtree->get_RepresentativeKey());
    }

    if (key < parent->get_RepresentativeKey()) {
      // we are to the left of our parent
      
      parent->set_Left(new_child);
      delete cur;
    }
    else {
      // right of parent 

      parent->set_Right(new_child);
      delete cur;
    }
  
    //
    // STEP 3: return
    //
    return;
  }

  //
  // end:
  //
  // Returns an iterator to the end of the iteration space,
  // i.e. to no element. In other words, if your iterator
  // == multiset.end(), then you are not pointing to an element.
  //
  iterator end()
  {
    return iterator(nullptr);
  }

};
