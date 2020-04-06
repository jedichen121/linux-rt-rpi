#!/bin/bash
#sudo mount /dev/sdc1 mnt/fat32
#sudo mount /dev/sdc2 mnt/ext4
sudo mount /dev/mmcblk0p1 mnt/fat32/
sudo mount /dev/mmcblk0p2 mnt/ext4/
sudo env PATH=$PATH make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=mnt/ext4 modules_install
KERNEL=kernel7
sudo cp arch/arm/boot/zImage mnt/fat32/$KERNEL-tt.img
sudo cp arch/arm/boot/dts/*.dtb mnt/fat32/
sudo cp arch/arm/boot/dts/overlays/*.dtb* mnt/fat32/overlays/
sudo cp arch/arm/boot/dts/overlays/README mnt/fat32/overlays/
sudo umount mnt/fat32
sudo umount mnt/ext4
echo "finished"
