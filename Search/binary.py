"""
Given a sorted array of n values arr[], 
write a function to search for a particular element x in the array.
"""

def binarySearchItre(arr, x):
    l, r = 0, len(arr)

    while l <= r:
        mid = (l+r)//2
        if arr[mid] == x:
            return mid + 1
        elif arr[mid] > x:
            r = mid - 1
        elif arr[mid] < x:
            l = mid + 1
    return -1


def binarySearchRecu(arr, l, r, x):
    if l > r:
        return -1

    mid = (l + r)//2
    if arr[mid] > x:
        return binarySearchRecu(arr, l, mid-1, x)
    elif arr[mid] < x: 
        return binarySearchRecu(arr, mid+1, r, x)
    if arr[mid] == x:
        return mid + 1

if __name__ == '__main__':
    print(binarySearchRecu([1,3,5,6,8,9,12,17],0, 8, 17))
    print(binarySearchItre([1,3,5,6,8,9,12,17], 17))