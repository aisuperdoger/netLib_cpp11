#!/bin/bash

set -e

# 如果没有build目录 创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -fr `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/netLib       .so库拷贝到 /usr/lib
if [ ! -d /usr/include/netLib ]; then
    mkdir /usr/include/netLib
fi

for header in `ls *.h`
do
    cp $header /usr/include/netLib
done

cp `pwd`/lib/libnetLib.so /usr/lib

ldconfig