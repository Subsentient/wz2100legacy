from utils import readfile
'''
Research tech tree stats generator.
'''

def main():
	data = readfile.read('stats/research/multiplayer/research.txt')
	for row in data:
		print(', '.join(row))
