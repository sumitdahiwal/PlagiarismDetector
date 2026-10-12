def sum(n): # type: ignore
    if(n==1):
        return 1
    return sum(n-1)+n # type: ignore

num = int(input("Enter a numer : "))

print(sum(num)) # type: ignore
