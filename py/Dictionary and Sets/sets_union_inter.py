s1 = {1,45,6}
s2 = {7,8,1,78}
print(s1.union(s2))
print(s1.intersection(s2))
print(s1.difference(s2))
print(s2.difference(s1))

a = {1, 2, 3}
b = {3, 4, 5}
print(a.symmetric_difference(b))  # {1, 2, 4, 5}

a = {1, 2}
b = {1, 2, 3}
print(a.issubset(b))  # True

a = {1, 2, 3}
b = {1, 2}
print(a.issuperset(b))  # True

a = {1, 2}
b = {3, 4}
print(a.isdisjoint(b))  # True



