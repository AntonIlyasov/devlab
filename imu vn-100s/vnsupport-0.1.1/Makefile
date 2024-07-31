UNAME = $(shell uname -s)
CYGWIN = $(findstring CYGWIN, $(UNAME))
MINGW = $(findstring MINGW32, $(UNAME))
CC = gcc
DEBUG = NO
SLEEP = NO
WINDOWS = NO

# Find out if we are using windows
ifneq ("$(MINGW)", "")
	WINDOWS = YES
endif

ifneq ("$(CYGWIN)", "")
	WINDOWS = YES
endif

# If we are using windows, don't append -fPIC as mingw compiles code as position independent.
# Otherwise, compile with -fPIC
ifeq ($(WINDOWS), YES)
ifeq ($(DEBUG), YES)
	CFLAGS += -g -DDEBUG
endif
else
ifeq ($(DEBUG), YES)
	CFLAGS += -g -fPIC -DDEBUG
else
	CFLAGS += -fPIC
endif
endif

# Compile with Sleep Delay support
ifeq ($(SLEEP), YES)
	CFLAGS += -DVNSLEEPDELAY
endif

# Set C Flags
ifeq ("$(UNAME)", "Darwin")
	CFLAGS += -m32
	WINDOWS = NO
endif

# Find if we are on a posix supported system
ifeq ("$(UNAME)", "Linux")
	WINDOWS = NO
endif

ifneq ("$(CYGWIN)", "")
	WINDOWS = YES
	CFLAGS += -DCYGWIN
endif

# Disable VNSetup.h if we are on MinGW. MinGW doesn't have termios.h
ifneq ("$(MINGW)", "")
	WINDOWS = YES
	CFLAGS += -DMINGW
endif

ifeq ($(WINDOWS), NO)
# Check for Mac OS X, if not, Linux
ifeq ("$(UNAME)", "Darwin")
all: VNWrite.o VNRead.o VNChecksum.o VNSetup.o Macros.o VNControl.o
	echo "Compiling for Mac OS X"
	$(CC) $(CFLAGS) -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,0,-current_version,0 -o libvnsupport-0.1.1.dylib VNControl.o VNWrite.o VNRead.o VNChecksum.o VNSetup.o Macros.o
	ln -sf libvnsupport-0.1.1.dylib libvnsupport-0.dylib
	ln -sf libvnsupport-0.dylib libvnsupport.dylib
else
all: VNWrite.o VNRead.o VNChecksum.o VNSetup.o Macros.o VNControl.o
	echo "Compiling for Linux/Other"
	$(CC) $(CFLAGS) -shared -Wl,-soname,libvnsupport.so.0 -o libvnsupport.so.0.1.1 VNWrite.o VNRead.o VNChecksum.o VNSetup.o Macros.o VNControl.o
	ln -sf libvnsupport.so.0.1.1 libvnsupport.so.0
	ln -sf libvnsupport.so.0 libvnsupport.so
endif
# Do Windows compilation here
else
ifneq ("$(MINGW)", "")
all: VNWrite.o VNRead.o VNChecksum.o Macros.o VNControl.o
	echo "Compiling for MinGW"
	$(CC) $(CFLAGS) -Wall -shared VNWrite.o VNRead.o VNChecksum.o Macros.o VNControl.o -o libvnsupport-0.1.1.dll
	ln -sf libvnsupport-0.1.1.dll libvnsupport-0.dll
	ln -sf libvnsupport-0.dll libvnsupport.dll
else
# Cygwin
all: VNWrite.o VNRead.o VNChecksum.o Macros.o VNControl.o VNSetup.o
	echo "Compiling for Cygwin"
	$(CC) $(CFLAGS) -Wall -shared VNWrite.o VNRead.o VNChecksum.o Macros.o VNControl.o VNSetup.o -o libvnsupport-0.1.1.dll
	ln -sf libvnsupport-0.1.1.dll libvnsupport-0.dll
	ln -sf libvnsupport-0.dll libvnsupport.dll
endif
endif

ifeq ($(WINDOWS), NO)
clean:
	rm -rf VNWrite.o VNRead.o VNSetup.o Macros.o VNControl.o VNChecksum.o libvnsupport*
else
clean: 
	rm -rf VNWrite.o VNRead.o Macros.o VNControl.o VNChecksum.o VNSetup.o libvnsupport-0.1.1.dll libvnsupport-0.dll libvnsupport.dll
endif

vntest: vntest.c
	$(CC) $(CFLAGS) -L. -lvnsupport vntest.c -o vntest

clean_vntest:
	rm -rf vntest vntest.exe

ifeq ($(WINDOWS), NO)
ifeq ("$(UNAME)", "Darwin")
install:
	cp libvnsupport.dylib /usr/local/lib/
	cp libvnsupport-0.dylib /usr/local/lib/
	cp libvnsupport-0.1.1.dylib /usr/local/lib/
else
install:
	cp libvnsupport.so /usr/local/lib/
	cp libvnsupport.so.0 /usr/local/lib/
	cp libvnsupport.so.0.1.1 /usr/local/lib/
endif
else
endif

ifeq ($(WINDOWS), NO)
ifeq ("$(UNAME)", "Darwin")
uninstall:
	rm /usr/local/lib/libvnsupport-0.1.1.dylib
	rm /usr/local/lib/libvnsupport-0.dylib
	rm /usr/local/lib/libvnsupport.dylib
else
uninstall:
	rm /usr/local/lib/libvnsupport.so
	rm /usr/local/lib/libvnsupport.so.0
	rm /usr/local/lib/libvnsupport.so.0.1.1
endif
endif

VNWrite.o: VNWrite.h VNWrite.c VNDefines.h VNStructs.h VNChecksum.h

VNRead.o: VNDefines.h VNStructs.h VNChecksum.h VNWrite.h VNWrite.c

VNChecksum.o: VNChecksum.h VNChecksum.c

VNControl.o: VNControl.h VNControl.c VNDefines.h VNStructs.h

VNSetup.o: VNStructs.h VNSetup.h VNSetup.c

Macros.o: Macros.h Macros.c

