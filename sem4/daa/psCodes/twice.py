def find_single_element(A):
    left = 0
    right = len(A) - 1
    
    while left < right:
        mid = (left + right) // 2
        
        # Ensure mid is even
        if mid % 2 == 1:
            mid -= 1
        
        # Compare A[mid] and A[mid + 1]
        if A[mid] == A[mid + 1]:
            # The single element is to the right of mid
            left = mid + 2
        else:
            # The single element is at or to the left of mid
            right = mid
    
    # When left == right, we have found the single element
    return A[left]

arr = [1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6]
print(find_single_element(arr))




    
    