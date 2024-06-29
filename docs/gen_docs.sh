if [ ! -d doxygen ];
then 
wget https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/doxygen-1.11.0.linux.bin.tar.gz
gunzip doxygen-1.11.0.linux.bin.tar.gz
tar xf doxygen-1.11.0.linux.bin.tar
rm -rf doxygen-1.11.0.linux.bin.tar
mv doxygen-1.11.0 doxygen
fi

export PATH=$PATH:$(cd doxygen/bin && pwd)

cd .. && doxygen docs/doxygen.config
