CC := gcc -Wall -Wextra

all: q1/phrases q2/addmx q3/cypher

q1/phrases: ./q1/phrases.c
	$(CC) ./q1/phrases.c -o ./q1/phrases


q2/addmx: ./q2/addmx.c
	$(CC) ./q2/addmx.c -o ./q2/addmx


q3/cypher: ./q3/cypher.c
	$(CC) ./q3/cypher.c -o ./q3/cypher

clean: 
	rm -f q1/phrases q2/addmx q3/cypher