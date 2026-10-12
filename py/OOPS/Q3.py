class demo:
    a=4
    
o = demo() # prints the class attribute because istance attribute is not present
print(o.a) 
o.a = 0 # instance atribute is present
print(o.a) # print the instance attribute because instance attribute is present
print(demo.a)

   