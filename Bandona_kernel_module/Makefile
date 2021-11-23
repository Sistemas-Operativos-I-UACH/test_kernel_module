bj-m += bandona_module.o

all:
	echo "Compilando modulo bandona"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
