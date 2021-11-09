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
cd ~/SCA
rmdir -r script

# make directory out if it doesn't alread exist
cd ~/SCA/SCA/core
mkdir -p out

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
