#!/bin/env bash

NUM_RUNS=5
DEPTH=5
TARGET_DIR="logs/compare_depth"

echo "start batch compare between AlphaBeta depth"

# mkdir -p "$TARGET_DIR"

mkdir -p build 
cd build
build_dir=$(pwd)
cmake .. && make

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program $NUM_RUNS times..."
    cd ..
    
    echo "$(pwd)"
    mkdir -p $TARGET_DIR
    cd $TARGET_DIR
    echo "$(pwd)"
    
    for((depth1=1; depth1<=$DEPTH; depth1++ )); do
        mkdir $depth1
        cd $depth1
        for((depth2=1; depth2<=$DEPTH; depth2++ )); do
            mkdir $depth2
            cd $depth2
            echo "$(pwd) $depth1 $depth2"
            for((i=1;i<=$DEPTH;i++)); do
                echo "Run ($depth1,$depth2): $i $(pwd)"
                sleep 1
                $build_dir/play 3 "$i.txt" $depth1 $depth2
                echo "----------------"
            done
            cd ..
        done
        cd ..
    done
else 
    echo "Compilation failed."
fi