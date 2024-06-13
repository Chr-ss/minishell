#!/bin/bash
# Reference: http://stdioe.blogspot.com/2012/01/creating-virtual-machine-with.html
# Reference: https://andreafortuna.org/2019/10/24/how-to-create-a-virtualbox-vm-from-command-line/
# Reference: https://gist.githubusercontent.com/seandenigris/5039427/raw/dde96014afcbd3c489c52d079a2c607762601e9a/install_debian.sh
# Reference: https://dustinspecker.com/posts/ubuntu-autoinstallation-virtualbox/
# Reference: https://askubuntu.com/questions/1473018/installing-packages-via-autoinstall-vs-user-data
# Reference: https://www.pugetsystems.com/labs/hpc/ubuntu-22-04-server-autoinstall-iso/
# Reference: https://superuser.com/questions/180744/how-do-i-extract-an-iso-on-linux-without-root-access
# Reference: https://www.baeldung.com/linux/path-variable
# Reference: https://stackoverflow.com/questions/592620/how-can-i-check-if-a-program-exists-from-a-bash-script
# Reference: https://stackoverflow.com/questions/51585564/delete-files-extracted-with-xorriso
# Reference: https://stackoverflow.com/questions/2953081/how-can-i-write-a-heredoc-to-a-file-in-bash-script
# Reference: https://stackoverflow.com/questions/55195182/bash-run-script-from-here-doc
# Reference: https://www.youtube.com/watch?v=DtXZ6BMaKbA
# Reference: https://stackoverflow.com/questions/13438095/replace-the-first-line-in-a-text-file-by-a-string
# Reference: https://stackoverflow.com/questions/94074/slow-wget-speeds-when-connecting-to-https-pages
# Reference: https://stackoverflow.com/questions/5613345/how-to-shrink-the-git-folder
# Reference: https://www.virtualbox.org/manual/ch01.html

XORISSO_EXE=xorriso-1.5.6/xorriso/xorriso
XORISSO_TAR=xorriso-1.5.6.pl02.tar.gz
XORISSO_DIR=xorriso-1.5.6
MACHINENAME="TestMachine"
UBUNTU_CD_IMAGE="ubuntu.iso"
TEST_CD_IMAGE="ubuntu-22.04.2-test-autoinstall.iso"
TEST_IMAGE=sources-files
TEST_IMAGE_USR=server
TEST_IMAGE_BTT=bootpart
USER_DATA=user-data

if ! command -v VBoxManage &> /dev/null
then
    echo "VBoxManage could not be found, this is needed to create the VM"
    exit 1
fi

# Download xorriso
if [ ! -f ./$XORISSO_EXE ]; then
    echo -ne 'Downloading xorriso       : #                         (0%)\r'
    wget https://www.gnu.org/software/xorriso/xorriso-1.5.6.pl02.tar.gz -O $XORISSO_TAR &> /dev/null
    echo -ne 'Unpacking xorriso         : #####                     (33%)\r'
    tar zxvf $XORISSO_TAR &> /dev/null
    echo -ne 'Building xorriso          : #############             (66%)\r'
    (cd $XORISSO_DIR && ./configure && make) &> /dev/null
    echo -ne 'installing xorisso        : ######################    (95%)\r' 
    rm -rf $XORISSO_TAR &> /dev/null
    echo -ne 'xorisso installed         : #######################   (100%)\r' 
    echo -ne '\n'
fi 

# Add xorriso to path 
if [ -f ./$XORISSO_EXE ]; then
XORISSO_PATH=$(cd $XORISSO_DIR && cd xorriso && pwd)
export PATH=$PATH:$XORISSO_PATH
fi 

# Download debian.iso
# if [ ! -f ./$UBUNTU_CD_IMAGE ]; then
#     echo -ne 'Downloading ubuntu image  : #                         (0%) (this might take a few minutes)\r'
#     wget https://releases.ubuntu.com/jammy/ubuntu-22.04.4-live-server-amd64.iso --inet4-only --no-check-certificate -O $UBUNTU_CD_IMAGE 
#     echo -ne 'Downloading complete      : #######################   (100%)\r'
# fi

