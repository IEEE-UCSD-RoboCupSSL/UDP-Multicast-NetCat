default: mnc

install:
	@echo "..."

#compiler = clang++
compiler = g++

boostlib = -lpthread -lboost_system -lboost_thread 

std = -std=c++17

cppflags = $(std) $(boostlib)
 
mnc: mnc.cpp
	$(compiler) -o $@ $< $(cppflags)


clean:
	@rm mnc