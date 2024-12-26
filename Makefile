cpplox:
	g++ -o cpplox src/main.cpp src/Interpreter/Interpreter.cpp src/Error/Error.cpp

clean:
	rm -f cpplox