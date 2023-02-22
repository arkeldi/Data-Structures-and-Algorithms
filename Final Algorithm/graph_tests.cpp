#include <cassert>
#include <iostream>
#include <sstream>

#include "graph.h"

using std::cout, std::endl;

int main() {
    std::cout << "make an empty digraph" << std::endl;
    Graph G;

    std::cout << "add vertices" << std::endl;
    for (size_t n = 1; n <= 7; n++) {
        G.add_vertex(n);
    }

    std::cout << "add directed edges" << std::endl;
    G.add_edge(1, 2, 5);  // 1 ->{5} 2; (edge from 1 to 2 with weight 5)
    G.add_edge(1, 3, 3);
    G.add_edge(2, 3, 2);
    G.add_edge(2, 5, 3);
    G.add_edge(2, 7, 1);
    G.add_edge(3, 4, 7);
    G.add_edge(3, 5, 7);
    G.add_edge(4, 1, 2);
    G.add_edge(4, 6, 6);
    G.add_edge(5, 4, 2);
    G.add_edge(5, 6, 1);
    G.add_edge(7, 5, 1);

    std::cout << "G has " << G.vertex_count() << " vertices" << std::endl;
    std::cout << "G has " << G.edge_count() << " edges" << std::endl;
    std::cout << std::endl;

    std::cout << "compute mst path from 2" <<std::endl;
    G.prim(2);

    std::cout << "print minimum spanning paths" <<std::endl;
    for (size_t n = 1; n <= 7; n++) {
        std::cout << "minimum spanning path from 2 to " << n << std::endl;
        std::cout << "  ";
        G.print_path(n);
    }
    std::cout << std::endl;

    std::cout << "compute shortest path from 2" <<std::endl;
    G.dijkstra(2);

    std::cout << "print shortest paths" <<std::endl;
    for (size_t n = 1; n <= 7; n++) {
        std::cout << "shortest path from 2 to " << n << std::endl;
        std::cout << "  ";
        G.print_shortest_path(n);
    }

    // Custom tests
    // Assert G contains vertices
    assert(G.vertex_count() == 7);
    assert(G.contains_vertex(1));
    assert(G.contains_vertex(2));
    assert(G.contains_vertex(3));
    assert(G.contains_vertex(4));
    assert(G.contains_vertex(5));
    assert(G.contains_vertex(6));
    assert(G.contains_vertex(7));

    // Assert G contains edges
    assert(G.edge_count() == 12);
    assert(G.contains_edge(1, 2));
    assert(G.contains_edge(1, 3));
    assert(G.contains_edge(2, 3));
    assert(G.contains_edge(2, 5));
    assert(G.contains_edge(2, 7));
    assert(G.contains_edge(3, 4));
    assert(G.contains_edge(3, 5));
    assert(G.contains_edge(4, 1));
    assert(G.contains_edge(4, 6));
    assert(G.contains_edge(5, 4));
    assert(G.contains_edge(5, 6));
    assert(G.contains_edge(7, 5));

    // Assert weights are correct
    assert(G.cost(1, 2) == 5);
    assert(G.cost(1, 3) == 3);
    assert(G.cost(2, 3) == 2);
    assert(G.cost(2, 5) == 3);
    assert(G.cost(2, 7) == 1);
    assert(G.cost(3, 4) == 7);
    assert(G.cost(3, 5) == 7);
    assert(G.cost(4, 1) == 2);
    assert(G.cost(4, 6) == 6);
    assert(G.cost(5, 4) == 2);
    assert(G.cost(5, 6) == 1);
    assert(G.cost(7, 5) == 1);

    // Assert failed add edge due to missing vertex
    assert(!G.add_edge(1, 8, 1));
    assert(!G.add_edge(8, 1, 1));

    // Prim and Dijkstra from invalid nodes
    assert(!G.is_path(100));
    assert(G.distance(100) == INFINITY);

    // Remove edge 1,2
    assert(G.remove_edge(1, 2));
    assert(!G.contains_edge(1, 2));
    assert(G.edge_count() == 11);
    assert(G.contains_vertex(1));
    assert(G.contains_vertex(2));
    assert(G.contains_edge(1, 3));
    assert(!G.remove_edge(1, 2));

    // Remove vertex 1
    assert(G.remove_vertex(1));
    assert(!G.contains_vertex(1));
    assert(G.vertex_count() == 6);
    assert(!G.contains_edge(1, 2));
    assert(!G.contains_edge(1, 3));
    assert(!G.remove_vertex(1));

    // Deep copy test
    Graph g_base;
    assert(g_base.add_vertex(1));
    assert(g_base.add_vertex(2));
    assert(g_base.add_vertex(3));
    assert(g_base.add_edge(1, 2, 5));
    assert(g_base.add_edge(1, 3, 3));
    assert(g_base.add_edge(2, 3, 2));

    Graph g_copy = g_base;
    assert(g_copy.vertex_count() == 3);
    assert(g_copy.edge_count() == 3);
    assert(g_copy.contains_edge(1, 2));
    assert(g_copy.contains_edge(1, 3));
    assert(g_copy.contains_edge(2, 3));

    // Assert that the copy is a deep copy
    assert(g_copy.add_vertex(4));
    assert(g_copy.add_edge(1, 4, 4));
    assert(g_copy.vertex_count() == 4);
    assert(g_copy.edge_count() == 4);
    assert(g_base.vertex_count() == 3);
    assert(g_base.edge_count() == 3);
    assert(!g_base.contains_edge(1, 4));

    Graph g_other(g_base);
    assert(g_other.vertex_count() == 3);
    assert(g_other.edge_count() == 3);
    assert(g_other.contains_edge(1, 2));
    assert(g_other.contains_edge(1, 3));
    assert(g_other.contains_edge(2, 3));

    // Assert other as a deep copy
    assert(g_other.add_vertex(4));
    assert(g_other.add_edge(1, 4, 4));
    assert(g_other.vertex_count() == 4);
    assert(g_other.edge_count() == 4);
    assert(g_base.vertex_count() == 3);
    assert(g_base.edge_count() == 3);
    assert(!g_base.contains_edge(1, 4));


    Graph new_g;
    new_g.add_vertex(1);
    new_g.add_vertex(2);
    new_g.add_vertex(3);
    new_g.add_edge(1, 2, 5);
    new_g.add_edge(3,2,1);
    new_g.remove_vertex(2);
    assert(new_g.edge_count() == 0);

    new_g.prim(0);
    new_g.dijkstra(0);



    return 0;
}
