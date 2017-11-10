
#compilers
NVCC=nvcc
CC=g++

#include path
INC_PATH	:= -I./EasyBMP_1.06 -I./inc
LINK_PATH	:= ./EasyBMP_1.06


#files
SRCDIR		:= ./src
EASY_BMP_DIR	:= ./EasyBMP_1.06
CU_DIR		:= ./CU
SRC		:= $(wildcard $(SRCDIR)/*.cpp)
INC		:= $(wildcard ./inc/*.h)
CU		:= $(wildcard $(CU_DIR)/*.cu)
EASY_BMP	:= $(wildcard $(EASY_BMP_DIR)/*.cpp)
ODIR		:= ./obj

#compiler and linker flags
LFLAGS		:= -gencode=arch=compute_35,code=sm_35
CFLAGS		:= -std=c++11  $(INC_PATH)
NFLAGS		:= -std=c++11 -gencode=arch=compute_35,code=sm_35

#object files
OBJ		= $(SRC:$(SRCDIR)/%.cpp=$(ODIR)/%.o) $(EASY_BMP:$(EASY_BMP_DIR)/%.cpp=$(ODIR)/%.o) $(CU:$(CU_DIR)/%.cu=$(ODIR)/%.o)


all: objDir voronoi

voronoi: $(OBJ) 
	$(NVCC) -o $@ $^ $(LFLAGS)


$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -Wall  -c $< -o $@

$(ODIR)/%.o: $(EASY_BMP_DIR)/%.cpp
	$(CC) $(CFLAGS)  -c $< -o $@

$(ODIR)/%.o: $(CU_DIR)/%.cu
	$(NVCC) $(NFLAGS) -c $< -o $@

%.o: %.cu
	$(NVCC) $(NFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o

.PHONU: objDir
objDir:
	mkdir -p $(ODIR)


