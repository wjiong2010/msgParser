all: llp_main.o llp_parser.o debug.o
	gcc -o llp_main $^
	
llp_main.o: llp_main.c
	gcc -c -o $@ $<
	
llp_parser.o: llp_parser.c
	gcc -c -o $@ $<
	
debug.o: debug.c
	gcc -c -o $@ $<

clean:
	rm -rf *.o *.exe