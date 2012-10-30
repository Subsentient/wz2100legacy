git pull
rm -r ./wzlegacy_build
cp -R ./wzlegacy ./wzlegacy_build
cd ./wzlegacy_build
./configure CFLAGS="-march=native -mtune=native -O3" CXXFLAGS="-march=native -mtune=native -O3"
make -j4
sudo make install
