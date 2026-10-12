from random import randint

class train :
    def __init__(self,trainNo):
        self.trainNo = trainNo
        
    def book (self,fro,to):
        print(f"ticket is booked in train no: {self.trainNo} from {fro} to {to}")
        
    def getstatus (self):
        print(f"the train no: {self.trainNo} is running on time")
        
    def getfare(self,fro,to):
        print(f"ticket fare in train no: {self.trainNo} from {fro} to {to} is {randint(222,5555)}")  
        
        
t = train(12345)
t.book("pune","Beed")  