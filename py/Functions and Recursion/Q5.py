def pattern(n): # type: ignore
    if(n==0):
        return
    print("*"*n) # type: ignore
    pattern(n-1) # type: ignore


num = int(input("Enter a number : "))
print(pattern(num))
