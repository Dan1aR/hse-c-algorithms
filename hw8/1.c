#include <stdio.h>
#include <stdlib.h>

// let's implement some kind of graph class
// but without class...
typedef struct {
    size_t* neighbours;
    size_t _neighbours_len;
    size_t num_neighbours;
    char is_marked; // for search
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
int add_edge(graph_t* const self, const size_t src, const size_t dst)
{

    if (_dyn_realloc(
            (void**)&self->nodes[src].neighbours,
            &self->nodes[src]._neighbours_len,
            self->nodes[src].num_neighbours + 1,
            sizeof(size_t))
        != 0) {
        return 100;
    }
    self->nodes[src].neighbours[self->nodes[src].num_neighbours] = dst;
    self->nodes[src].num_neighbours++;

    if (_dyn_realloc(
            (void**)&self->nodes[dst].neighbours,
            &self->nodes[dst]._neighbours_len,
            self->nodes[dst].num_neighbours + 1,
            sizeof(size_t))
        != 0) {
        return 101;
    }
    self->nodes[dst].neighbours[self->nodes[dst].num_neighbours] = src;
    self->nodes[dst].num_neighbours++;
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
        return -1;
    }
    for (size_t i = 0; i < self->num_nodes; ++i) {
        graph_node_t _node = {
            .neighbours = NULL,
            ._neighbours_len = 0,
            .num_neighbours = 0,
            .is_marked = 0,
        };
        self->nodes[i] = _node;
    }
    return 0;
}

// basic stack with the same interface
typedef struct {
    size_t* array;
    size_t num_elements;
    size_t _capacity;
} my_stack_t;

int init_stack(my_stack_t* stack)
{
    stack->num_elements = 0;
    stack->_capacity = 1;
    stack->array = (size_t*)malloc(stack->_capacity * sizeof(size_t));
    if (stack->array == NULL) {
        return 102;
    }
    return 0;
}

// push value on stack
int push(my_stack_t* stack, size_t value)
{
    if (_dyn_realloc(
            (void**)&stack->array,
            &stack->_capacity,
            stack->num_elements + 1,
            sizeof(size_t))
        != 0) {
        return 103;
    }
    stack->array[stack->num_elements] = value;
    stack->num_elements++;
    return 0;
}

// pop value from stack (and put it in *value)
int pop(my_stack_t* stack, size_t* value)
{
    if (stack->num_elements == 0) {
        return 104;
    }
    *value = stack->array[stack->num_elements - 1];
    stack->num_elements--;
    return 0;
}

// queue implementation on two stacks
// two stacks queue must be more efficient
typedef struct {
    my_stack_t stack1;
    my_stack_t stack2;
} queue_t;

int init_queue(queue_t* queue)
{
    if (init_stack(&queue->stack1) != 0) {
        return 105;
    }
    if (init_stack(&queue->stack2) != 0) {
        return 106;
    }
    return 0;
}

int enqueue(queue_t* queue, size_t value)
{
    int _ret_code = 0;
    while (queue->stack1.num_elements > 0) {
        size_t temp;
        _ret_code = pop(&queue->stack1, &temp);
        if (_ret_code != 0) {
            return _ret_code;
        }
        _ret_code = push(&queue->stack2, temp);
        if (_ret_code != 0) {
            return _ret_code;
        }
    }
    push(&queue->stack1, value);
    while (queue->stack2.num_elements > 0) {
        size_t temp;
        _ret_code = pop(&queue->stack2, &temp);
        if (_ret_code != 0) {
            return _ret_code;
        }
        _ret_code = push(&queue->stack1, temp);
        if (_ret_code != 0) {
            return _ret_code;
        }
    }

    return 0;
}

int dequeue(queue_t* queue, size_t* value)
{
    if (queue->stack1.num_elements == 0) {
        return 107;
    }

    return pop(&queue->stack1, value);
}

/*
    BFS implementation like in lecture
    0 - not found
    1 - found
    -1 - error code
*/
int bfs_search(graph_t* const G, const size_t src, const size_t dst)
{
    queue_t queue;
    if (init_queue(&queue) != 0) {
        return 111;
    }

    // src as starting, mark as visited
    enqueue(&queue, src);
    G->nodes[src].is_marked = 1;

    int _ret_code = 0;
    while (queue.stack1.num_elements > 0) {
        size_t node;
        _ret_code = dequeue(&queue, &node);
        if (_ret_code != 0) {
            return _ret_code;
        }

        if (node == dst) {
            return 1;
        }

        // enqueue neighbours
        for (size_t i = 0; i < G->nodes[node].num_neighbours; ++i) {
            size_t neighbour = G->nodes[node].neighbours[i];
            if (!G->nodes[neighbour].is_marked) {
                _ret_code = enqueue(&queue, neighbour);
                if (_ret_code != 0) {
                    return _ret_code;
                }
                G->nodes[neighbour].is_marked = 1;
            }
        }
    }
    return 0;
}

int main()
{
    size_t n, m, s, d;
    if (scanf("%ld", &n) != 1) {
        return 10;
    }
    if (scanf("%ld", &m) != 1) {
        return 11;
    }
    if (scanf("%ld", &s) != 1) {
        return 12;
    }
    if (scanf("%ld", &d) != 1) {
        return 13;
    }

    graph_t G;
    if (init_graph(&G, n) != 0) {
        return 108;
    }

    for (size_t i = 0; i < m; ++i) {
        size_t u, v;
        if (scanf("%ld %ld", &u, &v) == 0) {
            return 14;
        }
        int _ret_code = add_edge(&G, u, v);
        if (_ret_code != 0) {
            return _ret_code;
        }
    }

    int result = bfs_search(&G, s, d);
    if (result == 1) {
        printf("true");
    } else if (result == 0) {
        printf("false");
    } else {
        return result;
    }

    return 0;
}
