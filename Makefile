# Makefile for scheduling program
#
# make rr_p - for round-robin with priority scheduling
# make edf - for earliest deadline first scheduling
# make pa - for priority with aging scheduling
# make all - to compile all schedulers
# make clean - to remove all compiled files

CC=gcc
# Added -pthread for compiling and linking thread-aware code
CFLAGS=-Wall -pthread

# List of all object files common to schedulers
COMMON_OBJS = driver.o list.o CPU.o

# List of all scheduler executables
TARGETS = rr_p edf pa

all: $(TARGETS)

clean:
	rm -rf *.o
	rm -rf $(TARGETS)
	# If your old makefile created 'rr' instead of 'rr_p', you might want to add:
	# rm -rf rr

# --- Target Executables ---

rr_p: $(COMMON_OBJS) schedule_rr_p.o
	$(CC) $(CFLAGS) -o rr_p $(COMMON_OBJS) schedule_rr_p.o

edf: $(COMMON_OBJS) schedule_edf.o
	$(CC) $(CFLAGS) -o edf $(COMMON_OBJS) schedule_edf.o

pa: $(COMMON_OBJS) schedule_pa.o
	$(CC) $(CFLAGS) -o pa $(COMMON_OBJS) schedule_pa.o

# --- Object File Compilation Rules ---

driver.o: driver.c schedule_rr_p.h schedule_edf.h schedule_pa.h list.h task.h
	$(CC) $(CFLAGS) -c driver.c

schedule_rr_p.o: schedule_rr_p.c schedule_rr_p.h CPU.h list.h task.h
	$(CC) $(CFLAGS) -c schedule_rr_p.c

schedule_edf.o: schedule_edf.c schedule_edf.h CPU.h list.h task.h
	$(CC) $(CFLAGS) -c schedule_edf.c

schedule_pa.o: schedule_pa.c schedule_pa.h CPU.h list.h task.h
	$(CC) $(CFLAGS) -c schedule_pa.c

list.o: list.c list.h task.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c CPU.h task.h
	$(CC) $(CFLAGS) -c CPU.c

# Phony target for 'all' to ensure it always tries to build, even if a file named 'all' exists.
.PHONY: all clean
