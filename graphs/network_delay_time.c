#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 105
#define MAXM 10005

// ----------- edge structure -----------
typedef struct {
    int to;
    int weight;
    int next;
} Edge;


Edge edges[MAXM];
int head[MAXN];
int edgeCount = 0;

// ----------- min-heap node ------------
typedef struct {
    int dist;
    int node;
} HeapNode;

HeapNode heap[MAXM];
int heapSize = 0;

// ------------ heap helpers ------------
void swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void push(HeapNode x) {
    heap[++heapSize] = x;
    int i = heapSize;
    while (i > 1 && heap[i].dist < heap[i/2].dist) {
        swap(&heap[i], &heap[i/2]);
        i /= 2;
    }
}

HeapNode pop() {
    HeapNode top = heap[1];     // smallest element
    heap[1] = heap[heapSize--]; // move last element to root
    int i = 1;
    while (1) {
        int smallest = i;
        int l = 2*i; r = 2*i + 1;
        if (l <= heapSize && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r <= heapSize && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return top;
}

int empty() {
    return heapSize == 0;
}

// -------------- Graph Builder ------------
void addEdge(int u, int v, int w) {
    edges[++edgeCount].to = v;
    edges[edgeCount].weight = w;
    edges[edgeCount].next = head[u];
    head[u] = edgeCount;
}

// -------------- MAIN Dijkstra ------------
int networkDelayTime(int times[][3], int timesSize, int n, int k) {
    // Initialize adjacency List
    for (int i = 0; i <= n; i++) head[i] = 0;
    edgeCount = 0;

    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0];
        int v = times[i][1];
        int w = times[i][2];
        addEdge(u, v, w);
    }

    int dist[MAXN];
    for (int i = 1; i <= n; i++) dist[i] = INT_MAX;
    dist[k] = 0;

    heapSize = 0;
    push((HeapNode){0, k});

    while (!empty()) {
        HeapNode cur = pop();
        int d = cur.dist;
        int u = cur.node;
        if (d > dist[u]) continue;

        for (int e = head[u]; e != 0; e = edges[e].next) {
            int v = edges[e].to;
            int w = edges[e].weight;
            if (dist[v] > d + w) {
                dist[v] = d + w;
                push((HeapNode){dist[v], v});
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) return -1;
        if (dist[i] > ans) ans = dist[i];
    }
    return ans;
}

// ---------- example ----------
int main() {
    int times[][3] = {
        {2, 1, 1},
        {2, 3, 1},
        {3, 4, 1}
    };
    int n = 4, k = 2;
    int ans = networkDelayTime(times, 3, n, k);
    printf("Network delay time = %d\n", ans);
    return 0;
}