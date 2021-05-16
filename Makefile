rules = -Wall -Wextra -g
exec_name = tema2

sources = main.c browser.c structs.c utils.c

build:
	gcc $(sources) $(rules) -o $(exec_name)
clean:
	rm $(exec_name)
run:
	./$(exec_name)