n = int(input("Enter a rows:"))
# for i in range(n,0,-1):
#     for j in range(i):
#         print("*",end=" ")
#     print()    
    
    
for i in range(n):
    print(" "*i,end=" ")
    print("*"*(n-i),end=" ")
    print()    