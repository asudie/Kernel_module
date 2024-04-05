# Kernel_module

Source code is in src, patches are in 01 and 02.

All tasks are completed on a real machine.

## Task 1

> cd src

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

### How to enable debugfs

*To enable it for onetime, i.e information will be available until next boot of system.*

> mount -t debugfs none /sys/kernel/debug

### Checking files with checkpatch.pl

![style](https://github.com/asudie/Kernel_module/assets/80539245/4489d521-e6eb-4626-a375-2a25136eb4ad)

### Checking that jiffies work with printk

![jiffies_works](https://github.com/asudie/Kernel_module/assets/80539245/f9334684-ac60-4ad4-aeac-c1fb1581691f)

### Checking that data work with printk

![data_working](https://github.com/asudie/Kernel_module/assets/80539245/eadd081c-c9de-4020-88e1-6b2c59c79df3)







