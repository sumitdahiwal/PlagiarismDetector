
try:
    a = int(input("enter a number : "))
    print(a)
    
except ValueError as v:
    print(v)
    
finally:
    print("I am in finally")
    
print("thank you!")
