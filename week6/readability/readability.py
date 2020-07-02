import re
text = input("Text:")

n_letters = len(re.findall(r"\w", text))
n_words = len(re.findall(r"\s", text)) + 1
n_sentences = len(re.findall(r"\!|\.|\?", text))

index = 0.0588 * (n_letters / n_words) * 100 - 0.296 * (n_sentences / n_words) * 100 - 15.8

if index > 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {round(index)}")
