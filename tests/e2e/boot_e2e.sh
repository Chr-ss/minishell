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

VAGRANT_ZIP=vagrant.zip
VAGRANT=vagrant
VAGRANT_DIR=linux_install
VAGRANT_IMAGE=generic/ubuntu2204

#Download vagrant
if [ ! -f $VAGRANT ];
then
	echo -ne 'Downloading vagrant  : #                         (0%) (this might take a few minutes)\r'
	wget https://releases.hashicorp.com/vagrant/2.4.1/vagrant_2.4.1_linux_amd64.zip --inet4-only --no-check-certificate -O $VAGRANT_ZIP 
	echo -ne 'Downloading vagrant      : #######################   (100%)\r'
	echo
	unzip $VAGRANT_ZIP
	rm -rf $VAGRANT_ZIP
fi

#Add vagrant to path
export PATH=$PATH:$(pwd)

#create vagrant directory
mkdir -p $VAGRANT_DIR

#cd into vagrant directory
cd $VAGRANT_DIR

#prepare vagrant directory with needed files
mkdir -p shared_resources && echo > Vagrantfile

#add ubuntu image
vagrant box add $VAGRANT_IMAGE --provider virtualbox --force

#initialize vagrant
vagrant init $VAGRANT_IMAGE --force

#add plugin for guest additions
vagrant plugin install vagrant-vbguest

#copy vagrantfile over
cp ../util/Vagrantfile ./

#up virtual machine
vagrant up --provider=virtualbox
