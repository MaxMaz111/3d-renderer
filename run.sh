#!/bin/sh

BUILD_TYPE="Release"
RUN_AFTER_BUILD=false
CLEAN_BUILD=false
BUILD_DIR="build"
SOURCE_DIR=".."
BUILD_TARGET=""
RUN_ARGS=""

show_help() {
    echo "Usage: $0 [options] [-- executable_arguments]"
    echo "Options:"
    echo "  -t, --type TYPE       Build type (Debug or Release)"
    echo "  -r, --run             Run the executable after building"
    echo "  -c, --clean           Clean build directory before configuring"
    echo "  -b, --build-dir DIR   Specify build directory (default: build)"
    echo "  -s, --source-dir DIR  Specify source directory (default: ..)"
    echo "  -T, --target TARGET   Specify build target"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Arguments after -- will be passed to the executable when using --run"
    exit 0
}

# Process options until we see -- or run out of arguments
while [[ $# -gt 0 ]]; do
    key="$1"

    # Check if we've reached the argument separator
    if [[ "$key" == "--" ]]; then
        shift
        # Collect all remaining arguments for the executable
        RUN_ARGS="$@"
        break
    fi

    case $key in
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -r|--run)
            RUN_AFTER_BUILD=true
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -b|--build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -s|--source-dir)
            SOURCE_DIR="$2"
            shift 2
            ;;
        -T|--target)
            BUILD_TARGET="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            ;;
    esac
done

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "Error: Invalid build type. Valid options are: Debug, Release"
    exit 1
fi

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR" || { echo "Failed to change to build directory"; exit 1; }

if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directory..."
    rm -rf *
fi

echo "Configuring project with build type: $BUILD_TYPE"
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" "$SOURCE_DIR"

echo "Building project..."
if [ -n "$BUILD_TARGET" ]; then
    cmake --build . --config "$BUILD_TYPE" --target "$BUILD_TARGET" -j 8
else
    cmake --build . --config "$BUILD_TYPE" -j 8
fi

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build successful."

if [ "$RUN_AFTER_BUILD" = true ]; then
    echo "Looking for executable to run..."
    if [ -f "CMakeCache.txt" ]; then
        PROJECT_NAME=$(grep "CMAKE_PROJECT_NAME" CMakeCache.txt | cut -d'=' -f2)
        if [ -n "$PROJECT_NAME" ] && [ -x "$PROJECT_NAME" ]; then
            echo "Running $PROJECT_NAME with args: $RUN_ARGS"
            ./"$PROJECT_NAME" $RUN_ARGS
            exit 0
        fi
    fi

    for dir in . bin Debug Release; do
        if [ -d "$dir" ]; then
            EXECUTABLE=$(find "$dir" -type f -executable -not -name "*.sh" -not -name "*.cmake" | head -n 1)
            if [ -n "$EXECUTABLE" ]; then
                echo "Running $EXECUTABLE with args: $RUN_ARGS"
                "$EXECUTABLE" $RUN_ARGS
                exit 0
            fi
        fi
    done

    echo "No executable found to run. Please specify the path manually."
fi
