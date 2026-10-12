class employee :
    company = "Google"
    def show(self):
       print(f"the name is {self.company} and salary is {self.company} ")

      
class programmer(employee):
    company = "Microsoft"
    def showlanguage(self):
        print(f"the name is {self.company} and language is {self.company}")
        
        
a = employee()
b = programmer()
print(a.company,b.company)
                