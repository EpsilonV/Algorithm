"""
Given a sorted array of n values arr[], 
write a function to search for a particular element x in the array.
"""
import math
def jumpSearch(arr, x):
    k = math.floor(math.sqrt(len(arr)))
    end = len(arr)//k
    i = 1
    while arr[i*k-1] < x and i <= end:
        i += 1
    if i-1  != end:
        for j in range((i-1)*k, i*k):
            if arr[j] == x:
                return j+1
    else:
        for j in range(i*k, len(arr)):
            if arr[j] == x:
                return j+1

    return -1

if __name__ == '__main__':
    print(jumpSearch([1,3,5,7,9,14,17,18,19,42,67,87], 19))