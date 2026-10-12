class employee :
    company = "Google"
    def show(self):
       print(f"the name is {self.company} and salary is {self.company} ")

 
class coder:
    language = "python"
    def printlan(self):
        print(f"the language is {self.language}")      
class programmer(employee,coder):
    company = "Microsoft"
    def showlanguage(self):
        print(f"the name is {self.company} and language is {self.language}")
        
        
a = employee()
b = programmer()
print(a.company,b.company)
b.show()
b.showlanguage()
b.printlan()
                