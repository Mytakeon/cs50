n = input("Card number:")
l = len(n)

# Check brand
if l in (13, 16):
    if n[:2] in ("51", "52", "53", "54", "55"):
        card = "MASTERCARD"
    if n[0] == "4":
        card = "VISA"
elif l == 15 and n[:2] in ("34", "37"):
    card = "AMEX"
else:
    print("INVALID")
    quit()

# Use Luhn’s Algorithm
offset = l % 2  # 0 or 1, to ensure that we start from the last digit
l1 = [int(d) * 2 for i, d in enumerate(n) if i % 2 == 0 + offset]
s1 = sum([int(digit) for number in l1 for digit in str(number)])
s2 = sum([int(c) for i, c in enumerate(n) if i % 2 == 1 - offset])

if ((s1 + s2) % 10 == 0):
    print(card)
else:
    print("INVALID")
