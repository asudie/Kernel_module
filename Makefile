# obj-m += my_module.o
# obj-m += future_jiffies_patch.o
obj-m += future_data_patch.o
# CFLAGS_my_module.o := -DDEBUG
CFLAGS_future_data_patch.o := -DDEBUG
# CFLAGS_future_jiffies_patch.o := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean; rm read_test
test:
	gcc -g read_test.c -o read_test; ./read_test