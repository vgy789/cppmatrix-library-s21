include Makefile.tools

CXX = g++ -std=c++17
CXXFLAGS = -g -Wall -Werror -Wextra
OPTFLAGS = -O2 -flto -march=native

GTEST_DIR = ../googletest
GTEST_LIB = -I$(GTEST_DIR)/googletest/include -I$(GTEST_DIR)/googletest $(GTEST_DIR)/googletest/src/gtest-all.cc
GCOV_REPORT_DIR = ./html_gcov_report
TEST_RUNNER = run_tests

AR = ar

SRCMODULES = $(wildcard *.cpp)
OBJMODULES = $(SRCMODULES:.cpp=.o)

MAINBINARIES = s21_matrix_oop.a

all:	$(MAINBINARIES)		## Build all

test: clean s21_matrix_oop.a		## Run tests

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $(OPTFLAGS) -c $^ -o $@

s21_matrix_oop.a:	$(OBJMODULES)		## Build static library
	$(AR) -rcs $@ $^

clean:		## Clean up
	find . -name "*.o" | xargs rm -f
	rm -f report.info $(TEST_RUNNER) $(MAINBINARIES)
	rm -rf $(GCOV_REPORT_DIR)/

gcov_report:	test		## Generate gcov report
	lcov -c -t "$(TEST_RUNNER)" --directory ./ -o report.info --exclude *tests.c
	genhtml -o $(GCOV_REPORT_DIR) report.info

help:		## Display this help screen
	@grep -h -E '^[a-zA-Z0-9_.-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
