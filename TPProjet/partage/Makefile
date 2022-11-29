all: tunnel64d

tunnel64d: tunnel64d.c extremite.c iftun.c traitement.c
	gcc tunnel64d.c iftun.c extremite.c traitement.c -o ../partage/tunnel64d
	#gcc -g tunnel64d.c extremite.c iftun.c -o tunnel64d

clean:
	rm ../partage/tunnel64d
	#rm tunnel46d