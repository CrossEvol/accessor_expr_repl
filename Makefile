# Default compiler choice
COMPILER_CHOICE ?= gcc

# Build directory
BUILD_DIR = build

# CMake configuration options
CMAKE_OPTIONS = -DCOMPILER_CHOICE=$(COMPILER_CHOICE)

.PHONY: all debug release clean list-env

# Default target
all: debug release

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Configure and build debug version
debug: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. $(CMAKE_OPTIONS) -DCMAKE_BUILD_TYPE=Debug
	cd $(BUILD_DIR) && cmake --build . --config Debug

# Configure and build release version
release: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. $(CMAKE_OPTIONS) -DCMAKE_BUILD_TYPE=Release
	cd $(BUILD_DIR) && cmake --build . --config Release

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build both debug and release versions (default)"
	@echo "  debug      - Build debug version"
	@echo "  release    - Build release version"
	@echo "  clean      - Remove build directory"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  COMPILER_CHOICE - Set compiler (gcc, clang, or msvc) [default: gcc]" 