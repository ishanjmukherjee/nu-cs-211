/*main.cpp*/
//
// main() function for executing the program.
//
// Ishan Mukherjee
// Northwestern University
// CS 211
#include <iostream>
#include <string>

#include "building.h"
#include "buildings.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

int main()
{
    cout << "OSM file > ";
    string osm_filename;
    getline(cin, osm_filename);
    XMLDocument xmldoc;
    if (!osmLoadMapFile(osm_filename, xmldoc))
    {
        cout << "XML document could not be created" << endl;
        return 0;
    }
    
    Nodes nodes;
    nodes.readMapNodes(xmldoc);
    cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
    Buildings buildings;
    buildings.readMapBuildings(xmldoc);
    cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;

    string input;
    cout << "Enter building name(partial or complete), or * to list, or $ to end>" << endl;
    getline(cin, input);

    // user input loop: terminated on pressing "#"
    while (input != "$")
    {
        // print all buildings
        if (input == "*")
        {
            for (Building b : buildings.MapBuildings)
            {
                cout << b.ID << ": " << b.Name << ", " << b.StreetAddress << endl;
            }
        }
        // print information about a specific building
        else
        {
            // iterating through Building objects in the current Buildings object
            for (Building b : buildings.MapBuildings)
            {
                // if inputted spring found in a Building name
                if (b.Name.find(input) != string::npos)
                {
                    cout << b.Name << endl; 
                    cout << "Address: " << b.StreetAddress << endl;
                    cout << "Building ID: " << b.ID << endl;
                    cout << "Nodes:" << endl;
                    // printing all the nodes in Building object
                    for (long long nid : b.NodeIDs)
                    {
                        double lat, lon;
                        bool entrance;
                        if (!nodes.find(nid, lat, lon, entrance))
                        {
                            cout << nid << "** NOT FOUND **" << endl;
                        }
                        else
                        {
                            // print node id, lat, long and whether the node is an entrance
                            cout << "  " << nid << ": (" << lat << ", " << lon << ")";
                            if (entrance)
                            {
                                cout << ", is entrance";
                            }
                            cout << endl;
                        } 
                    }
                }
            }
        }

        cout << "Enter building name (partial or complete), or * to list, or $ to end>" << endl;
        getline(cin, input);
    }

    cout << "** Done **" << endl;
    cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
    cout << "# of Nodes created: " << Node::getCreated() << endl;
    cout << "# of Nodes copied: " << Node::getCopied() << endl;

    buildings.readMapBuildings(xmldoc);
    return 0;
}