# syntax: ./build.sh microwave_rebase
# error code:
# 0 - normal
# 1 - compilation error
# 2 - cannot access files
# 3 - invalid git branch

abort() {
	code=$1
	git checkout utils
	
	case $code in
		1) echo "[ERROR] Compilation error; Aborting operation." ;;
		2) echo "[ERROR] Cannot access files; Aborting operation." ;;
		3) echo "[ERROR] Invalid git branch; Aborting operation." ;;
	esac
	
	exit $code
}

# check parameters
branch="$1"
if [ "$branch" == "" ]; then
	echo "No git branch was specified; changing to default, 'microwave_rebase'."
	branch="microwave_rebase"
fi

# git stuff
git checkout "$branch" || abort 3
git pull

# rm old working files and cp over
rm -rf ./wzlegacy_build
cp -R ./wzlegacy ./wzlegacy_build || abort 2
cd ./wzlegacy_build

./autogen.sh || abort 1
./configure CFLAGS="-march=native -mtune=native -O3" CXXFLAGS="-march=native -mtune=native -O3" || abort 1
make -j4 || abort 1
sudo make install || abort 1

# cleanup
cd ..
rm -rf ./wzlegacy_build
git checkout utils
