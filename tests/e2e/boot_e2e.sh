#!/bin/bash
# Reference: http://stdioe.blogspot.com/2012/01/creating-virtual-machine-with.html
# Reference: https://gist.githubusercontent.com/seandenigris/5039427/raw/dde96014afcbd3c489c52d079a2c607762601e9a/install_debian.sh
# Reference: https://dustinspecker.com/posts/ubuntu-autoinstallation-virtualbox/
# Reference: https://askubuntu.com/questions/1473018/installing-packages-via-autoinstall-vs-user-data
# Reference: https://www.pugetsystems.com/labs/hpc/ubuntu-22-04-server-autoinstall-iso/
# Reference: https://superuser.com/questions/180744/how-do-i-extract-an-iso-on-linux-without-root-access
# Reference: https://www.baeldung.com/linux/path-variable
# Reference: https://stackoverflow.com/questions/592620/how-can-i-check-if-a-program-exists-from-a-bash-script

XORISSO_EXE=xorriso-1.5.6/xorriso/xorriso
XORISSO_TAR=xorriso-1.5.6.pl02.tar.gz
XORISSO_DIR=xorriso-1.5.6
MACHINENAME="TestMachine"
UBUNTU_CD_IMAGE="ubuntu.iso"
TEST_IMAGE=test.iso

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
if [ ! -f ./$UBUNTU_CD_IMAGE ]; then
    echo -ne 'Downloading ubuntu image  : #                         (0%) (this might take a few minutes)\r'
    wget https://releases.ubuntu.com/jammy/ubuntu-22.04.4-live-server-amd64.iso -O $UBUNTU_CD_IMAGE 
    echo -ne 'Downloading complete      : #######################   (100%)\r'
fi

# Extract iso image 
if [ ! -f ./$TEST_IMAGE ]; then
    xorriso -osirrox on -indev $UBUNTU_CD_IMAGE -extract $TEST_IMAGE 
fi

# #Create VM
# VBoxManage createvm --name $MACHINENAME --ostype "Ubuntu22_LTS_64" --register --basefolder `pwd`
# #Set memory and network
# VBoxManage modifyvm $MACHINENAME --ioapic on
# VBoxManage modifyvm $MACHINENAME --memory 1024 --vram 128
# VBoxManage modifyvm $MACHINENAME --nic1 nat
# #Create Disk and connect Debian Iso
# VBoxManage createhd --filename `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi --size 80000 --format VDI
# VBoxManage storagectl $MACHINENAME --name "SATA Controller" --add sata --controller IntelAhci
# VBoxManage storageattach $MACHINENAME --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium  `pwd`/$MACHINENAME/$MACHINENAME_DISK.vdi
# VBoxManage storagectl $MACHINENAME --name "IDE Controller" --add ide --controller PIIX4
# VBoxManage storageattach $MACHINENAME --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium `pwd`/$UBUNTU_CD_IMAGE
# VBoxManage modifyvm $MACHINENAME --boot1 dvd --boot2 disk --boot3 none --boot4 none

# #Enable RDP
# VBoxManage modifyvm $MACHINENAME --vrde on
# VBoxManage modifyvm $MACHINENAME --vrdemulticon on --vrdeport 10001

# #Start the VM
# VBoxHeadless --startvm $MACHINENAME