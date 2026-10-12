import random
n = random.randint(1,100)
a = -1
guess = 1
print("you have only 6 attempts")
while(a!=n and guess<=6):
    a = int(input("Guess a number: "))
    if(a>n):
        print("lower number please")
        guess+=1
    elif(a<n):
        print("higher number please")
        guess+=1
        
if(guess<=6 and a==n):
    print(f"you have guessed the correct number i.e {n} in {guess}th attempts")
else:
    print(f"You lose , please try again , guessed number is {n}")

