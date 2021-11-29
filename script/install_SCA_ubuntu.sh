#!/bin/bash

# install Java Runtime Enviornment and JDK
sudo apt-get update
sudo apt install default-jdk
sudo apt install default-jre

# install gcc if not already
sudo apt-get install build-essential

# install git
sudo apt install git

#remove SCA and ANTLR directories if they already exist
yes | rm -rf ~/SCA || true
yes | rm -rf ~/ANTLR || true

# create directories for our project and ANTLR files in ~/
cd ~
mkdir SCA
mkdir ANTLR

# clone our SCA repo into newly created directory SCA
git clone https://github.com/alihaider1264/SCA.git ~/SCA

# remove script directory from git clone
# pipe yes to remove command if it asks about removing files
yes | rm -r ~/SCA/script

# make directory /out and /output if they don't already exist
cd ~/SCA/SCA/core
mkdir -p out
cd ~/SCA/SCA/user
mkdir -p output

# Return to ANTLR dir and install required ANTLR files
cd ~/ANTLR

# get working directory and store in variable
workingDir=`pwd`

# download ANTLR jar file
wget https://www.antlr.org/download/antlr-4.9.2-complete.jar

# add CLASSPATH and alias only if they are not already in .bashrc
isFound1=$(grep -c "4.9.2-complete" ~/.bashrc)
if [[ "$isFound1" -eq 0 ]] 
then
    echo "export CLASSPATH=\".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\"" >> ~/.bashrc
fi

isFound2=$(grep -c "alias sca" ~/.bashrc)
if [[ "$isFound2" -eq 0 ]]
then
    # add alias for a.out created to run Driver.cpp
    echo "alias sca='~/SCA/SCA/core/src/a.out'" >> ~/.bashrc
fi
  
# add ANTLR jar file to classpath of currently open terminal
export CLASSPATH=".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH"

# Download CPP14 Lexer and Parser from GitHub
wget https://raw.githubusercontent.com/antlr/grammars-v4/master/cpp/CPP14Lexer.g4
wget https://raw.githubusercontent.com/antlr/grammars-v4/master/cpp/CPP14Parser.g4

# run .jar file on grammar files to produce .java classes
java -jar $workingDir/antlr-4.9.2-complete.jar CPP14Lexer.g4
java -jar $workingDir/antlr-4.9.2-complete.jar CPP14Parser.g4

# compile java classes
javac CPP14*.java

# return to src directory and compile SCA files
cd ~/SCA/SCA/core/src
g++ Driver.cpp

pkg1=$(dpkg-query -W -f='${Status}' default-jdk)
pkg2=$(dpkg-query -W -f='${Status}' default-jre)
pkg3=$(dpkg-query -W -f='${Status}' git)
pkg4=$(dpkg-query -W -f='${Status}' g++)
file1=~/SCA/SCA/core/src/SCA.h
file2=~/ANTLR/CPP14Parser$AbstractDeclaratorContext.class
file3=~/SCA/SCA/core/src/a.out
if [[ "$pkg1" = "install ok installed" ]] 
then
    if [[ "$pkg2" = "install ok installed" ]]
    then
        if [[ "$pkg3" = "install ok installed" ]]
        then
            if [[ "$pkg4" = "install ok installed" ]]
            then
                if [[ -f "$file1" ]]
                then
                    if [[ -f "$file2" ]]
                    then
                        if [[ -f "$file3" ]]
                        then
                            # Display message to user after completion of installation
                            echo ""  # skip a couple lines and print directions to screen
                            echo ""
                            echo "---------------------------------------------------------------------"
                            echo "System install successful!"
                            echo "---------------------------------------------------------------------"
                            echo "HOW TO RUN THE SYSTEM:"
                            echo "1. Load user source files to the folder: ~/SCA/SCA/user/source-code"
                            echo "2. Open a NEW terminal and run the command: sca"
                            echo "3. File explorer should open with your results as .html files"
                            echo "---------------------------------------------------------------------"
                            echo ""
                        else
                            echo "Install failed ... $file3 does NOT exist ... problem compiling SCA"
                        fi
                    else
                        echo "Install failed ... $file2 does NOT exist ... problem with ANTLR"
                    fi
                else
                    echo "Install failed ... $file1 does NOT exist ... problem with git clone"
                fi
            else
                echo "Install failed ... $pkg4 not installed"
            fi
        else
            echo "Install failed ... $pkg3 not installed"
        fi
    else
        echo "Install failed ... $pkg2 not installed"
    fi  
else
    echo "Install failed ... $pkg1 not installed"
fi
