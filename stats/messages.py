from utils import readfile
'''
Functions for obtaining actual text of stuff in messages/strings/names.txt
'''

def load():
	f = open('mod/messages/strings/names.txt', 'r')
	data = f.readlines()
	
	dic = {}
	for line in data:
		line = line.strip().replace("\t", " ")
		if not (line.startswith("/*") or line.startswith(" *") or line.startswith(" */") or line.startswith("//")):
			end1 = line.find(" ")
			start2 = line.find("_(\"") + 3
			end2 = line.find("\")")
			
			dic[line[:end1]] = line[start2:end2]
	return dic
	
