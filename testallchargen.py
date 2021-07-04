# writing to file
#file1 = open('myfile.txt', 'w')
#file1.writelines(L)
#file1.close()

# Using readlines()
file1 = open('testallchar.txt', 'r')
Lines = file1.readlines()

count = 0
# Strips the newline character
for line in Lines:
	count += 1
	print("case({}):\n{}break;".format(count-1, line.strip()))
