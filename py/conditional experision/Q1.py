a = int(input("Enter 1st number = "))
b = int(input("Enter 2nd number = "))
c = int(input("Enter 3rd number = "))
d = int(input("Enter 4th number = "))

if(a>b and a>c and a>d):
    print(a,"is the largest number")
elif(b>a and b>c and b>d):
    print(b,"is the largest number")
elif(c>a and c>b and c>d):
    print(c,"is the largest number")
else:
    print(d,"is the largest number")
    


# largest = max(a, b, c, d)
# print(largest, "is the largest number")
