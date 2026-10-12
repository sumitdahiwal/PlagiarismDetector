a = int(input("Enter a number : "))
b = int(input("Enter a number : "))
 
if(b==0):
     raise ZeroDivisionError("hey our program can't divide by zero")
else:
     print(a/b)
     
print("thank you!")