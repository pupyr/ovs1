gen:
	g++  generate_images.cpp -o gen.exe
	./gen.exe

learn:
	g++  learning.cpp -o learn.exe -Dlay1
	./learn.exe

learn2:
	clear
	g++  learning.cpp -o learn.exe -Dlay2
	./learn.exe

clear:
	rm imgs/*/*
	rm *.exe