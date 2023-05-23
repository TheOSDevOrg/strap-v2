SRC_DIR:=src
OBJ_DIR:=out/obj
OUT_DIR:=out/bin
SCRIPTS:=scripts
STATIC :=static

.DEFAULT_GOAL:=all

CXX=i686-elf-g++
CC=i686-elf-gcc
LD=i686-elf-ld
NASM=nasm
AS=i686-elf-as

NAME=strap-v2
VER	=0.230523.4
COMMIT=$(shell git rev-parse HEAD)

ISO_FILE=out/$(NAME).iso
ISO_DIR =iso_root

DEFINITIONS='-D KERN_NAME="$(NAME)"' '-D KERN_VER="$(VER)"' '-D KERN_COMMIT_ID="$(COMMIT)"'

CXXFLAGS=--std=c++17 $(DEFINITIONS) -Werror=implicit-fallthrough -Werror=return-type -fno-threadsafe-statics -Ilib -Iinc -nostdlib -fno-use-cxa-atexit -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
CFLAGS=--std=c17 $(DEFINITIONS) -Werror=implicit-fallthrough -Werror=implicit-function-declaration -Werror=return-type -Ilib -Iinc -nostdlib -ffreestanding -Wall -Wextra -fno-exceptions
NASMFLAGS=-felf32
ASFLAGS=--32 -nostdlib

#LOADER_SRC=$(SRC_DIR)/boot/loader.cpp
#LOADER_OUT=$(OBJ_DIR)/loader.a

CXX_SOURCES=$(shell find $(SRC_DIR) -name '*.cpp' -not -path '$(SRC_DIR)/boot/*')
C_SOURCES=$(shell find $(SRC_DIR) -name '*.c' -not -path '$(SRC_DIR)/boot/*')
AS_SOURCES=$(shell find $(SRC_DIR) -name '*.s' -not -path '$(SRC_DIR)/boot/*')
NASM_SOURCES=$(shell find $(SRC_DIR) -name '*.asm' -not -path '$(SRC_DIR)/boot/*')

CXX_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(CXX_SOURCES:.cpp=.cpp.o))
C_FILES_OUT =  $(subst $(SRC_DIR),$(OBJ_DIR),$(C_SOURCES:.c=.c.o))
AS_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(AS_SOURCES:.s=.s.o))
NASM_FILES_OUT = $(subst $(SRC_DIR),$(OBJ_DIR),$(NASM_SOURCES:.asm=.asm.o))

LINK_SOURCES=$(shell find $(OBJ_DIR) -name '*.o' -not -path "initrd/*")

.PHONY: all

all: $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link iso run
#bochs: $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link grub run-bochs

build: $(AS_FILES_OUT) $(NASM_FILES_OUT) $(C_FILES_OUT) $(CXX_FILES_OUT) link iso

#as:
#	$(NASM) $(ASFLAGS) 'src/asm/idt_handlers.asm' -o 'src/asm/idt_handlers.o'
#	$(NASM) $(ASFLAGS) 'src/asm/gdt_helpers.asm' -o 'src/asm/gdt_helpers.o'

loader:
	@$(CXX) $(CXXFLAGS) -c $(LOADER_SRC) -o $(LOADER_OUT)
loadervid:
	@$(CXX) $(CXXFLAGS) -c $(LOADER_SRC) -o $(LOADER_OUT) -D'VBE_MODE'

link:
	@$(LD) -nostdlib -Tscripts/link.ld -o '$(OUT_DIR)/$(NAME).bin' $(LINK_SOURCES)
	@echo "LD:   $(LINK_SOURCES) -> $(OUT_DIR)/$(NAME).bin"

clean:
	@-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.bin' -not -path './$(STATIC)/*') $(shell find . -name 'out/*.iso') $(MODULES_OUTPUT)

clean_objects: clean_modules
	@-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.a')
clean_modules:
	@-rm -Rf $(MODULES_OUTPUT)

iso:
	@mkdir $(ISO_DIR)/
	@cp $(STATIC)/* $(OUT_DIR)/$(NAME).bin $(SCRIPTS)/limine.cfg $(ISO_DIR)
	@xorriso -as mkisofs -b limine-cd.bin \
        	 -no-emul-boot -boot-load-size 4 -boot-info-table \
        	 $(ISO_DIR) -o $(ISO_FILE)
	@limine-deploy $(ISO_FILE)
	@echo "ISO:  $(OUT_DIR)/$(NAME).bin -> $(ISO_FILE)"
	@rm -rf $(ISO_DIR)/

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "G++:  $< -> $@"

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "GCC:  $< -> $@"	

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@
	@echo "AS:   $< -> $@"

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@$(NASM) $(NASMFLAGS) $< -o $@
	@echo "NASM: $< -> $@"

run:
	@qemu-system-i386 -cdrom $(ISO_FILE) -serial stdio -vga std -no-reboot -no-shutdown -m 5G
run-kvm:
	@qemu-system-i386 -cdrom $(ISO_FILE) -serial stdio -vga std -no-reboot -no-shutdown -m 1G -enable-kvm -cpu host
#run-bochs:
#	bochs -q -f .bochsrc 
test:
	@echo $(AS_FILES_OUT) $(NASM_FILES_OUT)