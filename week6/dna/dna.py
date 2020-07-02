import re
import sys

if len(sys.argv) != 3:
    print("Invalid number of arguments. Example usage: python dna.py databases/small.csv sequences/1.txt")
    quit()

_, fpath_db, fpath_dna = sys.argv

dna_database = [line.strip().split(",") for line in open(fpath_db, "r").readlines()]
dna = open(fpath_dna, "r").read()

match = []
for str_ in dna_database[0][1:]:  # first row minus the "name" column
    groups = re.findall(f"((?:{str_})+)", dna)
    if groups:
        max_length = max(len(group) for group in groups)
        n = str(int(max_length / len(str_)))
    else:
        n = "0"
    match.append(n)

# Compare match with database
for person in dna_database[1:]:
    if person[1:] == match:
        print(person[0])
        quit()

print("No match")
