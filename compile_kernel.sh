set -x
LLVM=1

export LLVM=1
make defconfig

make LLVM=1 -j 12   # check your $nproc
make modules
make modules_install
make install

