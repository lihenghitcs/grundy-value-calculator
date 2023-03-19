# grundy-value-calculator
grundy-value-calculator is a tool to compute the value of the Grundy function for proper 2-coloring game on trees resembling the following figure.

![image](https://github.com/Zireael775333728/grundy-value-calculator/blob/main/.readme/graph.png)

# Usage:
Compile it with gcc:
```
gcc grundy.c -o grundy
```

To compute the value to a specific N, you can:
```
./grundy -n <endN>
```

To read the existing results from `./result/` and continue the calculation from the results to a specific N, you can:
```
./grundy -r -n <endN>
```

# Result

Results will be saved in `./result/` as csv files. The data in row p and column q represents the Grundy value of `n=p-1, i=q-1`.

# Test

Use `judge.py` to compare the program's output with the theoretical results.

```
python judge.py
```
