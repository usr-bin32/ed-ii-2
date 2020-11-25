start:
	g++ src/main.cpp src/tests/hashing.cpp src/tests/common.cpp src/tests/balanced.cpp src/structures/author.cpp src/parsing/csv_parser.cpp -Ofast -std=c++11 -o ./project.out