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
# Reference: https://askubuntu.com/questions/161759/how-to-access-a-shared-folder-in-virtualbox
# Reference: https://www.jimangel.io/posts/automate-ubuntu-22-04-lts-bare-metal/
# Reference: https://blog.learncodeonline.in/automating-linux-installation-using-vagrant-and-virtualbox
# Reference: https://askubuntu.com/questions/672892/what-does-y-mean-in-apt-get-y-install-commands
# Reference: https://stackoverflow.com/questions/73397110/how-to-stop-ubuntu-pop-up-daemons-using-outdated-libraries-when-using-apt-to-i
VAGRANT_ZIP=vagrant.zip
VAGRANT=vagrant
VAGRANT_DIR=linux_install
VAGRANT_IMAGE=generic/ubuntu2204
VAGRANT_SHARED=shared_resources
PROJECT=$(find ../../../ -type d -name minishell)

echo -ne 'Creating VM directory  : #                         (0%) \r'
mkdir -p $VAGRANT_DIR/$VAGRANT_SHARED
echo -ne 'Creating VM directory  : #######################   (100%) \r'
echo
echo -ne 'Copying over project   : #                         (0%) \r'
rsync -av --exclude=*/.git* --exclude=*/vagrant* --exclude=*/boot_e2e* --exclude=*/$VAGRANT_DIR* $PROJECT $VAGRANT_DIR/$VAGRANT_SHARED &> /dev/null
echo -ne 'Copying over project   : #######################   (100%) \r'
echo

#Download vagrant
if [ ! -f $VAGRANT ];
then
	echo -ne 'Downloading vagrant  : #                         (0%) (this might take a few minutes)\r'
	wget https://releases.hashicorp.com/vagrant/2.4.1/vagrant_2.4.1_linux_amd64.zip --inet4-only --no-check-certificate -O $VAGRANT_ZIP &> /dev/null
	echo -ne 'Downloading vagrant  : #######################   (100%)\r'
	echo
	echo 'unzipping vagrant    : #                         (0%)'
	unzip $VAGRANT_ZIP
	echo 'unzipping vagrant    : #######################   (100%)'
	echo
	rm -rf $VAGRANT_ZIP
fi

#Add vagrant to path
export PATH=$PATH:$(pwd)

#create vagrant directory
echo -ne 'Preparing VM dir       : #                         (0%) \r'
mkdir -p $VAGRANT_DIR

#cd into vagrant directory
echo -ne 'Preparing VM dir       : ##############            (50%) \r'
cd $VAGRANT_DIR

#prepare vagrant directory with Vagrantfile
echo -ne 'Preparing VM dir       : #######################   (100%) \r'
echo 
echo > Vagrantfile

#removing old boxes and vm's
echo 'Removing old boxes/vms : #                         (0%)'
echo 
VBoxManage list runningvms | awk '{print $2}'  | xargs --no-run-if-empty -t -n1 -IXXX VBoxManage controlvm XXX poweroff                                                           
VBoxManage list vms | awk '{print $2}'  | xargs --no-run-if-empty -t -n1 VBoxManage unregistervm                                                                                  
killall -9 VBoxHeadless                                                                                                                                                           
rm -rf ~/Virtualbox\ VMs/* 
vagrant box remove $VAGRANT_IMAGE
# PREVIOUS_BOX=$(vagrant global-status --prune | awk 'FNR == 3 {print $1}')
# vagrant destroy $PREVIOUS_BOX --force
echo 'Removing old boxes/vms : #######################   (100%)'
echo 

#add ubuntu image
echo 'Launching Vagrant      : #                         (0%)'
vagrant box add $VAGRANT_IMAGE --force --provider virtualbox 

#initialize vagrant
echo 'Launching Vagrant      : #######                  (25%)'
vagrant init $VAGRANT_IMAGE --force

#add plugin for guest additions
echo 'Launching Vagrant      : ##############           (50%)'
vagrant plugin install vagrant-vbguest

#copy vagrantfile over
echo 'Launching Vagrant      : ####################     (75%)'
cat ../util/Vagrantfile > ./Vagrantfile

#up virtual machine
echo 'Launching Vagrant      : #########################(100%)'
vagrant up --provider=virtualbox
vagrant ssh
vagrant halt
