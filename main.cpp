#include <cstdint>
#include <iostream>

#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/geom/haversine.hpp>

#include <boost/graph/adjacency_list.hpp>

#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <fstream>

/**
 * libosmium definitions
 */
typedef osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location> index_type;
typedef osmium::handler::NodeLocationsForWays<index_type> location_handler_type;

/**
 * Boost definitions
 */
typedef boost::property<boost::vertex_name_t, uint64_t> VertexIdProperty;  //store OSM ids as properties
typedef boost::property<boost::edge_weight_t, uint64_t> EdgeWeightProperty;  //store edge weights as uint64_t
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, VertexIdProperty, EdgeWeightProperty>
    graphType; //declaring the graph type (Adjacency-list because the graph is sparse).

/**
 * Main graph variable
 */
graphType mainGraph;

std::map<uint64_t, uint64_t> trackMap; // contains the node id as the key and counter as value
class MainHandler: public osmium::handler::Handler {
 public:

  uint64_t nodes = 0;
  uint64_t ways = 0;
  uint64_t relations = 0;
  uint64_t length = 0;

  void node(osmium::Node &n) {
    trackMap[n.id()] = nodes;
    nodes++;  //Counter for the number of nodes starts from 0
  }

  void way(osmium::Way &w) {


    if (w.tags()["highway"]
        && !w.tags()["oneway"]) { // Highways are routable : http://wiki.openstreetmap.org/wiki/Highways
      for (auto it = w.nodes().begin(); it < w.nodes().end(); it++) { // Iterate through all nodes in the way
        //std::cout << "ref=" << it->ref() << " location=" << it->location() << "\n"; //debug
        auto nextIt = it + 1;
        if (trackMap.find(it->ref()) != trackMap.end() && nextIt < w.nodes().end()
            && trackMap.find(nextIt->ref()) != trackMap.end()) { //find and add valid edges into the boost graph
          boost::add_edge(trackMap[it->ref()],
                          trackMap[nextIt->ref()],
                          osmium::geom::haversine::distance(it->location(),
                                                            nextIt->location()), //Distance between two Geo-coordinates
                          mainGraph);
          //Adding in the opposite edge as well
          boost::add_edge(trackMap[nextIt->ref()],
                          trackMap[it->ref()],
                          osmium::geom::haversine::distance(it->location(),
                                                            nextIt->location()), //Distance between two Geo-coordinates
                          mainGraph);
        }
      }
    } else if (w.tags()["highway"] && strcmp(w.tags()["oneway"], "yes")) {// One way
      for (auto it = w.nodes().begin(); it < w.nodes().end(); it++) { // Iterate through all nodes in the way
        auto nextIt = it + 1;
        if (trackMap.find(it->ref()) != trackMap.end() && nextIt < w.nodes().end()
            && trackMap.find(nextIt->ref()) != trackMap.end()) { //find and add valid edges into the boost graph
          boost::add_edge(trackMap[it->ref()],
                          trackMap[nextIt->ref()],
                          osmium::geom::haversine::distance(it->location(),
                                                            nextIt->location()), //Distance between two Geo-coordinates
                          mainGraph);
        }
      }
    }
    ways++;
  }

  void relation(osmium::Relation &) {
    relations++;
  }

};

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
    exit(1);
  }

  osmium::io::File infile(argv[1]);
  osmium::io::Reader reader(infile);

  index_type index;
  location_handler_type location_handler(index);
  MainHandler handler;

  osmium::apply(reader, location_handler, handler);
  reader.close();

  //Generating the MTX here
  std::pair<graphType::edge_iterator, graphType::edge_iterator> edges = boost::edges(mainGraph);
  boost::property_map<graphType, boost::edge_weight_t>::type weights = boost::get(boost::edge_weight_t(), mainGraph);
  graphType::edge_iterator edge;

  std::ofstream outputMTXFile;
  outputMTXFile.open("IN.mtx", std::ios::out); //Pass in a fully qualified path if required
  if (outputMTXFile.is_open()) {
    outputMTXFile << "%%MatrixMarket matrix coordinate real general\n";
    outputMTXFile << num_vertices(mainGraph) << " " << num_vertices(mainGraph) << " " << num_edges(mainGraph) << "\n";
    for (edge = edges.first; edge != edges.second; ++edge) {
      outputMTXFile << (*edge).m_source << " ";
      outputMTXFile << (*edge).m_target << " ";
      outputMTXFile << boost::get(weights, *edge) << "\n";
    }

  }
  outputMTXFile.close();

  //Generating the node relationships file here:
  std::ofstream outputMapFile;
  outputMapFile.open("nodeMap.txt", std::ios::out); //Pass in a fully qualified path if required
  if (outputMapFile.is_open()) {
    for (auto entry: trackMap) {
      outputMapFile << entry.second << " ";
      outputMapFile << entry.first << "\n";
    }
  }

  outputMapFile.close();

  std::cout << "Nodes: " << handler.nodes << "\n";
  std::cout << "Ways: " << handler.ways << "\n";
  std::cout << "Relations: " << handler.relations << "\n";

  std::cout << "size of <nodeid,id> map:" << trackMap.size() << "\n";
  std::cout << "nodes in the boost graph:" << num_vertices(mainGraph) << "\n";
  std::cout << "edges in the boost graph:" << num_edges(mainGraph) << "\n";

  /*********
    IN.osm.pbf
    Nodes: 48581411
    Ways: 6219143
    Relations: 33316
    size of hash map:48581411
    nodes in the boost graph: 48581282
    edges in the boost graph: 32305508 (Its a sparse graph).
  *********/
}