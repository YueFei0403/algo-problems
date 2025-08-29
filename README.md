# LeetCode-Style Practice Problems

This repository contains my personal collection of some "classical" **algorithmic problem solutions** in Python.
They are inspired by **LeetCode** and **interview preps**.

## 📂 Repository Structure
```
problems/
│── arrays/ # Array-based problems
│── strings/ # String manipulation
│── graphs/ # Graph & DAG problems
│── dp/ # Dynamic programming
...
```


## 🚀 Example Problem: Load Factor in a DAG

Problem:
Given a service list that defines a directed acyclic graph (DAG), compute the load factor of each service:

- Entry point has load factor = 1
- Each dependency node’s load factor = sum of incoming load factors (propagated recursively)

Example Input:
```
service_list = [
"logging=",
"user=logging",
"implementations=user|foobar",
"cores=implementations|user|foobar",
"dashboard=cores|implementations|foobar|user"
]
entry_point = "dashboard"
```

Expected Output:
```
{'dashboard': 1,
'cores': 1,
'implementations': 2,
'foobar': 3,
'user': 4,
'logging': 4}
```

See implementation: `problems/graphs/load_factor_dag.py`

## 🛠 How to Run

Clone the repository:
```
git clone https://github.com/<your-username>/algo-problems.git
cd algo-problems
```
Run a solution file directly:
```
python3 problems/graphs/load_factor_dag.py
```

## 📌 Notes

All solutions are implemented in Python 3.8+.
Each file starts with a problem description in docstring format.
This is a live repo as I'm trying to tackle more problems!

## 📜 License

This repository is licensed under the MIT License – feel free to fork and use the code.
