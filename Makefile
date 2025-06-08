# --- CPP ---
UNAME = $(shell uname -s)
ifeq ($(UNAME),Darwin)
CXX=clang++
else
CXX=g++
endif

CPP_STD = -std=c++17
CXXFLAGS = $(CPP_STD) -Wall -Werror -Wextra
OPTFLAGS = -O2 -flto -march=native
LIBS = 
AR = ar

# --- GTest ---
G_DIR = googletest
GTEST_DIR = googletest/googletest
GTEST_BUILD_DIR = googletest/build
GTEST_LIB_DIR = $(GTEST_BUILD_DIR)/lib
GTEST_MAIN_LIB = $(GTEST_LIB_DIR)/libgtest_main.a
GTEST_CORE_LIB = $(GTEST_LIB_DIR)/libgtest.a
GTEST_LIBRARIES = $(GTEST_CORE_LIB) $(GTEST_MAIN_LIB)

# --- Project ---
GCOV_REPORT_DIR = html_gcov_report
TEST_DIR = unit_test
TEST_SOURCE = $(TEST_DIR)/tests.cpp
TEST_RUNNER = $(TEST_DIR)/run_tests.out
SRCMODULES = $(wildcard *.cpp)
OBJMODULES = $(SRCMODULES:.cpp=.o)
MAINBINARIES = s21_matrix_oop.a

# --- Targets ---
.PHONY: all test clean help

all:	$(MAINBINARIES)		## Build all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -c $^ -o $@

s21_matrix_oop.a:	CXXFLAGS += $(OPTFLAGS)
s21_matrix_oop.a:	$(OBJMODULES)		## Build static library
	$(AR) -rcs $@ $^

test:	CXXFLAGS += -g
test:	$(TEST_RUNNER)		## Run tests
	./$(TEST_RUNNER)

$(TEST_RUNNER): $(GTEST_LIBRARIES) $(MAINBINARIES)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include \
		$(TEST_SOURCE) $(MAINBINARIES) \
		-L$(GTEST_LIB_DIR) -lgtest -lgtest_main -lpthread \
		-o $@

$(GTEST_LIBRARIES):	submodules		## Build googletest
	mkdir -p $(GTEST_BUILD_DIR)
	cmake -S $(G_DIR) -B $(GTEST_BUILD_DIR) \
		-DBUILD_GMOCK=OFF -DCMAKE_CXX_COMPILER=$(CXX) \
		-DCMAKE_CXX_STANDARD=17
	$(MAKE) -C $(GTEST_BUILD_DIR)

clean:	clean_gcov		## Clean up
	find . -name "*.o" | xargs rm -f
	rm -f $(TEST_RUNNER) $(MAINBINARIES)
	rm -rf $(GTEST_BUILD_DIR)

help:		## Display this help screen
	@grep -h -E '^[a-zA-Z0-9_.-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

include Makefile.tools
