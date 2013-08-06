def italic(line):
	return "''" + line + "''"

def bold(line):
	return "'''" + line + "'''"
	
def heading(line, level):
	level += 1
	head = "=" * level
	return head + " " + line + " " + head

# Horizontal rule
def hr():
	return "---"

# Unordered list (bullet list)
def ul(line, level):
	head = "*" * level
	return head + " " + line

# Ordered list (numbered list)
def ol(line, level):
	head = "#" * level
	return head + " " + line

# Links (from <a href="link">)
def a(line):
	return "[" + line + "]"
