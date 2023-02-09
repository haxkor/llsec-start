KERNEL="$1"
IMG=/home/debian/linux/guest.img  # path to your filesystem

#-enable-kvm   # add this flag to the command below (sudo required)  + change cpu to "host"

qemu-system-x86_64 \
        -cpu qemu64 \
        -m 2G \
        -nographic \
        -hda $IMG \ 
        -kernel $KERNEL -append "root=/dev/sda rw console=ttyS0 pti=off nokaslr" \
        -net nic -net user,hostfwd=tcp::22222-:22 \     # forward port 22 to 22222 to enable ssh
        -smp cores=16,threads=1,sockets=2 \


