DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall 
LDLIBS := -lpthread -lrt
CC := gcc

all : dirs manager pa pb pc pd backup



dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pa: $(DIROBJ)pa.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pb: $(DIROBJ)pb.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pc: $(DIROBJ)pc.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pd: $(DIROBJ)pd.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) 

backup: $(DIROBJ)backup.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)manager students/Students.txt 1000 students/


clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) 

clean_students: #borrar la carpeta de los estudiantes
	find students/ -mindepth 1 ! -name 'Students.txt' -delete

clean_all: clean clean_students
