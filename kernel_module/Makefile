obj-m += mymodule.o
# /lib/modules/$(shell uname -r)/build   # this is the original -C parameter. 

LINUX_SRC=/home/debian/linux  # We exchange it for the src dir of the kernel in which we want to run the module

all:
        make -C $LINUX_SRC M=$(PWD) modules
clean:
        make -C $LINUX_SRC M=$(PWD) clean
