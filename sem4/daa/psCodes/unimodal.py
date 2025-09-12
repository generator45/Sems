def unimodalPeak(arr):
    low = 0 
    high = len(arr) - 1
    while(low < high):
        mid = (low+high)//2
        if(arr[mid] < arr[mid]+1 and arr[mid] > arr[mid-1]):
            return arr[mid]
        elif(arr[mid] < arr[mid+1]):
            low = mid+1
        else:
            high = mid-1
    return 0


arr = [5, 6, 7, 9, 8, 4, 3, 2]
arr2 = [1, 3, 7, 8, 12,11, 6, 4, 2]
print(unimodalPeak(arr))
print(unimodalPeak(arr2))