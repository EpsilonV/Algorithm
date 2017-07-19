"""Binomial Coefficients.
   1) A binomial coefficient C(n, k) can be defined as the coefficient of X^k in the expansion of (1 + X)^n.
   2) A binomial coefficient C(n, k) also gives the number of ways, disregarding order, that k objects can be chosen from among n objects; more formally, the number of k-element subsets (or k-combinations) of an n-element set."""

#C(n, k) = C(n-1, k) + C(n-1, k-1)
#C(n, 0) = C(n, n) = 1

def func(n, k):
    if k == 0 or k == n:
        return 1
    return func(n-1, k-1) + func(n-1, k)

def func_optima(n, k):
    C = [[0 for x in range(k+1)] for x in range(n+1)]
    for i in range(n+1):
        for j in range(min(i, k)+1):
            if j == 0 or j == i:
                C[i][j] = 1
            else:
                C[i][j] = C[i-1][j-1] + C[i-1][j]

    return C[n][k]

if __name__ == '__main__':
    print(func_optima(5, 2), func(5, 2))