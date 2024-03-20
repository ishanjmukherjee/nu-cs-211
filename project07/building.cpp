/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include <iostream>

#include "building.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// print
// 
// prints information about a building --- id, name, etc. -- to
// the console. The function is passed the Nodes for searching 
// purposes.
//
void Building::print(CURL* curl, Nodes& nodes, BusStops& busstops)
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;
  cout << "# perimeter nodes: " << this->NodeIDs.size() << endl;
  pair<double, double> location = this->getLocation(nodes);
  cout << "Location: (" << location.first << ", " << location.second << ")" << endl;
  this->printAllClosestBusStops(curl, nodes, busstops);

  // cout << "Nodes:" << endl;
  // for (long long nodeid : this->NodeIDs)
  // {
  //   cout << "  " << nodeid << ": ";

  //   double lat = 0.0;
  //   double lon = 0.0;
  //   bool entrance = false;

  //   bool found = nodes.find(nodeid, lat, lon, entrance);

  //   if (found) {
  //     cout << "(" << lat << ", " << lon << ")";

  //     if (entrance)
  //       cout << ", is entrance";

  //     cout << endl;
  //   }
  //   else {
  //     cout << "**NOT FOUND**" << endl;
  //   }
  // }//for
}

//
// printAllClosestBusStops
//
void Building::printAllClosestBusStops(CURL* curl, Nodes& nodes, BusStops& busstops)
{
  this->printClosestBusStop(curl, nodes, busstops, "Southbound");
  this->printClosestBusStop(curl, nodes, busstops, "Northbound");
}

//
// printClosestBusStop
//
void Building::printClosestBusStop(CURL* curl, Nodes& nodes, BusStops& busstops, string direction)
{
  double this_lon = this->getLocation(nodes).first;
  double this_lat = this->getLocation(nodes).second;

  // lowercasing direction
  string direction_lower = direction;
  transform(direction_lower.begin(), direction_lower.end(), direction_lower.begin(),
    [](unsigned char c){ return std::tolower(c); });

  // printing closest bus stop message
  cout << "Closest " << direction_lower << " bus stop:" << endl;

  // initializing closest bus stop distance and index (in array of bus stops)
  double closestDist = numeric_limits<double>::infinity();
  int closestStopIndex = -1;

  // iterating over all bus stops
  for (int i = 0, n = busstops.MapBusStops.size(); i < n; i++)
  {
    double dist = distBetween2Points(this_lon, this_lat, busstops.MapBusStops[i].Location.first, busstops.MapBusStops[i].Location.second);
    // if right direction and closer than nearest bus stop found yet
    if (busstops.MapBusStops[i].Direction == direction &&  dist < closestDist)
    {
      closestStopIndex = i;
      closestDist = dist;
    }
  }

  // print closest bus stop with distance
  busstops.MapBusStops[closestStopIndex].printWithDist(closestDist);

  // print bus predictions for closest bus stop
  busstops.MapBusStops[closestStopIndex].printBusPredictions(curl);
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid); 
}

//
// gets the center (lat, lon) of the building based
// on the nodes that form the perimeter
//
pair<double, double> Building::getLocation(const Nodes& nodes)
{
  // initializing variables used inside loop
  double lat_sum, long_sum, cur_lat, cur_long;
  lat_sum = long_sum = cur_lat = cur_long = 0.0;
  bool entrance = false;
  
  // loop through all of the building's nodes
  for (long long id : this->NodeIDs)
  {
    if (!nodes.find(id, cur_lat, cur_long, entrance))
      cout << "node not found" << endl;
    lat_sum += cur_lat;
    long_sum += cur_long; 
  }
  
  // return avg latitude and longitude
  size_t num_nodes = this->NodeIDs.size(); 
  return make_pair(lat_sum / num_nodes, long_sum / num_nodes);
}

