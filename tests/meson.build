project('tests', 'cpp', license : [ 'mit'])
add_global_arguments('-std=c++2a', language : 'cpp')
add_global_arguments('-I../..', language : 'cpp')
add_global_arguments('-lpthread', language : 'cpp')
add_global_arguments('-g', language : 'cpp')
gtest = dependency('gtest')
executable('tests', 'test.cpp', dependencies : gtest) 

