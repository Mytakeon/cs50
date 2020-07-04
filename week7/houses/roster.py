import sys
import cs50

if len(sys.argv) != 2:
    print("Wrong number of arguments. Usage: python roster.py <house_name>")
    sys.exit(1)

students_db = cs50.SQL("sqlite:///students.db")
students = students_db.execute("select * from students WHERE house=? ORDER BY last, first", sys.argv[1])

for student in students:
    _, first, middle, last, house, birth = student.values()
    if middle:
        print(f"{first} {middle} {last}, born {birth}")
    else:
        print(f"{first} {last}, born {birth}")
