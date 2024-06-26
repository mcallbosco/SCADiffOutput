# SCA Description and Installation

The SCA tool is designed for programmers beginning their journey in the computer science world. The goal
of our tool is to provide the user with helpful suggestions instead of unhelpful and ofen confusing errors
and warnings generated by compilers. The suggestions are designed to guide the user by promoting more 
reliable coding practices and "pushing" them in the right direction. We hope you find this tool easy 
to use and also beneficial in your coding endeavors.

Instructions on installing and using SCA tool:

Prerequisites => Must be running a Debian version of Linux 
                 (if you don't have a debian version of Linux you can always spin up a Virtual Machine).

1. Open a new terminal window
2. Download install script with command: wget https://raw.githubusercontent.com/mcallbosco/SCADiffOutput/main/script/install_SCA_ubuntu.sh
3. Give install script permission to execute: sudo chmod +x install_SCA_ubuntu.sh
4. Run the install script: ./install_SCA_ubuntu.sh
5. After the install script has successfully run, close the current terminal window. The install script should download and install all necessary requirements for
   SCA to run. Contents of SCA will be stored in a directory that can be found at ~/SCA (on your home directory). A picture of the directory
   structure can be found below.
6. To execute the SCA tool on your source code files, place the .cpp files you would like to have analyzed
   in the ~/SCA/SCA/user/source-code directory (see photo below)
7. Once you have your desired .cpp files in /source-code open a new terminal and enter the command: sca

   This command will run SCA on the files placed in /source-code. After sca is complete a new file explorer
   should open at the location of the resulting .html files. If it does not, you can view your results in 
   the directory ~/SCA/SCA/user/output
8. To analyze additional .cpp files, all you need to do is replace the files located in ~/SCA/SCA/user/source-code
   and run the sca command again.

![Directory_Tree](https://user-images.githubusercontent.com/64028736/140849179-40abb7ef-75b9-4ca8-b878-4b4665f5fedd.JPG)



