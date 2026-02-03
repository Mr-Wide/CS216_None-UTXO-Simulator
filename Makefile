CXX = g++
# -I. looks in root, others look in your subfolders
CXXFLAGS = -Wall -std=c++17 -I. -IPart3_VivekSahu -INiketans -IThanush_part4

SRCS = main.cpp \
       verifytx.cpp \
       Niketans/utxoman.cpp \
       Part3_VivekSahu/mempool.cpp \
       Thanush_part4/mining.cpp

TARGET = simulator

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	@if exist $(TARGET).exe del /f /q $(TARGET).exe