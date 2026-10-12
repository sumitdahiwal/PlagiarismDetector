class employee:
    def __init__(self):
        super().__init__()
        print("constructor of employee")
    a = 1
    
class coder(employee):
    def __init__(self):
        super().__init__()
        print("constructor of coder")
    b = 2
    
class programmer(coder):
    def __init__(self):
        super().__init__()
        print("constructor of programmer")
    c = 3
    
a = programmer()
print(a.a, a.b, a.c)

b = coder()
print(b.a, b.b)

c = employee()
print(c.a)
