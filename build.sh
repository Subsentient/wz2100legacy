# syntax: ./build.sh microwave_rebase
# error code:
# 0 - normal
# 1 - compilation error
# 2 - cannot access files

# git stuff
git checkout $1
git pull

# rm old working files and cp over
rm -rf ./wzlegacy_build
cp -R ./wzlegacy ./wzlegacy_build || exit 2
cd ./wzlegacy_build

./autogen.sh || exit 1
./configure CFLAGS="-march=native -mtune=native -O3" CXXFLAGS="-march=native -mtune=native -O3" || exit 1
make -j4 || exit 1
sudo make install || exit 1

# cleanup
cd ..
rm -rf ./wzlegacy_build
