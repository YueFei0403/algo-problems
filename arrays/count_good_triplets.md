# Count Good Triplets
## Enumeration Optimization: Idea & Algorithm

We consider an $O(n^2)$ enumeration of all pairs $(j,k)$ that satisfy $|arr[j] - arr[k]| \le b$. For each such pair, we count how many indices $i$ meet the required conditions. According to the problem, $i$ must satisfy: 

$|arr[i] - arr[j]| \le a$ and $|arr[i] - arr[k]| \le c$.

By expanding the absolute values, the valid range for $arr[i]$ must lie within the intersection of the two intervals:

$[arr[j] - a, arr[j] + a]$ and $[arr[k] - c, arr[k] + c]$.

We denote this intersection as $[l, r]$. Thus, when enumerating a pair $(j,k)$, the task reduces to quickly counting how many indices $i < j$ have values $arr[i]$ lying in $[l, r]$.


#### Why a Prefix-Sum (Cumulative Frequency) Array?
A naive scan over all $i < j$ per pair $(j,k)$ costs $O(n)$, giving $O(n^3)$ overall. Instead, observe this is a **range counting** query in value space: "How many of the already-seen values (indices $<j$) lie in $[l,r]$?"


We maintain a **frequency array** $freq[v]$ = number of occurrences of value $v$ among indices $< j$. From that we build a prefix-sum array:

$sum[x] = \sum_{v \le x} freq[v]$,

which lets us answer a range count in $O(1)$:
$\{ i < j | arr[i] \in [l,r]\} = sum[r] - sum[l-1]$.

