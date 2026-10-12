#  using walrus operator
if (n:= len([1,2,3,4,5])) > 3:
    print(f"List is too long ({n} elments, excepted <=3)")
    
    
numbers = [3, 5, 7, 9, -1, 2]
while (n := numbers.pop(0)) > 0:  # Assign and check in one line // 
    print(f"Processing number: {n}")
print(numbers)
