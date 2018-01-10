#### TALLER 8

# compilo con make -f Taller8.mk


*.pdf : *.dat graficas.py
	python graficas.py

*.dat : advection.x 
	./advection.x 

advection.x : advection.c
	cc advection.c -o advection.x -lm

clean :
	rm *.dat  *.x *.png *.pdf 
