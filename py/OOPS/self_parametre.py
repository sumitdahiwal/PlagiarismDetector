class student :
    language = "python" # thsi is class attribuites
    salary = 100000
    def getinfo(self):
        print(f"the langugage is {self.language} and salary is {self.salary}")
    
    @staticmethod  # we also use this static method for we have dont pass whole object
    def greet():
        print("Good Morning")
        
        
sumit = student()
sumit.language = "Javascript"# this is instance class attribuite
print( sumit.language , sumit.salary)
sumit.getinfo()  # -> student.getinfo(sumit)
sumit.greet()

