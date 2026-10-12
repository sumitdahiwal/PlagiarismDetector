n = int(input("Enter a rows:"))
for i in range(n):
    print(' ' * (n-i-1) + '* ' * (i+1))
    