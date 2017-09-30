
build:
	g++ -I/opt/local/include *.cpp -O3

graph:
	./a.out > output.log
	gnuplot graph.plot
	imgcat out.png
