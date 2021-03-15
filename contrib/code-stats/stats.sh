#!/bin/bash

function calculateLinesInFiles() {
    LINES=0
    LINES_CPP=$((find $1 -name "*.cpp" -print0 | xargs -0 cat ) | wc -l)
    LINES=$(( $LINES + $LINES_CPP))
    LINES_H=$((find $1 -name "*.h" -print0 | xargs -0 cat ) | wc -l)
    LINES=$(( $LINES + $LINES_H))
    LINES_HPP=$((find $1 -name "*.hpp" -print0 | xargs -0 cat ) | wc -l)
    LINES=$(( $LINES + $LINES_HPP))
    LINES_C=$((find $1 -name "*.c" -print0 | xargs -0 cat ) | wc -l)
    LINES=$(( $LINES + $LINES_C))
    LINES_CC=$((find $1 -name "*.cc" -print0 | xargs -0 cat ) | wc -l)
    LINES=$(( $LINES + $LINES_CC))
    echo $LINES

}

LINES_SRC=$(calculateLinesInFiles "../../src")
LINES_WSJCPP=$(calculateLinesInFiles "../../src.wsjcpp")

echo "Lines in src: $LINES_SRC"
echo "Lines in src.wsjcpp: $LINES_WSJCPP"
