/*set.h*/

//
// Implements a set in the mathematical sense, with no duplicates.
//
// Ishan Mukherjee
//
// Original template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

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
class set
{
public:
  // #################################################################
  //
  // A node in the search tree:
  //
  class NODE {
  private:
    TKey  Key;
    bool  isThreaded : 1; // 1 bit
    NODE* Left;
    NODE* Right;

  public:
    // constructor:
    NODE(TKey key)
      : Key(key), isThreaded(false), Left(nullptr), Right(nullptr)
    { }

    // getters:
    TKey  get_Key() { return this->Key; }
    bool  get_isThreaded() { return this->isThreaded; }
    NODE* get_Left() { return this->Left; }

    // NOTE: this ignores the thread, call to perform "normal" traversals
    NODE* get_Right() {
      if (this->isThreaded)
        return nullptr;
      else
        return this->Right;
    }

    NODE* get_Thread() {
      return this->Right;
    }

    // setters:
    void set_isThreaded(bool threaded) { this->isThreaded = threaded; }
    void set_Left(NODE* left) { this->Left = left; }
    void set_Right(NODE* right) { this->Right = right; }
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
  set()
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
      this->insert(other->get_Key());

      _copy(other->get_Left());
      _copy(other->get_Right());
    }
  }

public:
  set(const set& other)
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
  ~set()
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
  // Returns # of elements in the set
  //
  int size()
  {
    return this->Size;
  }

  //
  // contains
  // 
  // Returns true if set contains key, false if not
  //
private:
  bool _contains(NODE* cur, TKey key)
  {
    if (cur == nullptr)
      return false;
    else {

      if (key < cur->get_Key()) // search left:
        return _contains(cur->get_Left(), key);
      else if (cur->get_Key() < key) // search right:
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

  //
  // insert
  // 
  // Inserts the given key into the set; if the key is already in 
  // the set then this function has no effect.
  //
  void insert(TKey key)
  {
    NODE* prev = nullptr;
    NODE* cur = this->Root;
  
    //
    // 1. Search for key, return if found:
    //
    while (cur != nullptr) {
      if (key < cur->get_Key()) { // left:
        prev = cur;
        cur = cur->get_Left();
      }
      else if (cur->get_Key() < key) { // right:
        prev = cur;
        cur = cur->get_Right();
      }
      else { // must be equal => already in tree
        return;  // don't insert again
      }
    }
  
    //
    // 2. If not found, insert where we
    //    fell out of the tree:
    //
    NODE* n = new NODE(key);
  
    if (prev == nullptr) {
      //
      // tree is empty, insert at root:
      //
      this->Root = n;
    }
    else if (key < prev->get_Key()) {
      //
      // we are to the left of our parent:
      //
      prev->set_Left(n);
      n->set_isThreaded(true);
      n->set_Right(prev);
    }
    else {
      //
      // we are to the right of our parent:
      //
      n->set_isThreaded(prev->get_isThreaded());
      n->set_Right(prev->get_Thread());
      prev->set_isThreaded(false);
      prev->set_Right(n);
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
      
      TKey threadValue; 
      if (cur->get_isThreaded()) {
        threadValue = cur->get_Thread()->get_Key();
      } else {
        threadValue = no_element;
      }
      P.push_back(std::make_pair(cur->get_Key(), threadValue));
      
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
  // Returns true if set contains key, false if not.
  //
  bool operator[](TKey key)
  {
    return this->contains(key);
  }

  //
  // toVector
  //
  // Returns the elements of the set, in order, 
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
      V.push_back(cur->get_Key());
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

  public:
  
    iterator(NODE* ptr) : Ptr(ptr) {}

    //
    // !=
    //
    // Returns true if the given iterator is not equal to
    // this iterator.
    //
    bool operator!=(iterator other) {
      return this->Ptr != other.Ptr;
    }

    //
    // ++
    //
    // Advances the iterator to the next ordered element of
    // the set; if the iterator cannot be advanced, ++ has
    // no effect.
    //
    void operator++() {
      if (Ptr->get_isThreaded()) {
        Ptr = Ptr->get_Thread();
      } else {
        Ptr = leftmost(Ptr->get_Right());
      }
      return;
    }

    //
    // *
    //
    // Returns the key denoted by the iterator; this
    // code will throw an out_of_range exception if 
    // the iterator does not denote an element of the 
    // set.
    //
    TKey operator*()
    {
      if (this->Ptr == nullptr)
        throw std::out_of_range("set::iterator:operator*");

      return this->Ptr->get_Key();
    }

    //
    // ==
    //
    // Returns true if the given iterator is equal to
    // this iterator.
    //
    bool operator==(iterator other)
    {
      if (this->Ptr == other.Ptr)
        return true;
      else
        return false;
    }
  };

public:
  // helper function to return leftmost child of subtree
  static NODE* leftmost(NODE* root) {
    NODE* current = root;
    while (current && current->get_Left() != nullptr) {
      current = current->get_Left();
    }
    return current;
  }

  iterator begin() const {
    return iterator(leftmost(this->Root));
  }

  // #################################################################
  //
  // find:
  //
  // If the set contains key, then an iterator denoting this
  // element is returned. If the set does not contain key, 
  // then set.end() is returned.
  //
public:
  iterator find(TKey key)
  {
    NODE* cur = this->Root;

    while (cur != nullptr) {
      if (key < cur->get_Key()) { // search left:
        cur = cur->get_Left();
      }
      else if (cur->get_Key() < key) { // search right:
        cur = cur->get_Right();
      }
      else { // must be equal, found it!
        return iterator(cur);
      }
    }

    // if get here, not found
    return iterator(nullptr);
  }

  //
  // end:
  //
  // Returns an iterator to the end of the iteration space,
  // i.e. to no element. In other words, if your iterator
  // == set.end(), then you are not pointing to an element.
  //
  iterator end()
  {
    return iterator(nullptr);
  }

};
