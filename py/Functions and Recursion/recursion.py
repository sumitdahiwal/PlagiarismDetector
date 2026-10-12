def factorial (n): # type: ignore
    if(n==1 or n==0):
        return 1
    return n * factorial(n-1) # type: ignore

n = int(input("Enter a number : "))
print(f"the factorial of the number is {factorial(n)}")
