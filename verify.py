from re import sub
import shutil
import os
directory = '.'
submissions = []
gathered = []
for filename in os.scandir(directory):
    if ".py" not in filename.path:
        student = (filename.path[15:]).split('@')[0].replace(".","-")
        student = student + ".cpp"
        submissions.append(student)
for filename in os.scandir('./submissions'):
    gathered.append(filename.path[14:])
print(set(gathered))
print('\n\n\n\n')
print(set(submissions))
print((set(submissions) - set(gathered)))
print(len(set(submissions) - set(gathered)))
        
