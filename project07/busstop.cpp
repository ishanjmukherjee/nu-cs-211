/*busstop.cpp*/

//
// A bus stop in the Open Street Map.
// 
// Ishan Mukherjee
// 

#include "busstop.h"

using namespace std;

//
// constructor
//
BusStop::BusStop(long long id, double route, string name, string direction, string stoplocation, pair<double, double> location)
  : ID(id), Route(route), Name(name), Direction(direction), StopLocation(stoplocation), Location(location.first, location.second)
{}

//
// print
//
void BusStop::print()
{
  cout << this->ID << ": ";
  cout << "bus " << this->Route << ", ";
  cout << this->Name << ", ";
  cout << this->Direction << ", ";
  cout << this->StopLocation << ", ";
  cout << "location (" << this->Location.first << ", " << this->Location.second << ")" << endl;
}

//
// printWithDist
//
void BusStop::printWithDist(double dist)
{
  cout << "  " << this->ID << ": ";
  cout << this->Name << ", ";
  cout << "bus #" << this->Route << ", ";
  cout << this->StopLocation << ", ";
  cout << dist << " miles" << endl;
}

//
// printBusPredictions
//
void BusStop::printBusPredictions(CURL* curl)
{
  string url = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=8yU8XjvHc5zZjtN4E8LyJsAn9&rt=" + to_string(this->Route) + "&stpid=" + to_string(this->ID) + "&format=json";
  string response = "";
  if (!callWebServer(curl, url, response))
  {
    cout << "  <<bus predictions unavailable, call failed>>" << endl;
  }
  else
  {
    // read in predictions from response
    auto jsondata = json::parse(response);
    auto bus_response = jsondata["bustime-response"];
    auto predictions = bus_response["prd"];
    // for each prediction (a map) in the list:
    if (predictions.empty())
      cout << "  <<no predictions available>>" << endl;
    for (auto& M : predictions) {
      try 
      {
        // print prediction
        cout << "  vehicle #" << stoi(M["vid"].get_ref<std::string&>()) << " ";
        cout << "on route " << stoi(M["rt"].get_ref<std::string&>()) << " ";
        cout << "travelling " << M["rtdir"].get_ref<std::string&>() << " ";
        cout << "to arrive in " << stoi(M["prdctdn"].get_ref<std::string&>()) << " mins" << endl;
      }
      catch (exception& e)
      {
        cout << " error" << endl;
        cout << " malformed CTA response, prediction unavailable"
         << " (error: " << e.what() << ")" << endl;
      }
    }
  }
}