#!/bin/bash
# Reference: http://stdioe.blogspot.com/2012/01/creating-virtual-machine-with.html
# Reference: https://gist.githubusercontent.com/seandenigris/5039427/raw/dde96014afcbd3c489c52d079a2c607762601e9a/install_debian.sh
# https://dustinspecker.com/posts/ubuntu-autoinstallation-virtualbox/
# https://askubuntu.com/questions/1473018/installing-packages-via-autoinstall-vs-user-data
# https://www.pugetsystems.com/labs/hpc/ubuntu-22-04-server-autoinstall-iso/
# https://superuser.com/questions/180744/how-do-i-extract-an-iso-on-linux-without-root-access

MACHINENAME="TestMachine"
UBUNTU_CD_IMAGE="ubuntu.iso"

# Download xorriso
# https://www.gnu.org/software/xorriso/xorriso-1.5.6.pl02.tar.gz

# Download debian.iso
if [ ! -f ./$UBUNTU_CD_IMAGE ]; then
    wget https://releases.ubuntu.com/jammy/ubuntu-22.04.4-live-server-amd64.iso -O $UBUNTU_CD_IMAGE
fi

#Create VM
VBoxManage createvm --name $MACHINENAME --ostype "Ubuntu22_LTS_64" --register --basefolder `pwd`
#Set memory and network
VBoxManage modifyvm $MACHINENAME --ioapic on
VBoxManage modifyvm $MACHINENAME --memory 1024 --vram 128
VBoxManage modifyvm $MACHINENAME --nic1 nat
#Create Disk and connect Debian Iso
VBoxManage createhd --filename `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi --size 80000 --format VDI
VBoxManage storagectl $MACHINENAME --name "SATA Controller" --add sata --controller IntelAhci
VBoxManage storageattach $MACHINENAME --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium  `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi
VBoxManage storagectl $MACHINENAME --name "IDE Controller" --add ide --controller PIIX4
VBoxManage storageattach $MACHINENAME --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium `pwd`/$UBUNTU_CD_IMAGE
VBoxManage modifyvm $MACHINENAME --boot1 dvd --boot2 disk --boot3 none --boot4 none

#Enable RDP
VBoxManage modifyvm $MACHINENAME --vrde on
VBoxManage modifyvm $MACHINENAME --vrdemulticon on --vrdeport 10001

#Start the VM
VBoxHeadless --startvm $MACHINENAME