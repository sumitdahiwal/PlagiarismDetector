def big(a,b,c): # type: ignore
    if(a>b and a>c):
        return a # type: ignore
    elif(b>a and b>c):
        return b # type: ignore
    else:
        return c # type: ignore
    
a = int(input("Enter a number : "))
b = int(input("Enter a number : "))
c = int(input("Enter a number : "))
print(big(a,b,c))

            