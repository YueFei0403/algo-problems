# XOR Operation in an Array -- Mathematical Solution

## Key Properties of XOR
Let $\oplus$ denote the XOR operation. XOR satisfies the following properties:

- Self-canceling: $x \oplus x = 0$
- Commutative: $x \oplus y = y \oplus x$
- Associative: $ (x \oplus y) \oplus z = x \oplus (y \oplus z) $
- Self-inverse: $x \oplus y \oplus y = x$
- For all integers `i`,
    - $4i \oplus (4i + 1) \oplus (4i + 2) \oplus (4i + 3) = 0$
    - **Why?** 
        ```c
        0 ^ 1 ^ 2 ^ 3 = 0
        4 ^ 5 ^ 6 ^ 7 = 0
        8 ^ 9 ^ 10 ^ 11 = 0
        ... and so on
        ```
        Using the **associative law**, we can group the first two numbers and the last two numbers. Then, convert the numbers into their binary form.
        For a sequence that starts with a multiple of 4, the higher bits (beyond the last two) are the same, so only the last two bits matter. The four consecutive numbers can be written as:
        ```c
        xxx00, xxx01, xxx10, xxx11
        ```
        - The XOR of the first two (`xxx00 ^ xxx01`) is `01`.
        - The XOR of the last two (`xxx00 ^ xxx01`) is also `01`. 
        - Finally, `01 ^ 01 = 0`.
        Thus, the result of XORing four consecutive integers starting at a multiple of 4 is always `0`.

## Step 1. Transform the Formula

Original Problem:

$Q = start \oplus (start + 2i) \oplus (start + 4i) \oplus \cdots \oplus (start + 2(n-1))$

Define:
- `s = ⌊ start / 2 ⌋`
- `e = result's lowest bit`

The expression becomes:

$Q = (s \oplus (s + 1) \oplus \cdots \oplus (s + n - 1)) \times 2 + e$

## Step 2. What is the Lowest Bit?
The only case the result's LSB is `1` is when `start` is odd and `n` is odd; otherwise, the result's LSB is `0`.

Now the problem reduces to XOR of a sequence of **consecutive integers**, scaled by 2, with the extracted bit added back.

## Step 3. Defining `sumXor(x)`
Let:
$sumXor(x) = 0 \oplus 1 \oplus 2 \oplus \cdots \oplus x$

From the pattern above, `sumXor(x)` can be computed in **O(1)** time:
```c
sumXor(x) = 
    x           if x % 4 == 0
    1           if x % 4 == 1
    x + 1       if x % 4 == 2
    0           if x % 4 == 3 
```

Step 4. How Everything Finally Collapses to O(1)

- $(s \oplus (s + 1) \oplus \cdots \oplus (s + n - 1)) = $ 
`(0 ^ 1 ^ 2 ^ 3 ^ (s - 1)) ^ (0 ^ 1 ^ 2 ^ 3 ^ ... ^ (s - 1) ^ s ^ (s + 1) ^ (s + 2) ^ ... ^ (s + n - 1))`

- $(s \oplus (s + 1) \oplus \cdots \oplus (s + n - 1)) = $ `(0 ^ 1 ^ 2 ^ 3 ^ (s - 1)) ^ (0 ^ 1 ^ 2 ^ 3 ^ ... ^ (s - 1) ^ s ^ (s + 1) ^ (s + 2) ^ ... ^ (s + n - 1))`

- So we can just compute:
    - `sumXor(s - 1) ^ sumXor(s + n - 1)`
    - `sumXor()` function determines, based on the input `n`, how many complete groups of four consecutive numbers can be canceled out, and what remains afterward.
