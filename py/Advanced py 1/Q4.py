try:
    a = int(input("Enter a number: "))
    b = int(input("Enter a number: "))
    print(a / b)

except ZeroDivisionError:
    print("Infinite (Cannot divide by zero)")

