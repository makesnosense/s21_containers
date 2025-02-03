#━━━━━━━━━━Make Configuration━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: test clean coverage valgrind all
.DEFAULT_GOAL := test

#━━━━━━━━━━Compiler and Base Flags━━━━━━━━━━━━━━━━━━
CXX = g++
STD_FLAGS := -std=c++17 -pedantic
WARN_FLAGS := -Werror -Wextra -Wall -Weffc++ -Wconversion -Wsign-conversion -Wpadded
DEPENDENCY_FLAGS := -MMD -MP
INCLUDE_DIRS := -I./src
DEBUG_FLAG := -g
CXXFLAGS := $(STD_FLAGS) $(WARN_FLAGS) $(DEPENDENCY_FLAGS) $(INCLUDE_DIRS) $(DEBUG_FLAG)

#━━━━━━━━━━Test and Coverage Configuration━━━━━━━━━━
TESTS_LINKER_FLAGS := -lgtest_main -lgtest -pthread
COVERAGE_FLAGS := -fprofile-arcs -ftest-coverage
GCOVR_THEME := --html-theme github.dark-green
GCOVR_CONFIG_FLAGS := --html --html-single-page=js-enabled --html-details
GCOVR_EXCLUDES := --exclude-unreachable-branches --exclude-throw-branches --exclude-noncode-lines
GCOVR_FLAGS := $(GCOVR_CONFIG_FLAGS) $(GCOVR_EXCLUDES) $(GCOVR_THEME)
VALGRIND_FLAGS := --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes --track-origins=yes --verbose

#━━━━━━━━━━Directory Structure━━━━━━━━━━━━━━━━━━━━━━
SRC_DIR := src
TESTS_SRC_DIR := tests
BUILD_DIR := build
TESTS_BUILD_DIR := $(BUILD_DIR)/tests
COVERAGE_DIR := $(BUILD_DIR)/coverage

#━━━━━━━━━━Files and Dependencies━━━━━━━━━━━━━━━━━━━
-include $(TESTS_BUILD_DIR)/*.d
TESTS_SRC_FILES := $(wildcard $(TESTS_SRC_DIR)/*.cc)
TESTS_OBJ_FILES := $(patsubst $(TESTS_SRC_DIR)/%.cc,$(TESTS_BUILD_DIR)/%.o,$(TESTS_SRC_FILES))

#━━━━━━━━━━OS-Specific Configuration━━━━━━━━━━━━━━━━
ifeq ($(shell uname),Darwin)
	OPEN_COMMAND := open
else
	OPEN_COMMAND := xdg-open
endif

#━━━━━━━━━━Targets━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
test: $(BUILD_DIR)/run_tests
	./$(BUILD_DIR)/run_tests 

all: coverage valgrind

$(BUILD_DIR)/run_tests: $(TESTS_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ $(TESTS_LINKER_FLAGS) -o $@

$(TESTS_BUILD_DIR)/%.o: tests/%.cc
	@mkdir -p $(TESTS_BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

coverage: CXXFLAGS += $(COVERAGE_FLAGS)
coverage: clean $(BUILD_DIR)/run_tests
	mkdir -p $(COVERAGE_DIR)
	./$(BUILD_DIR)/run_tests
	@gcovr -o $(COVERAGE_DIR)/gcov_report.html $(GCOVR_FLAGS) --exclude $(TESTS_SRC_DIR)
	@$(OPEN_COMMAND) $(COVERAGE_DIR)/gcov_report.html &
	rm -f *.gcda *.gcno *.gcov

clean:
	rm -rf $(BUILD_DIR)

valgrind: $(BUILD_DIR)/run_tests
	valgrind $(VALGRIND_FLAGS) ./$(BUILD_DIR)/run_tests
