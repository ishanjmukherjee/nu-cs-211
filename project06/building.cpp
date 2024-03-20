
/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include "building.h"
#include <algorithm>

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
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//
// prints nodes connected to the building
//
void Building::print(const Nodes& nodes, const Footways& footways) const
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;

  cout << "Nodes: " << this->NodeIDs.size() << endl;
  for (long long nodeid : this->NodeIDs)
  {
    cout << " " << nodeid << ": ";

    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if (found) {
      cout << "(" << lat << ", " << lon << ")";

      if (entrance)
        cout << ", is entrance";

      cout << endl;
    }
    else {
      cout << "**NOT FOUND**" << endl;
    }
  }//for

  vector<Footway> footways_copy = footways.MapFootways;
  sort(footways_copy.begin(), footways_copy.end(), [] (const auto& lhs, const auto& rhs) {
    return lhs.ID < rhs.ID;});

  vector<Footway> intersections;
  for (Footway F : footways_copy)
  {
    this->add_intersection(F, intersections);
  }
  cout << "Footways that intersect: " << intersections.size() << endl;
  if (intersections.size() == 0)
  {
    cout << " None" << endl;
  }
  else
  {
    for (Footway F : intersections)
    {
      cout << " Footway " << F.ID << endl;
    }
  }
}

//
// adds a Footway to a list of intersections
//
void Building::add_intersection(Footway F, vector<Footway> &intersections) const
{
  // iterating over nodes in footway F
  for (long long nodeid : F.NodeIDs)
  {
    if (find(this->NodeIDs.begin(), this->NodeIDs.end(), nodeid) != this->NodeIDs.end())
    {
      intersections.emplace_back(F.ID);
      break;
    }
  }
}