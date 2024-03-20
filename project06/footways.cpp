/*footways.cpp*/

//
// A collection of footways in the Open Street Map.
// 
// Ishan Mukherjee
// 

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "footways.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

//
// readMapFootways
//
// Given an XML document, reads through the document and 
// stores all the footways into the given vector.
//
void Footways::readMapFootways(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document way by way, looking for university buildings:
  //
  XMLElement* way = osm->FirstChildElement("way");

  while (way != nullptr)
  {
    const XMLAttribute* attr = way->FindAttribute("id");
    assert(attr != nullptr);

    //
    // if this is a footway, store info into vector:
    //
    if (osmContainsKeyValue(way, "highway", "footway") || osmContainsKeyValue(way, "area:highway", "footway"))
    {
      //
      // create footway object, then add the associated
      // node ids to the object:
      //
      long long id = attr->Int64Value();

      Footway F(id);

      XMLElement* nd = way->FirstChildElement("nd");

      while (nd != nullptr)
      {
        const XMLAttribute* ndref = nd->FindAttribute("ref");
        assert(ndref != nullptr);

        long long id = ndref->Int64Value();

        F.add(id);

        // advance to next node ref:
        nd = nd->NextSiblingElement("nd");
      }

      //
      // add the footway to the vector:
      //
      this->MapFootways.push_back(F);
    }//if

    way = way->NextSiblingElement("way");
  }//while

  //
  // done:
  //
}

//
// accessors / getters
//
int Footways::getNumMapFootways() {
  return (int) this->MapFootways.size();
}
