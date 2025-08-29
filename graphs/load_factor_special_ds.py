#!/usr/bin/env python3
"""
Compute load factors in a DAG of services.

Rules:
- Entry point has load factor = 1.
- Each dependency node's load factor = sum of incoming load factors.
- Load factors accumulate recursively through the DAG
"""

# defaultdict:  a smarter dictionary that gives you default values automatically
# deque:        a fast, flexible queue that supports operations on both ends.
from collections import defaultdict, deque


# -----------------------------------
# Construct the forward graph
# -----------------------------------
def parse_services(service_list):
    """ Parse service definitions into adjacency lists."""
    graph = defaultdict(list)
    declared_nodes = set()  # only nodes defined on LHS
    
    for s in service_list:
        name, deps = s.split("=")
        name = name.strip()
        declared_nodes.add(name)
        if deps.strip():
            graph[name] = [d.strip() for d in deps.split("|")]
        else:
            graph[name] = []  # ensure empty list
    return graph, declared_nodes 

# --------------------------------------
# Top-Down Propagation
# --------------------------------------
def compute_load_factors_topdown(service_list, entry_point):
    graph, declared_nodes = parse_services(service_list)
    
    # Step 1 : Topological order     
    indegree = defaultdict(int)
    for u in graph:
        for v in graph[u]:
            indegree[v] += 1
    q = deque([entry_point])
    topo_order = []
    while q:
        node = q.popleft()
        topo_order.append(node)
        for dep in graph[node]:
            indegree[dep] -= 1 
            if indegree[dep] == 0:
                q.append(dep)
                
    # Step 2: DP accumulation
    load = defaultdict(int)
    load[entry_point] = 1
    for node in topo_order:
        for dep in graph[node]:
            if dep not in declared_nodes:
                continue
            load[dep] += load[node]
    
    return dict(load)

def main():
    # Example service list
    service_list = [
        "logging=",
        "user=logging",
        "implementations=user|foobar",
        "cores=implementations|user|foobar",
        "dashboard=cores|implementations|foobar|user"
    ]
    entry_point = "dashboard"
    
    print("Top-Down Load Factors:")
    print(compute_load_factors_topdown(service_list, entry_point))



if __name__ == "__main__":
    main()