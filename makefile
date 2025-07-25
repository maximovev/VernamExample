CXX = g++
CXXFLAGS = -Wall -Wextra

all:
	$(CXX) $(CXXFLAGS) vernam.cpp -o vernam.elf

vernam.elf: vernam.cpp
	$(CXX) $(CXXFLAGS) vernam.cpp -o vernam.elf
clean:
	rm -f vernam.elf

test:
	./vernam.elf testdata.txt key.dat out.dat fakeouttext.txt fakekey.dat