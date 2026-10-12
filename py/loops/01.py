# for i in range(1,11):
#     print(i)
    
# l = [1,2,3,4,5,6,7,8,9,10]
# for i in l:
#     print(i)

n = int(input("Enter a number:"))
# for i in range(n,n*11,n):
#     print(i)

# fact = 1
# for i in range(1,n+1):
#     fact*=i
# print(fact)

# sum = 0
# for i in range(1,n+1):
#     sum+=i
# print(sum)
# print(sum / n)

# for i in range(1,n+10):
#     print(n*i)
flag = 1
for i in range(2,n):
    if(n%i==0):
        
        flag = 0
    
  
    
if(flag == 0):
    print(" not prime")
else:
    print(" prime")
 