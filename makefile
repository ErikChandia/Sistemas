
#Todo de src/../*.cpp a obj/../*.o
define TO
$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1)))
endef

EXEC     := calcu
CC       := g++ -std=c++17
CCFLAGS  := -Wall
SRC      := src
OBJ      := obj
INCLUDE  := $(SRC)

ifdef DEBUG
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif

SUBDIR    := $(shell find $(SRC) -type d)
OBJSUBDIR := $(patsubst $(SRC)%,$(OBJ)%, $(SUBDIR))
ALLCPPS   := $(shell find $(SRC) -type f -iname *.cpp)
ALLOBJS   := $(patsubst %.cpp,%.o,$(ALLCPPS))
#ALLOBJS   := $(foreach F,$(ALLCPPS),$(call TO,$(F)))


$(EXEC) : $(OBJSUBDIR) $(ALLOBJS)
	$(CC) $(patsubst $(SRC)%,$(OBJ)%,$(ALLOBJS)) -o $(EXEC)

%.o : %.cpp
	$(CC) -c $^ -I $(INCLUDE) -o $(patsubst $(SRC)%,$(OBJ)%,$@) $(CCFLAGS)

.PHONY : info

$(OBJSUBDIR) :
	mkdir $(OBJSUBDIR)

info :
	$(info $(SUBDIR))
	$(info $(OBJSUBDIR))
	$(info $(ALLCPPS))
	$(info $(ALLOBJS))

clean:
	rm -r $(OBJ)
