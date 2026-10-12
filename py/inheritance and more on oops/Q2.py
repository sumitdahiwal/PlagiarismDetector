class animal:
    pass
class pets(animal):
    pass
class dog(pets):
    @staticmethod
    def bark():
        print("bow bow")
      
d = dog()   
d.bark()
    