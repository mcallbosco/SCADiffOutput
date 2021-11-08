#!/bin/bash
# cd Documents/ANTLR_Install

# install Java Runtime Enviornment and JDK
sudo apt install default-jdk
sudo apt install default-jre

# install gcc if not already
sudo apt-get update
sudo apt-get install build-essential

# change directory and make directory for ANTLR files needed
cd ~
mkdir SCA && cd SCA

# set up dir structure
mkdir core && mkdir user
cd core && mkdir ANTLR && mkdir src && mkdir dat && mkdir out
cd ../user
mkdir source-code && mkdir output

# Return to ANTLR dir and install required files
cd ../core/ANTLR

# get working directory and store in variable
workingDir=`pwd`

# download ANTLR jar file
wget https://www.antlr.org/download/antlr-4.9.2-complete.jar

# add ANTLR jar file to classpath
echo "export CLASSPATH=\".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\"" >> ~/.bashrc
export CLASSPATH=".:$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH"

#  create aliases (some are not necessary, commented out)
#  echo "alias antlr4='java -Xmx500M -cp \"$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\" org.antlr.v4.Tool'" >> ~/.bashrc
#  alias antlr4='java -Xmx500M -cp "$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
 echo "alias grun='java -Xmx500M -cp \"$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH\" org.antlr.v4.gui.TestRig'" >> ~/.bashrc
#  alias grun='java -Xmx500M -cp "$workingDir/antlr-4.9.2-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'

# Download CPP14 Lexer and Parser from GitHub
wget https://raw.githubusercontent.com/antlr/grammars-v4/master/cpp/CPP14Lexer.g4
wget https://raw.githubusercontent.com/antlr/grammars-v4/master/cpp/CPP14Parser.g4

# run .jar file on grammar files to produce .java classes
java -jar $workingDir/antlr-4.9.2-complete.jar CPP14Lexer.g4
java -jar $workingDir/antlr-4.9.2-complete.jar CPP14Parser.g4

# compile java classes
javac CPP14*.java

# return to src directory and download SCA files
cd ../src
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/ANTLR_Server.cpp
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/ANTLR_Server.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/AST_Parser.cpp
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/AST_Parser.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Driver.cpp
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Node.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Rule.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/SCA.cpp
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/SCA.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/SCAInterface.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/TemplateTable.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Template_Matcher.h
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Tree.cpp
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/Tree.h

# return to dat directory and download data file
cd ../dat
wget https://raw.githubusercontent.com/alihaider1264/SCA/main/rules.txt

# return to src directory and compile SCA files
cd ../src
g++ Driver.cpp
