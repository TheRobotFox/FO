#ifndef GRAPH_H_
#define GRAPH_H_

#include "FO.hpp"
#include "vec2f.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Edge = std::pair<size_t, size_t>;

class Node : public vec2f
{
    std::unordered_set<Node*> edges;
public:
    auto begin()const{return edges.begin();}
    auto end()const{return edges.end();}
    void disconnect(Node *other) {other->edges.erase(this);}
    void disconnectAll() {for(Node *n : edges) disconnect(n);}
    void connect(Node *other){
        edges.insert(other);
        other->edges.insert(this);
    }
    Node(vec2f v):vec2f(v){}
};

class Graph
{
    std::unordered_map<int, std::unordered_map<int, std::vector<Node*>>> chunks;
    float node_push { };
    float edge_pull { };

    float min_force { 0.1F };
    float chunk_size { };

    auto get_chunk(Node &n) -> std::vector<Node*>*
    {
        int x = n.x/chunk_size;
        int y =n.y/chunk_size;
        return &chunks[x][y];
    }

    // move node and adjust chunks
    auto node_move(Node &n, vec2f force) -> void
    {
        std::vector<Node*>* prev = get_chunk(n);
        n.x+=force.x;
        n.y+=force.y;
        std::vector<Node*> *next = get_chunk(n);

        if(prev!=next){
            prev->erase(std::ranges::find(*prev, &n));
            next->push_back(&n);
        }
    }

public:

    std::unordered_map<std::string, std::pair<std::function<bool(std::vector<const Node*>)>, int>> relations;
    std::unordered_map<std::string, std::pair<std::function<Node*(std::vector<const Node*>)>, int>> functions;
    std::unordered_map<std::string, Node*> constants;
        using T = Node;
    std::list<Node> universe;

    Graph(float node_push = 700000.F, float edge_pull = 100.F)
        : node_push(node_push), edge_pull(edge_pull),
          chunk_size(std::sqrt(node_push/min_force)/3) // solve force F/dist² for dist and spread neigbours
    {
        relations["E"] = {[this](std::vector<const Node*> elems){return this->E(*elems[0],*elems[1]);}, 2};
    }

    auto begin(){return universe.begin();}
    auto end(){return universe.end();}

    auto removeNode(decltype(universe.begin()) node) -> void
    {
        node->disconnectAll();
        auto &c = *get_chunk(*node);
        c.erase(std::ranges::find(c, &*node));
        universe.erase(node);
    }

    auto addNode(vec2f pos) -> Node*
    {
        Node &n = universe.emplace_back(pos);
        get_chunk(n)->push_back(&n);
        return &n;
    }

    auto relax(float time) -> void;

    auto E(const Node &a, const Node &b) -> bool
    {
        return std::ranges::find(a, &b)!=a.end();
    }
};


#endif // GRAPH_H_
