s = {1,5,32,54,5,5,5,"Sumit"} 
print(s,type(s))
s.add(566)
print(s,type(s))

d = {1, 2}
d.update([3, 4])
print(d)  # {1, 2, 3, 4}

d = {1, 2, 3}
d.remove(2)
print(d)  # {1, 3}

d = {1, 2, 3}
d.discard(4)  # No error
print(d)  # {1, 2, 3}

d = {1, 2, 3}
print(d.pop())  # 1 (or any other element)
print(d)  # Remaining elements

d = {1, 2, 3}
d.clear()
print(d)  # set()

