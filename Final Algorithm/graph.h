#ifndef GRAPH_H
#define GRAPH_H

#include <cmath>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct prim_data {
    size_t parent;
    double cost;
};

struct dijkstra_data {
    size_t parent;
    double dist;
};

// To be used during prim's algorithm for the priority queue
typedef std::pair<double, size_t> priority_data;
template <typename T>
class PairComparison {
public:
    bool operator()(const T &left, const T &right) const {
        return left.first > right.first;
    }
};

class Graph {
private:
    // implement the graph in a mix between adjacency list and adjacency matrix
    std::unordered_map<size_t, std::unordered_map<size_t, double>> edges;

    // Structure to store the result of the last run Prim Algorithm
    std::unordered_map<size_t, prim_data> prim_result;

    // Structure to store the result of the last run dijkstra Algorithm
    std::unordered_map<size_t, dijkstra_data> dijkstra_result;

public:
    // Task 1
    Graph();

    Graph(const Graph &other);

    Graph &operator=(const Graph &other);
    ~Graph();

    size_t vertex_count() const;
    size_t edge_count() const;

    bool contains_vertex(size_t id) const;
    bool contains_edge(size_t src, size_t dest) const;
    double cost(size_t src, size_t dest) const;

    bool add_vertex(size_t id);
    bool add_edge(size_t src, size_t dest, double weight = 1);
    bool remove_vertex(size_t id);
    bool remove_edge(size_t src, size_t dest);

    // Task 2
    void prim(size_t source_id);
    bool is_path(size_t id) const;
    void print_path(size_t dest_id, std::ostream &os = std::cout) const;

    // // Task 3
    void dijkstra(size_t source_id);
    double distance(size_t id) const;
    void print_shortest_path(size_t dest_id, std::ostream &os = std::cout) const;
};

Graph::Graph() : edges(), prim_result(), dijkstra_result() {}

Graph::Graph(const Graph &other) : edges(other.edges), prim_result(other.prim_result), dijkstra_result(other.dijkstra_result) {}

Graph &Graph::operator=(const Graph &other) {
    edges = other.edges;
    prim_result = other.prim_result;
    dijkstra_result = other.dijkstra_result;

    return *this;
}

Graph::~Graph() {}

size_t Graph::vertex_count() const {
    return edges.size();
}

size_t Graph::edge_count() const {
    size_t count = 0;
    for (auto lst : edges) {
        count += lst.second.size();
    }
    return count;
}

bool Graph::contains_vertex(size_t id) const {
    return edges.find(id) != edges.end();
}

bool Graph::contains_edge(size_t src, size_t dest) const {
    // Both the source and the edge must exist
    if (contains_vertex(src) && contains_vertex(dest)) {
        return edges.at(src).find(dest) != edges.at(src).end();
    }
    return false;
}

double Graph::cost(size_t src, size_t dest) const {
    if (contains_edge(src, dest)) {
        return edges.at(src).at(dest);
    }
    return INFINITY;
}

bool Graph::add_vertex(size_t id) {
    if (contains_vertex(id)) {
        return false;
    }
    edges.insert(std::make_pair(id, std::unordered_map<size_t, double>()));
    return true;
}

bool Graph::add_edge(size_t src, size_t dest, double weight) {
    if (contains_edge(src, dest) || !contains_vertex(src) || !contains_vertex(dest)) {
        return false;
    }
    edges.at(src).insert(std::make_pair(dest, weight));
    return true;
}

bool Graph::remove_vertex(size_t id) {
    if (!contains_vertex(id)) {
        return false;
    }
    edges.erase(id);

    // find all edges that have dst = id
    for (auto &lst : edges) {
        if (lst.second.find(id) != lst.second.end()) {
            lst.second.erase(id);
        }
    }
    return true;
}

bool Graph::remove_edge(size_t src, size_t dest) {
    if (!contains_edge(src, dest)) {
        return false;
    }
    edges.at(src).erase(dest);
    return true;
}

