/*busstop.h*/

//
// A bus stop in the Open Street Map.
// 
// Ishan Mukherjee
// 

#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>
#include "curl_util.h"
#include "json.hpp"
using json = nlohmann::json;

#pragma once

using namespace std;

class BusStop
{
public:
  long long ID;
  long Route;
  string Name;
  string Direction;
  string StopLocation;
  pair<double, double> Location;

  //
  // constructor
  //
  BusStop(long long id, double route, string name, string direction, string stoplocation, pair<double, double> location);

  //
  // print
  //
  void print();

  //
  // printWithDist
  //
  void printWithDist(double dist);

  //
  // printWithDist
  //
  void printBusPredictions(CURL* curl);
};