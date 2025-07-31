
from os import listdir
import os
from os.path import isfile, join
onlyfiles = [f for f in listdir("src") if isfile(join("src", f))]

#print(onlyfiles)
for file in onlyfiles:
    print('compiling ' + file)
    os.system("glslc " + join("src", file) + " -o " + join("bin", file[:-4]) + file[-4:] + ".spv")