void Graph::prim(size_t source_id) {
    if (!contains_vertex(source_id)) {
        return;
    }

    // Clear previous data
    prim_result.clear();
    for (auto &v : edges) {
        // Parent = SIZE_MAX, Cost = INFINITY
        prim_result.insert(std::make_pair(v.first, prim_data{SIZE_MAX, INFINITY}));
    }

    // Initialize first vertex
    prim_result.at(source_id).parent = source_id;
    prim_result.at(source_id).cost = 0;

    // Initialize the priority queue with lowest cost prim_result
    std::priority_queue<priority_data, std::vector<priority_data>, PairComparison<priority_data>> pq;
    pq.push(std::make_pair(0, source_id));

    // Keep track of visited nodes
    std::unordered_set<size_t> visited;

    // While the queue is not empty
    while (!pq.empty()) {
        // Get the lowest cost prim_result
        auto curr = pq.top();
        pq.pop();

        // If the current prim_result is not visited
        if (visited.find(curr.second) == visited.end()) {
            // Mark it as visited
            visited.insert(curr.second);
        } else {
            // If it is already visited, skip it
            continue;
        }

        // For each neighbor of the current prim_result
        for (auto &neighbor : edges.at(curr.second)) {
            size_t dst = neighbor.first;
            double cost = neighbor.second;

            // Check not visited
            if (visited.find(dst) != visited.end()) {
                continue;
            }

            // If the cost is lower than the current cost
            if (cost < prim_result.at(dst).cost) {
                // Update the cost and parent
                prim_result.at(dst).cost = cost;
                prim_result.at(dst).parent = curr.second;

                // Push the new cost to the queue
                // This works because the new cost will always follow the priority queue comparison function and the best cost at the time of visit will be the first one
                // By checking if a node has been visited, we can avoid pushing the same node twice
                // This is a workaround to Extract-Min with an update, which is unavailable
                pq.push(std::make_pair(cost, dst));
            }
        }
    }
}

bool Graph::is_path(size_t id) const {
    // A path exists if a node exists and it has a parent
    return prim_result.find(id) != prim_result.end() && prim_result.at(id).parent != SIZE_MAX;
}

void Graph::print_path(size_t dest_id, std::ostream &os) const {
    if (!is_path(dest_id)) {
        os << "<no path>\n";
        return;
    }

    // Build a stack of nodes by starting at the end of the path
    std::stack<size_t> stack;
    stack.push(dest_id);

    // Find path to the source by traversing the parents
    size_t current_v = dest_id;
    while (prim_result.at(current_v).parent != current_v) {
        current_v = prim_result.at(current_v).parent;
        stack.push(current_v);
    }

    // Print the path in a reverse order, given by the stack
    os << stack.top();
    stack.pop();
    while (!stack.empty()) {
        os << " --> " << stack.top();
        stack.pop();
    }
    os << "\n";
}

void Graph::dijkstra(size_t source_id) {
    if (!contains_vertex(source_id)) {
        return;
    }

    // Clear previous data
    dijkstra_result.clear();
    for (auto &v : edges) {
        // Parent = SIZE_MAX, Dist = INFINITY
        dijkstra_result.insert(std::make_pair(v.first, dijkstra_data{SIZE_MAX, INFINITY}));
    }

    // Initialize first vertex
    dijkstra_result.at(source_id).parent = source_id;
    dijkstra_result.at(source_id).dist = 0;

    // Initialize priority queue
    std::priority_queue<priority_data, std::vector<priority_data>, PairComparison<priority_data>> pq;

    pq.push(std::make_pair(0, source_id));

    // Keep track of visited nodes
    std::unordered_set<size_t> visited;

    // While the queue is not empty
    while (!pq.empty()) {
        auto curr = pq.top();
        pq.pop();

        // Check visited
        if (visited.find(curr.second) != visited.end()) {
            continue;
        } else {
            visited.insert(curr.second);
        }

        // Iterate over neighbours
        for (auto &neighbor : edges.at(curr.second)) {
            size_t dst = neighbor.first;
            double distance = neighbor.second;

            // Check not visited
            if (visited.find(dst) != visited.end()) {
                continue;
            }

            double new_dist = dijkstra_result.at(curr.second).dist + distance;

            // If the new distance is lower than the current distance
            if (new_dist < dijkstra_result.at(dst).dist) {
                // Update the distance and parent
                dijkstra_result.at(dst).dist = new_dist;
                dijkstra_result.at(dst).parent = curr.second;

                // Push the new distance to the queue
                // Check Prim's algorithm for explanation
                pq.push(std::make_pair(new_dist, dst));
            }
        }
    }
}

double Graph::distance(size_t id) const {
    // A non existent node is considered infinite
    if (dijkstra_result.find(id) == dijkstra_result.end())
        return INFINITY;
    else
        return dijkstra_result.at(id).dist;
}

void Graph::print_shortest_path(size_t dest_id, std::ostream &os) const {
    if (distance(dest_id) == INFINITY) {
        os << "<no path>\n";
        return;
    }

    std::stack<size_t> stack;
    stack.push(dest_id);

    // Find path to the source
    size_t current_v = dest_id;
    while (dijkstra_result.at(current_v).parent != current_v) {
        current_v = dijkstra_result.at(current_v).parent;
        stack.push(current_v);
    }

    // Print the path
    os << stack.top();
    stack.pop();
    while (!stack.empty()) {
        os << " --> " << stack.top();
        stack.pop();
    }
    os << " distance: " << distance(dest_id) << "\n";
}

#endif // GRAPH_H
