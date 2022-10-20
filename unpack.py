import shutil
import os

def recurse(directory='.', student=''):
    for filename in os.scandir(directory):
        student = student if student else "./submissions/" + (filename.path[15:]).split('@')[0].replace(".","-") + ".cpp"
        if filename.is_file():
            if ("zip" in filename.path):
                shutil.unpack_archive(filename.path, "./temp")
                recurse("./temp",student)
                shutil.rmtree("./temp")
            if ("cpp" in filename.path):
                print('ff',filename.path, student)
                os.makedirs(os.path.dirname(student), exist_ok=True)
                shutil.copyfile(filename.path, student)
            
        if filename.is_dir():
            recurse(filename.path, student)
        student=''
shutil.rmtree("./submissions")  
recurse()