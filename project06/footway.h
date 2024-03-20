/*footway.h*/

//
// A footway in the Open Street Map.
// 
// Ishan Mukherjee
// 

#pragma once

#include <string>
#include <vector>

#include <iostream>
#include "node.h"
#include "nodes.h"

using namespace std;

class Footway
{
public:
  long long ID;
  vector<long long> NodeIDs;

  //
  // constructor
  //
  Footway(long long id);

  //
  // adds the given nodeid to the end of the vector.
  //
  void add(long long nodeid);

  // void insert_into_intersections(vector<Footway>& intersections);
};