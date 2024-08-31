#!/usr/bin/env bash

#
# Ref: https://www.osadl.org/Create-a-latency-plot-from-cyclictest-hi.bash-script-for-latency-plot.0.html
#

# 1. Run the 'cyclictest' on all cores
sudo cyclictest --mlockall --smp --priority=80 --interval=1000 --duration=5s --histogram=400 > cyclictest-out

# 2. Get maximum latency
max=`grep "Max Latencies" cyclictest-out | tr " " "\n" | sort -n | tail -1 | sed s/^0*//`

# 3. Grep data lines, remove empty lines and create a common field separator
grep -v -e "^#" -e "^$" cyclictest-out | tr " " "\t" >histogram 

# 4. Set the number of cores
cores=4

# 5. Create two-column data sets with latency classes and frequency values for each core
for i in `seq 1 $cores`
do
  column=`expr $i + 1`
  cut -f1,$column histogram >histogram$i
done

# 6. Create plot command header
echo -n -e "set title \"Latency plot\"\n\
set terminal png\n\
set xlabel \"Latency (us), max $max us\"\n\
set logscale y\n\
set xrange [0:400]\n\
set yrange [0.8:*]\n\
set ylabel \"Number of latency samples\"\n\
set output \"plot.png\"\n\
plot " >plotcmd

# 7. Append plot command data references 
for i in `seq 1 $cores`
do
  if test $i != 1
  then
    echo -n ", " >>plotcmd
  fi
  cpuno=`expr $i - 1`
  if test $cpuno -lt 10
  then
    title=" CPU$cpuno"
   else
    title="CPU$cpuno"
  fi
  echo -n "\"histogram$i\" using 1:2 title \"$title\" with histeps" >>plotcmd
done

# 8. Execute plot command
gnuplot -persist <plotcmd

# 9. Plot is ready
file plot.png
