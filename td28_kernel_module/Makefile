obj-m += td28_module.o

all:
	echo "Compilando modulo tamales de la 28"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
