#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100

typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node* adjLists[MAX_VERTICES];
} Graph;

typedef struct PQNode {
    int vertex;
    int dist;
} PQNode;

typedef struct PriorityQueue {
    PQNode* items[MAX_VERTICES];
    int size;
} PriorityQueue;

// -------- Utility functions --------
Node* createNode(int v, int w) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Undirected
    newNode = createNode(src, weight);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// -------- Priority Queue (Min-Heap) --------
PriorityQueue* createPQ() {
    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

void swap(PQNode** a, PQNode** b) {
    PQNode* temp = *a;
    *a = *b;
    *b = temp;
}

void pushPQ(PriorityQueue* pq, int vertex, int dist) {
    PQNode* node = malloc(sizeof(PQNode));
    node->vertex = vertex;
    node->dist = dist;
    int i = pq->size++;
    pq->items[i] = node;

    // heapify up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->items[parent]->dist <= pq->items[i]->dist)
            break;
        swap(&pq->items[parent], &pq->items[i]);
        i = parent;
    }
}

PQNode* popPQ(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    PQNode* root = pq->items[0];
    pq->items[0] = pq->items[--pq->size];

    int i = 0;
    while (1) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < pq->size && pq->items[left]->dist < pq->items[smallest]->dist)
            smallest = left;
        if (right < pq->size && pq->items[right]->dist < pq->items[smallest]->dist)
            smallest = right;
        if (smallest == i) break;
        swap(&pq->items[i], &pq->items[smallest]);
        i = smallest;
    }
    return root;
}

int isEmptyPQ(PriorityQueue* pq) {
    return pq->size == 0;
}

// -------- Dijkstra’s Algorithm --------
void Dijkstra(Graph* graph, int start) {
    int dist[MAX_VERTICES];
    for (int i = 0; i < graph->numVertices; i++)
        dist[i] = INT_MAX;
    dist[start] = 0;

    PriorityQueue* pq = createPQ();
    pushPQ(pq, start, 0);

    while (!isEmptyPQ(pq)) {
        PQNode* n = popPQ(pq);
        int u = n->vertex;
        Node* temp = graph->adjLists[u];
        while (temp) {
            int v = temp->vertex;
            int w = temp->weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pushPQ(pq, v, dist[v]);
            }
            temp = temp->next;
        }
        free(n);
    }

    printf("Shortest distances from vertex %d:\n", start);
    for (int i = 0; i < graph->numVertices; i++)
        printf("%d -> %d\n", i, dist[i]);
}

// -------- Main --------
int main() {
    Graph* graph = createGraph(6);

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 2);
    addEdge(graph, 1, 2, 5);
    addEdge(graph, 1, 3, 10);
    addEdge(graph, 2, 4, 3);
    addEdge(graph, 4, 3, 4);
    addEdge(graph, 3, 5, 11);

    Dijkstra(graph, 0);
    return 0;
}
