# Kernel_module

## Task 1

### 1. To compile do
> make

### 2. To load do
> sudo insmod my_module.ko

### 3. To unload do
> sudo rmmod my_module.ko

### 4. To see my_module among modules
> sudo lsmod

### 5. To see output in logs
> sudo dmesg | tail

![Снимок экрана от 2024-03-27 11-45-35](https://github.com/asudie/Kernel_module/assets/80539245/3e97470a-3262-468d-9fef-50224a9a2af7)

## Task 2

### How to enable debugfs :Permalink

To enable it for onetime, i.e information will be available until next boot of system.

> mount -t debugfs none /sys/kernel/debug

To make the change permanent, add following line to /etc/fstab file.

> debugfs    /sys/kernel/debug      debugfs  defaults  0 0
