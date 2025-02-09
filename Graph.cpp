#include "Graph.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <execution>
#include <numeric>
#include <vector>

// auto Graph::relax(float time) -> void
// {
//     size_t len = universe.size();
//     std::vector<vec2f> vec(len, {0,0});
//     for(size_t i=0; i<len; i++){
//         std::transform(std::execution::unseq, universe.begin(), universe.end(), std::back_inserter(vec),
//                        [push=node_push,
//                         &node](const Node &other){
//                            float dist = node.distance(other);
//                            float force = push/(dist*dist);
//                            vec2f direction = (other-node).normalize();
//                            return direction*force;
//                        });
//     }
// }

static auto calc_push(Node &node, std::vector<Node*> &against, float push) -> vec2f
{
    std::vector<vec2f> vec(against.size(), {0,0});

    // calc push from neighbours
    std::transform(std::execution::par_unseq, against.begin(), against.end(),vec.begin(),
                    [push=push,
                    &node](const Node *other){
                        if(&node==other) return vec2f{.x=0,.y=0};
                        float dist = node.distance(*other);
                        dist = std::max<float>(dist, 5);
                        float force = push/(dist*dist);
                        vec2f direction = (node-*other).normalize();
                        return direction*force;
                    });
    return std::reduce(std::execution::par_unseq, vec.begin(), vec.end());
}

auto Graph::relax(float time) -> void
{
    size_t len = universe.size();
    std::vector<vec2f> vec(len, {0,0});
    for(auto &node : universe){

        int x = node.x/chunk_size;
        int y =node.y/chunk_size;

        // center force
        vec2f force = node.normalize()*-3;

        // push force
        for(int y_offset=-1; y_offset<=1; y_offset++){
            for(int x_offset=-1; x_offset<=1; x_offset++){
                std::vector<Node*>& neighbours = chunks[x+x_offset][y+y_offset];
                force+=calc_push(node, neighbours, node_push*time);
            }
        }

        for(Node *other : node){
            if(&node==other) continue;
            vec2f edge = *other-node;
            float push = -(50-edge.length())/edge_pull;
            force+= edge.normalize()*push;
        }

        float abs = force.length();
        if(isnanf(abs)) {force={1,1};}

        node_move(node, force);
    }
}
