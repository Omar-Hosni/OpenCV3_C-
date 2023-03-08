print("enter grades\n")

english = int(input("English: "))
math = int(input("Math: "))
science = int(input("Science: "))

avg = (english+science+math)/3

if avg > 40:
    print("passed")
else:
    print("failed")