obj-m += kernel_padwan.o

all:
	echo "Compilando modulo Kernel Padwan"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
