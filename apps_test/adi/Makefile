EXEC = ./adi

LIBS = 
CPP = /opt/rh/devtoolset-2/root/usr/bin/gcc
FLAGS = -fplugin=../../roeval_plugin/bin/libplugin.so

SRC= main.c

make : $(EXEC)

$(EXEC) : $(SRC) Makefile
	$(CPP) $(SRC) $(FLAGS) -o $(EXEC)  

clean:
	rm -rf $(OBJ) $(EXEC)

	




