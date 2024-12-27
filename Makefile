cpplox:
	g++ -o cpplox src/main.cpp src/Interpreter/Interpreter.cpp src/Error/Error.cpp src/Token/Token.cpp src/Scanner/Scanner.cpp

clean:
	rm -f cpplox