To enforce $i < j$, we iterate $j$ from left to right; when we "arrive" at $j$, the data structure currently contains exactly the elements with indices $<j$. (Implementation detail: you typically **update** the structure when advancing $j$--i.e., after finishing all queries that use this $j$ as the right boundary, you insert $arr[j]$ so it'll be available when $j$ moves on.) 

> This approach is practical because the value range is small in this problem. If the domain is large, you'd use **coordinate compression** plus a **Fenwick tree/segment tree** to support $O(logV)$ updates and range queries.

## Worked Example (Step-by-Step)
Let's use a tiny array with small bounds so we can see the mechanics clearly.

- **arr = [1, 3, 2, 2]**
- Parameters: a = 1, b = 1, c= 1
- Assume values lie in $[0,5]$ (small domain just for illustration).

We'll sweep $j=0 \rightarrow 3$. For each $j$, we consider all $k$ (typically $k > j$ in the LeetCode variant where $i<j<k$); if your variant allows any order, you'll still check pairs satisfying $|arr[j] - arr[k]| \le b$. Below we'll show a couple of representative $(j,k)$ checks and how the prefix sums make counting $i < j$ instant.

## Data Structures
- `freq[0..5]` initialized to 0
- `sum[x] = cumulative sum of freq up to x` (rebuild or incrementally maintain as you update `freq`)

We'll maintain `sum` as we go; for clarity, we'll recompute it from `freq` each time we update.


**1. The Role of `freq` and `sum`**
- `freq[val]` stores how many times the value `val` has appeared among indices strictly less than the current $j$.
- Then, when we form a pair $(j,k)$, we can immediately ask:
>"How many $i<j$ have $arr[i]$ in $[l, r]$?" and answer it from `sum`.

So, the correctness requires:

👉 At the moment we process index $j$, the frequency structure should already contain exactly all the values with indices $< j$.

**2. When Do We Update?**
That's why we do an **update before moving on from j**
- At the start of processing $j$, the structure contains only indices $<j$ (strictly less than).
- We run all queries that use $j$ with $i<j$.
- Then we "insert" $arr[j]$ into the frequency structure by updating `freq[arr[j]] += 1`. 
- Now the structure contains all indices $\le j$, which is correct for the next step when $j$ increments.

This ensures that when we advance to $j + 1$, all indices `< (j + 1)` are represented.


**3. Example (Why Not Earlier?)**

Say `arr = [1, 3, 2]`
- Start with $j = 1$:
    `freq` should only have `arr[0] = 1`.
    If we had already inserted `arr[1]` too early, then the query "how many $i < 1$..." would accidentally include $i=1$, which is illegal (since $i$ must be strictly less than $j$). That would overcount.
So the **discipline** is:
- Process all queries with current $j$.
- Then do `freq[arr[j]] += 1`.

That way, we never let an element "see itself" as a valid $i$.


**4. Summary of Rationale**
We update `freq[arr[j]]` at the end of processing step $j$ because: 
- We need to guarantee `freq` always represents exactly the set of indices $i < j$.
- This prevents overcounting where $i=j$ sneaks in.
- It makes the prefix sums correct for answering range queries for the **next $j$**.


---

**j = 1 (arr[1] = 3)**
Elements with indices $<1$: just `arr[0] = 1`.
- Update `freq[1] += 1` $\rightarrow$ `freq = [0,1,0,0,0,0]` $\Rightarrow$ `sum = [0, 1, 1, 1, 1, 1]`

Now try $k = 2$ and $k = 3$ (assuming $k > j$):
1. Pair $(j,k) = (1,2)$: `arr[1] = 3`, `arr[2] = 2`
    - Check $|3-2| \le b = 1$ $\rightarrow$ **valid**
    - Intervals:
        - $[arr[j] - a, arr[j] + a] = [3-1, 3+1] = [2, 4]$
        - $[arr[k] - c, arr[k] + c] = [2-1, 2+1] = [1, 3]$
        - Intersection $[l,r] = [2,3]$
    - Count indices $i < 1$ with $arr[i] \in [2, 3]$:
            $sum[3] - sum[1] = 1 - 1 = 0$.
        (Indeed, only value seen so far is 1.)

2. Pair $(j,k) = (1,3)$: `arr[3]=2`
    - $|3-2| \le 1 \rightarrow $  **valid**
    - Same intersection $[2,3]$
    - Count = sum[3] - sum[1] = 0.
    (After finishing all pairs that use this $j$, we move on.)

---

**j = 2 (arr[2] = 2)**
Elements with indices $<2$: `arr[0] = 1`, `arr[1] = 3`.
- Update `freq[3] += 1` (we insert `arr[1]` as we advance past it)
    
    `freq = [0, 1, 0, 1, 0, 0]`
    `sum = [0, 1, 1, 2, 2, 2]`

Now consider $k = 3$:
1. Pair $(j,k) = (2, 3)$: `arr[2] = 2`, `arr[3] = 2`

- $|2 - 2| \le 1 \rightarrow$ **valid**
- Intervals:
    - $[2 - 1, 2 + 1] = [1, 3]$
    - $[2 - 1, 2 + 1] = [1, 3]$
    - Intersection $[1, 3]$
- Count indices $i < 2$ with $arr[i] \in [1, 3]$:
    sum[3] - sum[0] = 2 - 0 = 2.
    (Those are values 1 and 3 from indices 0 and 1.)
These 2 choices of $i$ (namely $i=0$ with value $1$ and $i=1$ with value $3$) each from a valid triplet with $(j, k) = (2, 3)$.

---

**j=3 (arr[3] = 2)**
Elements with indices $<3$: `arr[0]=1`, `arr[1]=3`, `arr[2]=2`.
- Update `freq[2] += 1` (insert arr[2])

    `freq = [0, 1, 1, 1, 0, 0]`
    `sum = [0, 1, 2, 3, 3, 3]`

There are no $k>3$, so we're done. (If your implementation queries using $(j,k)$ with both orders, you'd check those accordingly; the counting logic is unchanged.)

**Takeaway from the example:** each time we form $(j,k)$, computing $[l,r]$ is $O(1)$, and **counting** the valid $i<j$ is also $O(1)$ via `sum[r] - sum[l-1]`. This is the entire speedup.


---

### Complexity
- Enumerating all $(j,k)$ pairs: $O(n^2)$.
- Each pair's range count via prefix sums: $O(1)$.
- Maintaining `freq/sum` per step: $O(1)$ amortized if the domain is small (as in this problem).

Overall: $O(n^2)$. 

---

### Implementation Notes
- When advancing $j$, ensure the data structure contains exactly the elements at indices $<j$. A common pattern is:

    1. For current $j$, answer all queries against $k$ using the current structure.
    2. Then insert arr[j] before moving to $j+1$.

- If the value range is large, use **coordinate compression** and a **Fenwick tree (BIT)** or **segment tree** to support:
    - `update(value, +1)` in $O(logV)$,
    - `range_query(l, r)` in $O(logV)$.