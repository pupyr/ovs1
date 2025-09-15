gen:
	g++  generate_images.cpp -o gen.exe
	./gen.exe

learn:
	g++  learning.cpp -o learn.exe
	./learn.exe

clear:
	rm imgs/*/*
	rm *.exe