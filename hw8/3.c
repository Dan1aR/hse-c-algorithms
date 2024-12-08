#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// let's implement some kind of graph class
// but without class...
typedef struct {
    size_t* neighbours;
    size_t* neighbours_dist;
    size_t _neighbours_len;
    size_t _neighbours_dist_len;
    size_t num_neighbours;
} graph_node_t;

typedef struct {
    graph_node_t* nodes;
    size_t num_nodes;
} graph_t;

/*
    utility func to dynamic realloc and array (like in vector)
    **array - pointer, to where my array is held
    *current_len - pointer, to where cur array len is held
    new_len - new len for realloc
    element_size - size for realloc
*/
int _dyn_realloc(void** array, size_t* current_len, const size_t new_len, const size_t element_size)
{
    if (new_len > *current_len) {
        *array = realloc(*array, new_len * element_size);
        if (*array == NULL) {
            return -1;
        }
        *current_len = new_len;
    }
    return 0;
}

/*
    Adding edge to the graph
    self - graph itself
    src, dst - edge
*/
int add_edge(graph_t* const self, const size_t src, const size_t dst, const size_t weight)
{
    if (_dyn_realloc(
            (void**)&self->nodes[src].neighbours,
            &self->nodes[src]._neighbours_len,
            self->nodes[src].num_neighbours + 1,
            sizeof(size_t))
        != 0) {
        return 100;
    }
    if (_dyn_realloc(
            (void**)&self->nodes[src].neighbours_dist,
            &self->nodes[src]._neighbours_dist_len,
            self->nodes[src].num_neighbours + 1,
            sizeof(size_t))
        != 0) {
        return 100;
    }
    self->nodes[src].neighbours[self->nodes[src].num_neighbours] = dst;
    self->nodes[src].neighbours_dist[self->nodes[src].num_neighbours] = weight;
    self->nodes[src].num_neighbours++;
    return 0;
}

/*
    calloc graph - fill nodes array with empty structures
    as we know how many nodes we have from start
*/
int init_graph(graph_t* const self, const size_t num_nodes)
{
    self->num_nodes = num_nodes;
    self->nodes = (graph_node_t*)malloc(num_nodes * sizeof(graph_node_t));
    if (self->nodes == NULL) {
        return -1; // Allocation failed
    }
    for (size_t i = 0; i < self->num_nodes; ++i) {
        graph_node_t _node = {
            .neighbours = NULL,
            .neighbours_dist = NULL,
            ._neighbours_len = 0,
            ._neighbours_dist_len = 0,
            .num_neighbours = 0,
        };
        self->nodes[i] = _node;
    }
    return 0;
}

void dijkstra(const graph_t* graph, size_t start_node, size_t* dist)
{
    size_t n = graph->num_nodes;
    int* visited = (int*)malloc(n * sizeof(int));

    for (size_t i = 0; i < n; ++i) {
        dist[i] = ULONG_MAX;
        visited[i] = 0;
    }

    dist[start_node] = 0;

    for (size_t i = 0; i < n; ++i) {
        size_t min_dist_node = ULONG_MAX;
        size_t min_dist = ULONG_MAX;

        for (size_t j = 0; j < n; ++j) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                min_dist_node = j;
            }
        }

        if (min_dist_node == ULONG_MAX) {
            break;
        }

        visited[min_dist_node] = 1;

        // check node
        for (size_t j = 0; j < graph->nodes[min_dist_node].num_neighbours; ++j) {
            size_t neighbor = graph->nodes[min_dist_node].neighbours[j];
            size_t weight = graph->nodes[min_dist_node].neighbours_dist[j];
            if (dist[min_dist_node] != ULONG_MAX && dist[min_dist_node] + weight < dist[neighbor]) {
                dist[neighbor] = dist[min_dist_node] + weight;
            }
        }
    }

    free(visited);
}

int main()
{
    size_t n, m, k;
    if (scanf("%ld", &n) != 1) {
        return 10;
    }
    if (scanf("%ld", &k) != 1) {
        return 11;
    }
    if (scanf("%ld", &m) != 1) {
        return 12;
    }

    graph_t G;
    if (init_graph(&G, n) != 0) {
        return 108;
    }

    // adding edges like in 1st task
    for (size_t i = 0; i < m; ++i) {
        size_t u, v, weight;
        if (scanf("%ld %ld %ld", &u, &v, &weight) != 3) {
            return 14;
        }
        u--;
        v--;
        int _ret_code = add_edge(&G, u, v, weight);
        if (_ret_code != 0) {
            return _ret_code;
        }
    }

    size_t* dist = (size_t*)malloc(n * sizeof(size_t));
    dijkstra(&G, k - 1, dist);

    size_t max_dist = 0;
    int all_reachable = 1;
    for (size_t i = 0; i < n; ++i) {
        if (dist[i] == ULONG_MAX) {
            all_reachable = 0;
            break;
        }
        if (dist[i] > max_dist) {
            max_dist = dist[i];
        }
    }

    if (all_reachable) {
        printf("%ld", max_dist);
    } else {
        printf("-1");
    }

    free(dist);
    return 0;
}
