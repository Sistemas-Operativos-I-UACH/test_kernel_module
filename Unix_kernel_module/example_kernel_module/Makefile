obj-m += pepito_module.o

all:
	echo "Compilando modulo pepito"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
