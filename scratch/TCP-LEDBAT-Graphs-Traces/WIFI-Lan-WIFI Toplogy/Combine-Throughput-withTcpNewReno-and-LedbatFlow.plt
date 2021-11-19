set terminal png
set output "Combine-Throughput-Flow.png"
set title "Throghput of all 4 Flows"
set xlabel "TimeStamps"
set ylabel "Throughput"

set xrange [0:45.000000]
set yrange [0:4]
plot "Combine-Throughput-Flow-data.txt" using 1:2 title "Flow 1" with lines lw 2, "Combine-Throughput-Flow-data.txt" using 1:3 title "Flow-2" with lines lw 2, "Combine-Throughput-Flow-data.txt" using 1:4 title "Flow-3" with lines lw 2, "Combine-Throughput-Flow-data.txt" using 1:5 title "Flow-4" with lines lw 2
