#!/bin/bash
# cd Documents/ANTLR_Install

# install Java Runtime Enviornment and JDK
sudo apt-get update
sudo apt install default-jdk
sudo apt install default-jre

# install gcc if not already
sudo apt-get install build-essential

# install git
sudo apt install git

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

# add ANTLR jar file to classpath
echo "export CLASSPATH=\".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\"" >> ~/.bashrc
export CLASSPATH=".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH"

# add alias for grun
echo "alias grun='java -Xmx500M -cp \"$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\" org.antlr.v4.gui.TestRig'" >> ~/.bashrc

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

# Display message to user after completion of installation
echo ""  # skip a couple lines and print directions to screen
echo ""
echo "----------------------------------------------"
echo "System install successful!"
echo "----------------------------------------------"
echo "HOW TO RUN THE SYSTEM:"
echo "1. Load user source files to the folder: ~/SCA/SCA/user/source-code"
echo "2. Navigate to ~/SCA/SCA/core/src and run a.out using command ./a.out"
echo "3. To view the HTML files navigate to ~/SCA/SCA/user/output and double click to see results."
