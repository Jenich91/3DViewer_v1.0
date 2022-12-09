.PHONY: all build install uninstall clean dvi dist lib test gcov_report

CC = gcc
# CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
OS = $(shell uname)
ifeq ($(OS),Darwin)
	CHECKFLAGS = -lcheck
else
	CHECKFLAGS = -lcheck -lpthread -lrt -lm -lsubunit
endif

all: clean build

build:
	mkdir ./build/; cd ./build/; qmake ../; make; mv GLWidgets.app 3DViewer.app

install: uninstall build
	mkdir $(HOME)/Desktop/3DViewer/
	cp -rf ./build/3DViewer.app $(HOME)/Desktop/3DViewer/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/3DViewer/

dvi:
	makeinfo --html Documentation.texi
	open ./Documentation/index.html

dist:
	@ rm -rf archive3DViewer
	mkdir archive3DViewer; mkdir archive3DViewer/src
	cp *.c *.h *.cpp *.ui *.pro *.pro.user Makefile Documentation.texi archive3DViewer/src
	tar -cvzf archive3DViewer.tar.gz archive3DViewer/
	# mv archive3DViewer.tar.gz $(HOME)/Desktop/
	@ rm -rf archive3DViewer/

lib: 
	$(CC) $(CFLAGS) -c 3DViewer.c -o 3DViewer.o
	ar rc 3DViewer.a 3DViewer.o
	ranlib 3DViewer.a

test: lib
	$(CC) $(CFLAGS) test.c 3DViewer.a $(CHECKFLAGS) -o a.out
	./a.out

gcov_report: lib
	$(CC) $(CFLAGS) $(GCOVFLAGS) 3DViewer.c test.c $(CHECKFLAGS) -o gcov_rep.out
	./gcov_rep.out
	lcov -t "gcov_rep" --output-file=coverag.info --capture --directory .
	genhtml -o ./report coverag.info
	open ./report/index.html

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out
	rm -rf ./report
	rm -rf ./build
	rm -rf ./Documentation
	rm -rf  $(HOME)/Desktop/archiveSmartCalc_v1.tar.gz
	rm -rf archive3DViewer
	rm -rf archive3DViewer.tar.gz

