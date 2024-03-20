/*buildings.cpp*/
//
// Functions for buildings.h
//
// Ishan Mukherjee
// Northwestern University
// CS 211
//
// Original author: Prof. Joe Hummel
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#include "buildings.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

//
// getNumMapBuildings
//
// Returns the number of buildings loaded.
//
int Buildings::getNumMapBuildings() const
{
    return this->MapBuildings.size();
}

//
// readMapBuildings
//
// Given an XML document, reads through the document and 
// stores all the buildings into the given vector.
//
void Buildings::readMapBuildings(XMLDocument& xmldoc)
{
    XMLElement* osm = xmldoc.FirstChildElement("osm");
    assert(osm != nullptr);

    XMLElement* way = osm->FirstChildElement("way");
    assert(way != nullptr);

    // iterating through all valid ways
    while (way != nullptr)
    {
        if (osmContainsKeyValue(way, "building", "university"))
        {
            string name = osmGetKeyValue(way, "name");
            string streetAddr = osmGetKeyValue(way, "addr:housenumber") + " " + osmGetKeyValue(way, "addr:street");

            const XMLAttribute* attrId = way->FindAttribute("id");
            assert(attrId != nullptr);
            long long id = attrId->Int64Value();
            
            Building B(id, name, streetAddr);
            XMLElement* nd = way->FirstChildElement("nd");

            // iterating through all valid nodes in the way
            while (nd != nullptr)
            {
                const XMLAttribute* ndref = nd->FindAttribute("ref");
                assert(ndref != nullptr);
                long long id = ndref->Int64Value();
                B.add(id);
                // advance to next node ref:
                nd = nd->NextSiblingElement("nd");
            }

            this->MapBuildings.push_back(B);
            
        }

        way = way->NextSiblingElement("way");
    }

}