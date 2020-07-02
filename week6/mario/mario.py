from cs50 import get_int

n = 99  # non valid starting height
while n < 1 or n > 8:
    n = get_int("Height:")

for i in range(1, n + 1):
    print(" " * (n - i) + "#" * i + "  " + "#" * i)
