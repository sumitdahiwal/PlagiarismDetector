class employee:
    a = 1
class coder(employee):
    b = 2
class programmer(coder):
    c = 3
  
a = programmer()
print(a.a,a.b,a.c)

b = coder()
print(b.a,b.b)

c = employee()
print(c.a)
    