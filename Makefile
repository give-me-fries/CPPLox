cpplox:
	g++ -o cpplox src/main.cpp src/Interpreter/Interpreter.cpp src/ErrorHandler/ErrorHandler.cpp

clean:
	rm -f cpplox