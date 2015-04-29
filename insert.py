#!/usr/bin/python3
    
import os
import subprocess
import sys
import shutil
import binascii

OBJCOPY = 'arm-none-eabi-objcopy'
OBJDUMP = 'arm-none-eabi-objdump'
NM = 'arm-none-eabi-nm'
AS = 'arm-none-eabi-as'
CC = 'arm-none-eabi-gcc'
CXX = 'arm-none-eabi-g++'

def get_text_section():
	# Dump sections
	out = subprocess.check_output([OBJDUMP, '-t', 'build/linked.o'])
	lines = out.decode().split('\n')
	
	# Find text section
	text = filter(lambda x: x.strip().endswith('.text'), lines)
	section = list(text)[0]
	
	# Get the offset
	offset = int(section.split(' ')[0], 16)
	
	return offset
	
def symbols(subtract=0):
	out = subprocess.check_output([NM, 'build/linked.o'])
	lines = out.decode().split('\n')
	
	name = ''
	
	ret = {}
	for line in lines:
		parts = line.strip().split()
		
		if (len(parts) < 3):
			continue
			
		if (parts[1].lower() != 't'):
			continue
			
		offset = int(parts[0], 16)
		ret[parts[2]] = offset - subtract
		
	return ret
	
def insert(rom):
	where = 0x800000
	rom.seek(where)
	with open('build/output.bin', 'rb') as binary:
		data = binary.read()
	rom.write(data)
	return where
		       
def hook(rom, space, hook_at, register=0):
    with open('test.gba', 'rb+') as rom:
        # Align 2
        if hook_at & 1:
            hook_at -= 1
            
        rom.seek(hook_at)
        
        register &= 7
        
        if hook_at % 4:
            data = bytes([0x01, 0x48 | register, 0x00 | (register << 3), 0x47, 0x0, 0x0])
        else:
            data = bytes([0x00, 0x48 | register, 0x00 | (register << 3), 0x47])
            
        space += 0x08000001
        data += (space.to_bytes(4, 'little'))
        rom.write(bytes(data))
        
shutil.copyfile('BPRE0.gba', 'test.gba')
with open('test.gba', 'rb+') as rom:
	offset = get_text_section()
	table = symbols(offset)
	where = insert(rom)

	# Adjust symbol table
	for entry in table:
		table[entry] += where
		
	hook(rom, table['command'], 0x033224, 2)
	hook(rom, table['move_hook'], 0x01D040, 2)
	hook(rom, table['move_button_hook'], 0x02EC10, 0)
	print(table)
    
