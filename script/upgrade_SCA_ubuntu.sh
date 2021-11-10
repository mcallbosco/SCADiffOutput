# remove SCA directory
# git clone must be cloned to an empty directory
# pipe yes to remove command if it asks about removing files
yes | rm -r ~/SCA

# create empty SCA directory
mkdir ~/SCA

# clone our SCA repo into directory ~/SCA
git clone https://github.com/alihaider1264/SCA.git ~/SCA

# remove script directory from git clone
# pipe yes to remove command if it asks about removing files
yes | rm -r ~/SCA/script

# make directory /out and /output if they don't already exist
cd ~/SCA/SCA/core
mkdir -p out
cd ~/SCA/SCA/user
mkdir -p output