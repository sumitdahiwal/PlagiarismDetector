marks = int(input("Enter marks: "))

if 91 <= marks <= 100:
    grade = 'EX'
elif 81 <= marks <= 90:
    grade = 'A'
elif 71 <= marks <= 80:
    grade = 'B'
elif 61 <= marks <= 70:
    grade = 'C'
elif 51 <= marks <= 60:
    grade = 'D'
else:
    grade = 'F'
print("Grade:", grade)
