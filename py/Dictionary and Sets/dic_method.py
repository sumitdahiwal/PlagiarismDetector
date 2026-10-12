marks = {
    "sumit" : 34,
    "shubham" : 12,
    "Harry" : 56
}
print(marks.items())
print(marks.keys())
print(marks.values())
marks.update({"shubham":99 , "Aarya" : 89})
print(marks)
print(marks.pop("shubham"))
print(marks)

print(marks.get("sumit"))
print(marks.get("shubham2"))
print(marks["shubham2"])  # this will give error

my_dict = {'a': 1, 'b': 2}
my_dict.clear()
print(my_dict)  # Output: {}

my_dict = {'a': 1, 'b': 2}
new_dict = my_dict.copy()
print(new_dict)  # Output: {'a': 1, 'b': 2}
