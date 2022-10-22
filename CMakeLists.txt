# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_FLAGS "-pthread")
# プロジェクト名と使用する言語を設定
project(textpro C CXX)
# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(testread main.c read.c mainthread.c cpuinfo.c)