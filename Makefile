FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror
# gcc -xc++ -lstdc++ -shared-libgcc
ifeq ($(OS), Linux)
	CHECK_FLAGS = -lgtest -pthread
else
	CHECK_FLAGS = -lgtest
endif
OS = $(shell uname)
TEST = tests.cpp

GCOV =--coverage
ifeq ($(OS), Linux)
	CHECK_FLAGS = -lgtest -pthread
else
	CHECK_FLAGS = -lgtest
endif

TEST_COMPILE = $(FLAGS) tests.cpp $(CHECK_FLAGS)

all: clean test

clean:
	rm -rf *.o *.g* *.info *.gcov_report report *.a *.dSYM a.out

gcov_report: s21_containers.h
	$(FLAGS) tests.cpp $(CHECK_FLAGS) --coverage
	./a.out
	lcov -t "test" -c -d  ./ --no-external --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./report/
	open ./report/index.html

test:
	$(TEST_COMPILE)
	./a.out

check_leaks:
	make test
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./a.out
else
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./a.out
endif

	make clean

clangcheck:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.h *.cpp
	
clangfix:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.h *.cpp