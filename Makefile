
build:
	g++ -I/opt/local/include *.cpp -lfftw3 -lm

graph:
	./a.out > output.log
	gnuplot graph.plot
	imgcat out.png
