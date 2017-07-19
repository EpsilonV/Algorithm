"""
Given a sorted array of n values arr[], 
write a function to search for a particular element x in the array.
"""

def linearSearch(arr, x):
    for index, c in enumerate(arr):
        if x == c:
            return index + 1
    return -1

if __name__ == '__main__':
    print(linearSearch([1,3,5,7,8], 7))

