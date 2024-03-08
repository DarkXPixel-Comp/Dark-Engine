from ast import List
import pathlib
import glob
import os
import subprocess
import fileinput
import time


results = []
results1 = []
results2 = []

def find_ext(dir_path, extension, current):
	# Check if the given path is a valid directory
	
	if not os.path.isdir(dir_path):
		return
	# Get a list of all the files and directories in the given directory
	files_and_dirs = os.listdir(dir_path)
	# Iterate over the list of files and directories
	for file_or_dir in files_and_dirs:
		# Construct the full path of the file or directory
		full_path = os.path.join(dir_path, file_or_dir)
		current = file_or_dir
	
		# If the full path is a directory, recursively call the function to find the files in that directory
		if os.path.isdir(full_path):
			find_ext(full_path, extension, current)
		else:
			# If the full path is a file that ends with extension, print its path
			if full_path.endswith(extension):
				results1.append(str(current).removesuffix(extension))
				results2.append(str(full_path).removesuffix(str(current)))
				#print(full_path)



def find_files(dir_path, extension):
	# Check if the given path is a valid directory
	
	if not os.path.isdir(dir_path):
		return
	# Get a list of all the files and directories in the given directory
	files_and_dirs = os.listdir(dir_path)
	# Iterate over the list of files and directories
	for file_or_dir in files_and_dirs:
		# Construct the full path of the file or directory
		full_path = os.path.join(dir_path, file_or_dir)
	
		# If the full path is a directory, recursively call the function to find the files in that directory
		if os.path.isdir(full_path):
			find_files(full_path, extension)
		else:
			# If the full path is a file that ends with extension, print its path
			if full_path.endswith(extension):
				results.append(full_path)
				#print(full_path)


def get_path():
	p = str(pathlib.Path(__file__).parent.resolve()).replace("\\", "/")
	return p



find_ext(".", '.module.sharpmake.cs', str())




fi = []



#time.sleep(5)

	








find_files(".", '.sharpmake.cs')


for i in range(len(results)):
	results[i] = results[i][2:]
	


for i in range(len(results)):
	results[i] = results[i].replace('\\', '/')
	

for i in range(len(results2)):
	results2[i] = results2[i][2:]
	
for i in range(len(results2)):
	results2[i] = results2[i].replace('\\', '/')

#res += f"typeModules.Add(typeof({i})); "


res = str()

for i in range(len(results1)):
	res += f"typeModules.Add(new Module(typeof({results1[i]}), \"{results2[i]}\")); "

# for i in results1:
# 	res += f"typeModules.Add(typeof({i})); "
	

#print(results1)
#print(results2)

#time.sleep(10)

with open("DarkEngine.sharpmake.cs", "r") as file:
	lines = file.readlines()
	
	for line in lines:
		if("/*GENERATE_PATH*/" in line):
			line = f"\t/*GENERATE_PATH*/ public static string PathSolution = \"{get_path()}\";\n"
			#fi.append(line)
		if("/*GENERATE_MODULES*/" in line):
			line = "\t/*GENERATE_MODULES*/ static Modules() { " + res + " }\n"
			
		fi.append(line)
		#print(line)	
	



for i in fi:
	print(str(i))

with open("DarkEngine.sharpmake.cs", "w") as file:
	file.writelines(fi)


st = str()
for i in range(len(results)):
	st += "'" + results[i] + "',"

st = st[:-1]


subprocess.call('"Dark Engine/Programs/SharpMake/Sharpmake.Application.exe" " /sources(' + st + ') /verbose"')
#os.system('"Dark Engine/Programs/SharpMake/Sharpmake.Application.exe" " /sources(' + st + ')"')



# print(st)
