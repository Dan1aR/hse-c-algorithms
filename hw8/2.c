#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t* neighbours;
    int* neighbours_dist;
    size_t _neighbours_len;
    size_t _neighbours_dist_len;
    size_t num_neighbours;
} graph_node_t;

typedef struct {
    graph_node_t* nodes;
    size_t num_nodes;
} graph_t;

int dyn_realloc(void** array, size_t* current_len, size_t new_len, size_t element_size)
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

int add_edge(graph_t* g, size_t src, size_t dst, int weight)
{
    if (dyn_realloc((void**)&g->nodes[src].neighbours, &g->nodes[src]._neighbours_len, g->nodes[src].num_neighbours + 1, sizeof(size_t)) != 0) {
        return 100;
    }
    if (dyn_realloc((void**)&g->nodes[src].neighbours_dist, &g->nodes[src]._neighbours_dist_len, g->nodes[src].num_neighbours + 1, sizeof(int)) != 0) {
        return 100;
    }
    g->nodes[src].neighbours[g->nodes[src].num_neighbours] = dst;
    g->nodes[src].neighbours_dist[g->nodes[src].num_neighbours] = weight;
    g->nodes[src].num_neighbours++;
    return 0;
}

int init_graph(graph_t* g, size_t num_nodes)
{
    g->num_nodes = num_nodes;
    g->nodes = (graph_node_t*)malloc(num_nodes * sizeof(graph_node_t));
    if (g->nodes == NULL) {
        return -1;
    }

    for (size_t i = 0; i < num_nodes; ++i) {
        g->nodes[i] = (graph_node_t) {
            .neighbours = NULL,
            .neighbours_dist = NULL,
            ._neighbours_len = 0,
            ._neighbours_dist_len = 0,
            .num_neighbours = 0
        };
    }

    return 0;
}

typedef struct {
    int x;
    int y;
} point_t;

int comparator(const void* p1, const void* p2)
{
    const int(*x)[3] = p1;
    const int(*y)[3] = p2;
    return (*x)[2] - (*y)[2];
}

int find_parent(int parent[], int comp)
{
    if (parent[comp] == comp) {
        return comp;
    }
    return parent[comp] = find_parent(parent, parent[comp]);
}

void union_set(int u, int v, int parent[], int rank[])
{
    u = find_parent(parent, u);
    v = find_parent(parent, v);

    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

int kruskal(int edge_cnt, int edge[edge_cnt][3])
{
    qsort(edge, edge_cnt, sizeof(edge[0]), comparator);

    int parent[edge_cnt], rank[edge_cnt];
    for (int i = 0; i < edge_cnt; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    int min_cost = 0;
    for (int i = 0; i < edge_cnt; i++) {
        int v1 = find_parent(parent, edge[i][0]);
        int v2 = find_parent(parent, edge[i][1]);
        int wt = edge[i][2];

        if (v1 != v2) {
            union_set(v1, v2, parent, rank);
            min_cost += wt;
        }
    }

    return min_cost;
}

int convert_graph_to_edges(const graph_t* G, int edge[][3], int* edge_count)
{
    int count = 0;
    for (size_t i = 0; i < G->num_nodes; ++i) {
        for (size_t j = 0; j < G->nodes[i].num_neighbours; ++j) {
            size_t neighbor = G->nodes[i].neighbours[j];
            if (i < neighbor) {
                edge[count][0] = i;
                edge[count][1] = neighbor;
                edge[count][2] = G->nodes[i].neighbours_dist[j];
                count++;
            }
        }
    }
    *edge_count = count;
    return 0;
}

int main()
{
    size_t n;
    if (scanf("%ld", &n) != 1) {
        return 10;
    }

    graph_t g;
    if (init_graph(&g, n) != 0) {
        return 108;
    }

    point_t* points = (point_t*)malloc(n * sizeof(point_t));
    if (points == NULL) {
        return 110;
    }

    for (size_t i = 0; i < n; ++i) {
        if (!(scanf("%d %d", &points[i].x, &points[i].y) > 0)) {
            return 14;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j)
                continue;
            int weight = abs(points[i].x - points[j].x) + abs(points[i].y - points[j].y);
            int ret_code = add_edge(&g, i, j, weight);
            if (ret_code != 0)
                return ret_code;
        }
    }

    int edge[1000][3];
    int edge_cnt = 0;
    int ret_code = convert_graph_to_edges(&g, edge, &edge_cnt);
    if (ret_code != 0)
        return ret_code;

    int min_cost = kruskal(edge_cnt, (int(*)[3])edge);
    printf("%d\n", min_cost);

    return 0;
}
