/*busstops.cpp*/

//
// A collection of buildings in the Open Street Map.
// 
// Ishan Mukherjee
// 

#include "busstops.h"

using namespace std;

//
// constructor
//
BusStops::BusStops(string filename)
{
  ifstream file(filename);
  if (!file)
  {
    cerr << "Error: unable to open bus stops csv " << filename << endl;
  }

  string line;
  while (getline(file, line))
  {
    stringstream parser(line);
    string id, route, name, direction, stop, loc_lat, loc_long;

    // getting params for new bus stop
    getline(parser, id, ',');
    getline(parser, route, ',');
    getline(parser, name, ',');
    getline(parser, direction, ',');
    getline(parser, stop, ',');
    getline(parser, loc_lat, ',');
    getline(parser, loc_long);

    // creating new bus stop
    this->MapBusStops.emplace_back(stoll(id, nullptr), stol(route, nullptr), name, direction, stop, make_pair(stod(loc_lat, nullptr), stod(loc_long, nullptr)));
  }

  file.close();

  sort(this->MapBusStops.begin(), this->MapBusStops.end(), [](BusStop a, BusStop b) {return a.ID < b.ID;});
}

//
// print
//
void BusStops::print()
{
  for (BusStop& BS : this->MapBusStops)
    BS.print();
}

//
// accessors / getters
//
int BusStops::getNumMapBusStops()
{
  return (int) this->MapBusStops.size();
}