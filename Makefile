build:
	gcc run-loop.c -o run-loop -lm -Wall
convert:
	cd test-content && ./convert.py
