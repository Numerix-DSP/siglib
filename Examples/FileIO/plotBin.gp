
# set title "Tone Level Plots" font ",20"
print "-"

clear
set multiplot layout 2, 1

set style line 1 lt 1 lc rgb '#B8E720' # light green
set style line 2 lt 1 lc rgb '#1399DB' # dark blue
set style line 3 lt 1 lc rgb '#99DA07' # dark green
set style line 4 lt 1 lc rgb '#44BBF1' # light blue
set style line 5 lt 1 lc rgb '#464749' # black
set style line 6 lt 1 lc rgb '#D1D3D4' # grey

set autoscale  yfix
set style data points
set xlabel "Time (sec)"
set ylabel "Amplitude"

set grid x y
set tics out nomirror
set mxtics 4
set mytics 2
# set xrange [0:48000]            # Plot 3 seconds
# set key left box
set key off          # No key

plot "Tone_10_30dB.dat" using 1:2 with lines ls 1, "Tone_30_50dB.dat" using 1:2 with lines ls 2


set autoscale  yfix
set style data points
set xlabel "Time (sec)"
set ylabel "power (dBm)"

set grid x y
set tics out nomirror
set mxtics 4
set mytics 2
# set xrange [0:48000]            # Plot 3 seconds
# set key left box
set key off          # No key

plot "Tone_10_30dB_power.dat" using 1:2 with lines ls 1, "Tone_30_50dB_power.dat" using 1:2 with lines ls 2

print "Hit <CR> to continue ..."
pause -1

