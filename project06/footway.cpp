/*footway.cpp*/

//
// A footway in the Open Street Map.
// 
// Ishan Mukherjee
// 

#include "footway.h"

using namespace std;

//
// constructor
//
Footway::Footway(long long id)
  : ID(id) {}


//
// adds the given nodeid to the end of the vector.
//
void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

