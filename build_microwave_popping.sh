git pull
rm -rf ./wzlegacy_build
cp -R ./wzlegacy ./wzlegacy_build
cd ./wzlegacy_build
./autogen.sh
./configure CFLAGS="-march=native -mtune=native -O3" CXXFLAGS="-march=native -mtune=native -O3"
make -j4
sudo make install
cd ..
rm -rf ./wzlegacy_build
