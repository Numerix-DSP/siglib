# Plot a 3D Spectrogram

set pm3d at s flush begin noftriangles nohidden3d transparent implicit corners2color mean

set ticslevel 0					            # Spectrogram
set view 17, 5, 1, 2			          # Side view
#set view 0, 0, 1, 2				        # Overhead
set title "3D Spectrogram Plot"

set xlabel "Time (s)"			          # label x-axis with offsets
set ylabel "Frequency (KHz)"	      # label y-axis with offsets
set zlabel "dB"					            # 0, -80		# label x-axis with offsets

#set palette rgbformulae 34,35,36

set palette model RGB
set palette defined ( 0 "black", 1 "blue", 2 "red", 3 "yellow", 4 "white" )
#set palette defined ( 0 "black", 1 "black", 2 "blue", 3 "red", 4 "yellow", 5 "white" )

splot 'sc.gpdt' with pm3d

pause -1 "Hit return to continue"
reset

