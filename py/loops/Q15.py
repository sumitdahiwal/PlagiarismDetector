# n = int(input("enter a rows:"))
# for i in range(n,0,-1):
#     for j in range(1,i+1):
#         print(i+1-j,end=" ")
#     print()
        
        
n = int(input("Enter a rows:"))
for i in range(n,0,-1):
    for j in range(i,0,-1):
        print(j,end=" ")
    print()
                