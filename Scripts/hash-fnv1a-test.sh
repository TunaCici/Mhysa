#!/bin/bash

SAMPLE_INPUT="sample_input.txt"

echo "Generating a large sample input file with 64,000 elements..."
rm -f $SAMPLE_INPUT
for i in $(seq 1 64000); do
    echo "insert key$i $((i * 10))" >> $SAMPLE_INPUT
done

# Add some deletions and searches for testing
for i in $(seq 1 100); do
    echo "delete key$((i * 2))" >> $SAMPLE_INPUT
    echo "search key$((i * 3))" >> $SAMPLE_INPUT
done

# Print the hash table and exit
echo "print" >> $SAMPLE_INPUT
echo "exit" >> $SAMPLE_INPUT
