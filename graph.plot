set terminal png size 1024,768
set output 'out.png'
#set datafile separator "\t"
set pm3d map
#set zrange [0.000001:100]
set logscale z
set logscale x
splot "output.log" matrix with pm3d
