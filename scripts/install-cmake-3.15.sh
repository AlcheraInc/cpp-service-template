#!/bin/bash
#
#   Author  : github.com/luncliff (luncliff@gmail.com)
#
echo "----------------------------------------------------------------------"
echo "                                                                      "
echo " Install CMake                                                        "
echo "  - Version   : 3.15.4                                                "
echo "  - Source    : GitHub Release                                        "
echo "                                                                      "
echo "----------------------------------------------------------------------"

wget -q https://github.com/Kitware/CMake/releases/download/v3.15.4/cmake-3.15.4-Linux-x86_64.tar.gz
tar -xf cmake-3.15.4-Linux-x86_64.tar.gz
mv ./cmake-3.15.4-Linux-x86_64 /tmp/cmake-3.15.4
rm cmake-3.15.4-Linux-x86_64.tar.gz

rsync -ah /tmp/cmake-3.15.4/bin     /usr
rsync -ah /tmp/cmake-3.15.4/share   /usr

cmake --version
