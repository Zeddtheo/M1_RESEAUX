all: tunnel64d

tunnel64d: tunnel64d.c extremite.c iftun.c
	gcc tunnel64d.c iftun.c extremite.c -o ../partage/tunnel64d
	#gcc -g tunnel46d.c extremite.c iftun.c -o tunnel64d

clean:
	rm ../partage/tunnel64d
	#rm tunnel64d