@echo off
set CXX_ARGS=-O2 &
set OUTPUT_FILE=../gbemu.exe &

cd src &
g++ main.cpp %CXX_ARGS% -o %OUTPUT_FILE% &
cd ..

