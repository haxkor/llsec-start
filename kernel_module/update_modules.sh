MYMODULE=$1

# mount your image
mount /home/debian/linux/guest.img /mnt/guest/

# copy the given file to the / of your image
cp $MYMODULE /mnt/guest/

umount /mnt/guest

