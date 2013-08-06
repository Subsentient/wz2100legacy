def read(filename): # reads the file specified
	with open('mod/' + filename, 'r') as f:
		data = f.readlines()
		
		col_len = len(data)
		row_len = len(data[0].split(',')) # first line decides everything
		
		result = [['' for x in range(row_len)] for x in range(col_len)]
		for x in range(col_len):
			result[x] = data[x].split(',')
			for y in range(row_len):
				result[x][y] = result[x][y].strip()
		return result
