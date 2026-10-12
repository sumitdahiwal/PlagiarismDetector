class employee :
    salary = 234
    incremnet = 20
    @property
    def aftersalary(self):
        return (self.salary + self.salary * (self.incremnet/100))
 
e = employee()
print(e.aftersalary)

