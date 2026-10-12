with(
    open('poem.txt') as f1,
    open('poem_copy.txt', 'w') as f2
):
    for line in f1:
        f2.write(line)
        