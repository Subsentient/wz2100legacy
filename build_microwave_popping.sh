git pull
rm -r ./wzlegacy_build
cp -R ./wzlegacy ./wzlegacy_build
cd ./wzlegacy_build
./configure
make -j4
sudo make install
