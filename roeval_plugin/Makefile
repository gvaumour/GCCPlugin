EXEC = ./bin/libplugin.so

LIBS = 
CPP = /opt/rh/devtoolset-2/root/usr/bin/gcc
INCLUDE = -I. -I/opt/rh/devtoolset-2/root/usr/lib/gcc/x86_64-redhat-linux/4.8.2/plugin/include/

SRC= $(wildcard src/*.c)
OBJ= $(subst src/, bin/,  $(SRC:.c=.o))

make : $(EXEC)

$(EXEC) : $(OBJ) Makefile
	$(CPP) $(OBJ) $(LIBS) $(FLAGS) -fPIC -shared -o $(EXEC)  

clean:
	rm -rf $(OBJ) $(EXEC)

bin/%.o: src/%.c
	$(CPP) $(INCLUDE) -o $@ $(FLAGS) -fPIC -c $<

	

