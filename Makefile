obj-m += my_module.o
CFLAGS_my_module.o := -DDEBUG
# obj-m += future_jiffies_patch.o
# obj-m += future_data_patch.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean; rm read_test
test:
	gcc read_test.c -o read_test; ./read_test