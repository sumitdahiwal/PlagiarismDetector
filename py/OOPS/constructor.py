class student :
    language = "python" # thsi is class attribuites
    salary = 100000
    
    def __init__(self,name,salary,language):  # dunder method no need to call this function
        self.name = name
        self.salary = salary
        self.language = language
        print("I am very happy")
        
        
    def getinfo(self):
        print(f"the langugage is {self.language} and salary is {self.salary}")
    
    @staticmethod  # we also use this static method for we have dont pass whole object
    def greet():
        print("Good Morning")
        
        
sumit = student("sumit",1200000,"java")
# this is instance class attribuite
print( sumit.language , sumit.salary)
sumit.getinfo()  # -> student.getinfo(sumit)
sumit.greet()
print(sumit.language,sumit.salary,sumit.name)

