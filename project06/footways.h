/*footways.h*/

//
// A collection of footways in the Open Street Map.
// 
// Ishan Mukherjee
// 

#pragma once

#include <vector>
#include <iostream>

#include "footway.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Footways
{
public:
  vector<Footway> MapFootways;

  //
  // readMapFootways
  //
  // Given an XML document, reads through the document and 
  // stores all the footways into the given vector.
  //
  void readMapFootways(XMLDocument& xmldoc);

  //
  // accessors / getters
  //
  int getNumMapFootways();
};