set terminal png size 900,400
set title "plot title"
set ylabel "unit"
set xlabel "dt"
set xdata time
set datafile separator ","

set timefmt '%Y-%m-%dT%H:%M:%S'
set format x '%Y-%m-%dT%H:%M:%S'

set xtics rotate
set grid
plot '<cat' using 1:2 with lines
