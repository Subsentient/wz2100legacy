from utils import readfile
from format import wiki
import messages
'''
Research tech tree stats generator.
'''

def main():
	data = readfile.read('stats/research/multiplayer/prresearch.txt')
	strings = messages.load()
	prereqs = {}
	
	for row in data:
		res_id = row[0]
		res_prereq = row[1]
		
		if res_id in prereqs:
			prereqs[res_id].append(res_prereq)
		else:
			prereqs[res_id] = [res_prereq]
	
	for res_id in prereqs:
		if res_id in strings:
			print(wiki.heading(strings[res_id] + " " + wiki.italic(res_id), 1))
		else:
			print(wiki.heading(res_id, 1))
		
		for res_prereq in prereqs[res_id]:
			if res_prereq in strings:
				print(wiki.ul(strings[res_prereq] + " " + wiki.italic(res_prereq), 1))
			else:
				print(wiki.ul(res_prereq, 1))
		
if __name__ == "__main__":
    main()
