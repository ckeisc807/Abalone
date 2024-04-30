#!/bin/env bash

NUM_RUNS=10
DEPTH=3
TARGET_DIR="logs/depth$DEPTH"

echo "start batch compare between AlphaBeta and MinMax speed"

mkdir -p "$TARGET_DIR"
if [ ! -d "$TARGET_DIR" ]; then
    mkdir -p "$TARGET_DIR"
    echo "$TARGET_DIR created"
    sleep 5
fi

mkdir build
cd build 
cmake .. && make 


# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program $NUM_RUNS times..."

    # Run the program multiple times
    for (( i=1; i<=$NUM_RUNS; i++ )); do
        echo "Run $i:"
        ./play 1 "../$TARGET_DIR/$i.txt"
        echo "---------------------"
    done

else
    echo "Compilation failed. Exiting..."
fi