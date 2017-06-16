# usage : 
# make -f makefile
# make -f makefile clean

dir_bin = .

INC = -Iexternal/gasoncpp
LIB = 

SRC1 = src/socket.cc
SRC2 = src/http.cc
SRC3 = src/ftp.cc
SRC4 = external/gasoncpp/gason.cpp
SRC_MAIN1 = examples/tcp_client_01.cc
SRC_MAIN2 = examples/tcp_server_01.cc
SRC_MAIN3 = examples/http_client.cc
SRC_MAIN4 = examples/ftp_client.cc
SRC_MAIN5 = examples/json_client.cc
SRC_MAIN6 = examples/json_server.cc
SRC_MAIN7 = examples/http_server.cc

OBJ1 = socket.o
OBJ2 = http.o
OBJ3 = ftp.o
OBJ4 = gason.o
OBJ_MAIN1 = tcp_client_01.o
OBJ_MAIN2 = tcp_server_01.o
OBJ_MAIN3 = http_client.o
OBJ_MAIN4 = ftp_client.o
OBJ_MAIN5 = json_client.o
OBJ_MAIN6 = json_server.o
OBJ_MAIN7 = http_server.o

EXE1 = $(dir_bin)/tcp_client_01
EXE2 = $(dir_bin)/tcp_server_01
EXE3 = $(dir_bin)/http_client
EXE4 = $(dir_bin)/ftp_client
EXE5 = $(dir_bin)/json_client
EXE6 = $(dir_bin)/json_server
EXE7 = $(dir_bin)/http_server

OBJs = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ_MAIN1) $(OBJ_MAIN2) $(OBJ_MAIN3) $(OBJ_MAIN4) $(OBJ_MAIN5) $(OBJ_MAIN6) $(OBJ_MAIN7)
EXEs = $(EXE1) $(EXE2) $(EXE3) $(EXE4) $(EXE5) $(EXE6) $(EXE7)

CXX = g++
CXX_FLAG = -g -Wall -std=c++0x 
LINK_FLAG = -lstdc++ 

all:  $(EXEs) 

test : run1 

$(EXE1) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN1)
	$(CXX) $(LINK_FLAG) -o $(EXE1) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN1) $(LIB)

$(EXE2) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN2) 
	$(CXX) $(LINK_FLAG) -o $(EXE2) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN2) $(LIB)

$(EXE3) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN3) 
	$(CXX) $(LINK_FLAG) -o $(EXE3) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN3) $(LIB)

$(EXE4) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN4) 
	$(CXX) $(LINK_FLAG) -o $(EXE4) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN4) $(LIB)

$(EXE5) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN5) 
	$(CXX) $(LINK_FLAG) -o $(EXE5) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN5) $(LIB)

$(EXE6) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN6) 
	$(CXX) $(LINK_FLAG) -o $(EXE6) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN6) $(LIB)

$(EXE7) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ_MAIN7) 
	$(CXX) $(LINK_FLAG) -o $(EXE7) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ_MAIN7) $(LIB)

$(OBJ1) : $(SRC1)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC3)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC3) -o $(OBJ3)

$(OBJ4) : $(SRC4)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC4) -o $(OBJ4)
	
$(OBJ_MAIN1) : $(SRC_MAIN1)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN1) -o $(OBJ_MAIN1)

$(OBJ_MAIN2) : $(SRC_MAIN2)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN2) -o $(OBJ_MAIN2)

$(OBJ_MAIN3) : $(SRC_MAIN3)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN3) -o $(OBJ_MAIN3)

$(OBJ_MAIN4) : $(SRC_MAIN4)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN4) -o $(OBJ_MAIN4)

$(OBJ_MAIN5) : $(SRC_MAIN5)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN5) -o $(OBJ_MAIN5)

$(OBJ_MAIN6) : $(SRC_MAIN6)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN6) -o $(OBJ_MAIN6)

$(OBJ_MAIN7) : $(SRC_MAIN7)
	$(CXX) $(INC) $(CXX_FLAG) -c $(SRC_MAIN7) -o $(OBJ_MAIN7)

clean:
	rm -f $(OBJs) $(EXEs)



