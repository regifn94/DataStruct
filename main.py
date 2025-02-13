arr = [1,2,3,2,7,4,1]

largest = arr[0]
# print(arr[1])  # prints 1
for i in arr:
    if i > largest:
        largest = i


print("max : ", largest)