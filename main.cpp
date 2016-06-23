#include <cstdint>
#include <iostream>

#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

using namespace std;



class MyNode {
    int id;
    double lat;
    double longg;


public:
    int getId() const {
        return id;
    }

    void setId(int id) {
        MyNode::id = id;
    }

    double getLat() const {
        return lat;
    }

    void setLat(double lat) {
        MyNode::lat = lat;
    }

    MyNode() { }

public:
    MyNode(int id, double lat) : id(id), lat(lat) { }
};

vector<MyNode *> listOfNodes;

struct CountHandler : public osmium::handler::Handler {

    uint64_t nodes = 0;
    uint64_t ways = 0;
    uint64_t relations = 0;

    void node(osmium::Node &n) {
        MyNode *mynode = new MyNode();
        //mynode->setId(n.get_value_by_key("id"));
        ++nodes;
        listOfNodes.push_back(mynode);
    }

    void way(osmium::Way &) {
        ++ways;
    }

    void relation(osmium::Relation &) {
        ++relations;
    }

};


int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
        exit(1);
    }

    osmium::io::File infile(argv[1]);
    osmium::io::Reader reader(infile);

    CountHandler handler;
    osmium::apply(reader, handler);
    reader.close();

    std::cout << "Nodes: " << handler.nodes << "\n";
    std::cout << "Ways: " << handler.ways << "\n";
    std::cout << "Relations: " << handler.relations << "\n";

    cout << "size of list:" << listOfNodes.size();

}