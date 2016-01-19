#!/bin/bash
# Execute in script folder

# Arg 1 = Project name
# Arg 2 = Output directory
proj="$1"
out="$2"
tmp="tmp"

if [ "$out" = "" ]
then
    out="$proj"
fi

# make tempary files
cp CMakeLists.txt "CMakeLists.$tmp"
cp FileLists.cmake "FileLists.$tmp"
cp LibLists.cmake "LibLists.$tmp"
cp Character.cpp "Character.cpp$tmp"
cp Character.h "Character.h$tmp"
cp main.cpp "main.$tmp"

# replace project_name by actual project name $proj
tpmd="CMakeLists.$tmp FileLists.$tmp LibLists.$tmp Character.cpp$tmp Character.h$tmp main.$tmp"
sed -i "s/PROJECTNAME/"$proj"/g" $tpmd


# create output directory
mkdir "$out"

# copy processed tempory files in output directory
mv "CMakeLists.$tmp" "$out/CMakeLists.txt"
mv "FileLists.$tmp" "$out/FileLists.cmake"
mv "LibLists.$tmp" "$out/LibLists.cmake"
mv "Character.cpp$tmp" "$out/"$proj"Character.cpp"
mv "Character.h$tmp" "$out/"$proj"Character.h"
mv "main.$tmp" "$out/main.cpp"
