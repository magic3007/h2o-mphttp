set terminal png transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
set output 'plot/static_cond_20ms.png'
set style increment default
set style data lines
set xlabel 'time elapsed (ms)'
set ylabel 'byte range (Kb)'
plot [0:1000][0:2100] "0.dat" title '0' with points, "1.dat" title '1' with points, "2.dat" title '2' with points
