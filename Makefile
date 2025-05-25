UNAME = $(shell uname -s)
ifeq ($(UNAME),Darwin)
CXX=clang++
else
CXX=g++
endif
CPP_STD = -std=c++17
CXXFLAGS = $(CPP_STD) -Wall -Werror -Wextra
OPTFLAGS = -O2 -flto -march=native

GTEST_DIR = googletest/googletest
GTEST_LIB = -I$(GTEST_DIR)/include -I$(GTEST_DIR) $(GTEST_DIR)/src/gtest-all.cc
GCOV_REPORT_DIR = ./html_gcov_report
TEST_DIR = unit_test
TEST_RUNNER = $(TEST_DIR)/run_tests.out

AR = ar

SRCMODULES = $(wildcard *.cpp)
OBJMODULES = $(SRCMODULES:.cpp=.o)

MAINBINARIES = s21_matrix_oop.a

all:	$(MAINBINARIES)		## Build all

test:	CXXFLAGS += -g
test:	build_googletest clean s21_matrix_oop.a		## Run tests
	$(CXX) $(CXXFLAGS) $(GTEST_LIB) $(TEST_DIR)/tests.cpp s21_matrix_oop.a -o $(TEST_RUNNER)
	./$(TEST_RUNNER)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $(OPTFLAGS) -c $^ -o $@

s21_matrix_oop.a:	$(OBJMODULES)		## Build static library
	$(AR) -rcs $@ $^

clean:		## Clean up
	find . -name "*.o" | xargs rm -f
	rm -f report.info $(TEST_RUNNER) $(MAINBINARIES)
	rm -rf $(GCOV_REPORT_DIR)/

help:		## Display this help screen
	@grep -h -E '^[a-zA-Z0-9_.-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

include Makefile.tools
