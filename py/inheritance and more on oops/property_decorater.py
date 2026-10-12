class employee:
    a = 1
    
    @classmethod
    def show(cls):
        print(f"The class attribute is {cls.a}")
        
    @property
    def name(self):
        return f"{self.fname} {self.lname}" # type: ignore
    
    @name.setter
    def name(self, value): # type: ignore
        self.fname = value.split(" ")[0] # type: ignore
        self.lname = value.split(" ")[1] # type: ignore
        
e = employee()
e.a = 45

e.name = "Sumit Dahiwal"
print(e.fname, e.lname)  # type: ignore # Output: Sumit Dahiwal

e.show()  # Output: The class attribute is 1
