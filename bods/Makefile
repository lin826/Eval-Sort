# MacOS after `brew install boost`
# Reference: https://apple.stackexchange.com/questions/414622/installing-a-c-c-library-with-homebrew-on-m1-macs
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib

all:
	g++ -std=c++11 -Wall src/sortedness_data_generator.cpp -o ./sortedness_data_generator

clean:
	rm -rf src/sortedness_data_generator
