#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node* adjLists[MAX_VERTICES];
    int visited[MAX_VERTICES];
} Graph;

typedef struct Stack {
    int items[MAX_VERTICES];
    int top;
} Stack;

// -------- Utility functions --------
Node* createNode(int v) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Undirected graph: add reverse edge
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// -------- Stack operations --------
Stack* createStack() {
    Stack* s = malloc(sizeof(Stack));
    s->top = -1;
    return s;
}

void push(Stack* s, int value) {
    s->items[++(s->top)] = value;
}

int pop(Stack* s) {
    return s->items[(s->top)--];
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

// -------- Iterative DFS --------
void DFS_iterative(Graph* g, int start) {
    Stack* s = createStack();
    push(s, start);

    while (!isEmpty(s)) {
        int v = pop(s);

        if (!g->visited[v]) {
            printf("%d ", v);
            g->visited[v] = 1;
        }

        Node* temp = g->adjLists[v];
        while (temp) {
            if (!g->visited[temp->vertex])
                push(s, temp->vertex);
            temp = temp->next;
        }
    }
}

// -------- Main --------
int main() {
    Graph* graph = createGraph(6);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);
    addEdge(graph, 4, 5);

    printf("Iterative DFS starting from vertex 0:\n");
    DFS_iterative(graph, 0);
    printf("\n");
    return 0;
}
