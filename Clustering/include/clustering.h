/*  Performs clsutering of data based on the input
 *  set of conditions. Currently based on density based
 *  clustering methods.
 *  Developed by Anubhav Rohatgi
 *  Date: 25/04/2016
 *  Todo: in future I will include other clustering methods like k-means, knn and optics.
 */
 
#pragma once

#ifndef CLUSTERING
#define CLUSTERING

#include <iostream>
#include <vector>
#include <map>
#include <list>



namespace clustering {

/**
 * Node struture for graph
 */
template <typename T>
struct node {
    T val;
    bool visited;
    bool clustered;
    node() : visited(false), clustered(false) {}
};

/**
 * Template Aliasing
 */
template <typename T>
using cluster = std::vector<T>;

/**
 * Adjoint list - template aliasing
 */
template <typename T>
using adj_list = std::list<node<T>*>;


template <typename T>
using graph = std::map<node<T>*, adj_list<T>>;

/**
 * @brief Cluster       Performs clustering of data based on the min number of
 *                      data points in a neighborhood. Based on minimum distance
 *                      graph.
 * @param dataset       Data points of user defined of standard types
 * @param negatives     The outliers are referenced here.
 * @param dataset_size  Size of the dataset
 * @param eps           The minimum distance between the neghborhood data points
 * @param min_pts       The minimum number of data points in the neighborhood
 * @return              The function returns the vector of clusters.
 */
template <typename T, typename DIST_T>
std::vector<cluster<T>> Cluster(T* const&  dataset,
                               cluster<T>& negatives,
                               size_t const dataset_size,
                               DIST_T const eps,
                               size_t const min_pts,
                               DIST_T(*distance_function)(T const& lhs, T const& rhs))
{

    std::vector<node<T>> node_list(dataset_size);
    for (size_t i = 0; i < dataset_size; ++i) {
        node_list[i].val = dataset[i];
    }

    graph<T> g;
    for (size_t i = 0; i < node_list.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (distance_function(node_list[i].val, node_list[j].val) < eps) {
                g[&node_list[i]].push_back(&node_list[j]);
                g[&node_list[j]].push_back(&node_list[i]);
            }
        }
    }

    std::vector<cluster<T>> clusters;
    cluster<T> c;
    for (node<T>& n : node_list) {
        if (n.visited) continue;
        n.visited = true;
        adj_list<T> neighbour_pts = g[&n];

        if (neighbour_pts.size() >= min_pts) {
            expandCluster(n, neighbour_pts, c, g, min_pts);
            clusters.push_back(c);
            c = cluster<T>();
        }
    }

    for (node<T>& n : node_list) {
        if(!n.clustered)
            negatives.push_back(n.val);
    }
    
    return clusters;
}

/**
 * @brief expandCluster     Based on the specific set of parameters the nodes around the seed
 *                          are traversed and condition are tested to group the bunch as a cluster
 * @param point             Seed Point
 * @param neighbourhood     Group of neighborhood points
 * @param c                 References back the estimated cluster
 * @param g                 Map of data points and their indexes
 * @param min_pts           minimum number of points/nodes requried
 */
template <typename T>
void expandCluster(node<T>& point, adj_list<T>& neighbourhood, cluster<T>& c, graph<T>& g, unsigned const& min_pts) {
    c.push_back(point.val);
    point.clustered = true;
    for (node<T>*& n : neighbourhood) {
        if (!n->visited) {
            n->visited = true;
            adj_list<T> next_neighbourhood = g[n];
            if (next_neighbourhood.size() >= min_pts) neighbourhood.splice(neighbourhood.end(), next_neighbourhood);
        }
        if (!n->clustered) {
            c.push_back(n->val);
            n->clustered = true;
        }
    }
}

}// end of namespace



#endif // CLUSTERING


