CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC = Firewall.cpp LoadBalancer.cpp Request.cpp Server.cpp SingleServer.cpp main.cpp
BIN = load_balancer

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

run: all
	./$(BIN)

docs:
	doxygen Doxyfile

clean:
	rm -f $(BIN)