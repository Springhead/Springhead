import sys
import os

argc = len(sys.argv)
if argc > 1:
	spos = 1 if sys.argv[1][0] == '-' else 0
	max_count = int(sys.argv[1][spos:])
else:
	max_count = sys.maxsize

count = 0
while count < max_count:
	try:
		s = input()
	except:
		break
	print(s)
	count += 1

while True:
	try:
		input()
	except EOFError:
		break

sys.exit()
