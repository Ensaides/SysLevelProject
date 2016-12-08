library: ./src/main.c ./src/GUI.c ./src/Library.c ./src/LibraryFile.c ./src/Input.c ./src/Windows/MainMenu.c ./src/Windows/AddBookWindow.c ./src/Windows/DeleteBookWindow.c ./src/Windows/CheckOutWindow.c ./src/Windows/ReturnBookWindow.c ./src/Windows/StatusQueryWindow.c ./src/Windows/BookStatusWindow.c
	gcc -std=gnu11 -o./bin/library -I./include ./src/main.c ./src/GUI.c ./src/Library.c ./src/LibraryFile.c ./src/Input.c ./src/Windows/MainMenu.c ./src/Windows/AddBookWindow.c ./src/Windows/DeleteBookWindow.c ./src/Windows/CheckOutWindow.c ./src/Windows/ReturnBookWindow.c ./src/Windows/StatusQueryWindow.c ./src/Windows/BookStatusWindow.c -lncurses