# Extract iso image 
if [ ! -d ./$TEST_IMAGE ]; then
    mkdir -p $TEST_IMAGE/$TEST_IMAGE_BTT
    xorriso -osirrox on -indev $UBUNTU_CD_IMAGE --extract_boot_images $TEST_IMAGE/$TEST_IMAGE_BTT -extract / $TEST_IMAGE 
    chmod -R u+w $TEST_IMAGE
fi

#create folder + user-data / meta-data for autoinstall
(cd $TEST_IMAGE && mkdir -p $TEST_IMAGE_USR && cd $TEST_IMAGE_USR && cp $(find ../../../ -type d -name util)/$USER_DATA ./ && echo > meta-data)

# find files
GRUB_FILE=$(find ./ -type f -name grub.cfg)
GRUB2_MBR=$(find ./sources-files -type f -name "mbr_code_grub2.img")
EFI=$(find ./ -type f -name "gpt_part2_efi.img")

#change grub boot
cat << "EOF" > $(find ./ -type f -name grub.cfg)
set timeout=1

loadfont unicode

set menu_color_normal=white/black
set menu_color_highlight=black/light-gray

menuentry "Autoinstall Ubuntu Server" {
set gfxpayload=keep
linux /casper/vmlinuz quiet autoinstall ds=nocloud\;s=/cdrom/server/ ---
initrd /casper/initrd
}
menuentry "Try or Install Ubuntu Server" {
	set gfxpayload=keep
	linux	/casper/vmlinuz  ---
	initrd	/casper/initrd
}
menuentry "Ubuntu Server with the HWE kernel" {
	set gfxpayload=keep
	linux	/casper/hwe-vmlinuz  ---
	initrd	/casper/hwe-initrd
}
grub_platform
if [ "$grub_platform" = "efi" ]; then
menuentry 'Boot from next volume' {
	exit 1
}
menuentry 'UEFI Firmware Settings' {
	fwsetup
}
else
menuentry 'Test memory' {
	linux16 /boot/memtest86+.bin
}
fi
EOF

# remove ubuntu image for diskspace
rm -rf $UBUNTU_CD_IMAGE

# Make new iso file for test vm
if [ ! -f ./$TEST_CD_IMAGE ]; then
(cd $TEST_IMAGE && cat <<EOF | bash)
xorriso -as mkisofs -r \
-V 'Ubuntu 22.04.2 LTS amd64' \
-o ../$TEST_CD_IMAGE \
--grub2-mbr ./bootpart/mbr_code_grub2.img \
-partition_offset 16 \
--mbr-force-bootable \
-append_partition 2 28732ac11ff8d211ba4b00a0c93ec93b ./bootpart/gpt_part2_efi.img \
-appended_part_as_gpt \
-iso_mbr_part_type a2a0d0ebe5b9334487c068b6b72699c7 -c '/boot.catalog' \
-b '/boot/grub/i386-pc/eltorito.img' \
-no-emul-boot \
-boot-load-size 4 \
-boot-info-table \
--grub2-boot-info \
-eltorito-alt-boot \
-e '--interval:appended_partition_2_start_971297s_size_10068d:all::' \
-no-emul-boot .
EOF
fi

rm -rf $TEST_IMAGE

#Create VM
VBoxManage createvm --name $MACHINENAME --ostype "Ubuntu22_LTS_64" --register --basefolder `pwd`
#Set memory and network
VBoxManage modifyvm $MACHINENAME --ioapic on
VBoxManage modifyvm $MACHINENAME --memory 2048 --vram 128
VBoxManage modifyvm $MACHINENAME --nic1 nat
#Create Disk and connect Debian Iso
VBoxManage createhd --filename `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi --size 3000 --format VDI
VBoxManage storagectl $MACHINENAME --name "SATA Controller" --add sata --controller IntelAhci
VBoxManage storageattach $MACHINENAME --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium  `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi
VBoxManage storagectl $MACHINENAME --name "IDE Controller" --add ide --controller PIIX4
VBoxManage storageattach $MACHINENAME --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium `pwd`/$TEST_CD_IMAGE
VBoxManage modifyvm $MACHINENAME --boot1 dvd --boot2 disk --boot3 none --boot4 none

#Enable RDP
VBoxManage modifyvm $MACHINENAME --vrde on
VBoxManage modifyvm $MACHINENAME --vrdemulticon on --vrdeport 10001

#Start the VM
VBoxManage startvm $MACHINENAME

# VBoxHeadless --startvm $MACHINENAME