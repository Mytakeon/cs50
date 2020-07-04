import sys
import cs50

if len(sys.argv) != 2:
    print("Wrong number of arguments. Usage: python import.py characters.csv")
    sys.exit(1)

db = cs50.SQL("sqlite:///students.db")

with open(sys.argv[1], 'r') as file:
    students = [line.strip().split(',') for line in file.readlines()][1:]

for student in students:
    names = student[0].split(" ")
    middle = None if len(names) == 2 else names[1]
    db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
               names[0], middle, names[-1], student[1], student[2])
