
try:
    a = int(input("enter a number : "))
    print(a)
    
except ValueError as v:
    print(v)
    
else:
    print("I am in else")
    
print("thank you!")