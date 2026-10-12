class employee:
    a = 1
    @classmethod
    def show(cls):
        print(f"the value of a is {cls.a}")
        
e = employee()
e.a = 45
e.show()
print(e.a)        
