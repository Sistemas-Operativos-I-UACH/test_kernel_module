obj-m += pedro_module.o

all:
	echo "Compilando modulo pedro"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
