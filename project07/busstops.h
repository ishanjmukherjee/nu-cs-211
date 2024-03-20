/*busstops.h*/

//
// A collection of bus stops in the Open Street Map.
// 
// Ishan Mukherjee
// 

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "busstop.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class BusStops
{
public:
  vector<BusStop> MapBusStops;

  //
  // constructor
  //
  BusStops(string filename);

  //
  // print
  //
  // prints each bus stop to the console.
  //
  void print();

  //
  // accessors / getters
  //
  int getNumMapBusStops();
};