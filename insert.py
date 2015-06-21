#!/usr/bin/python3
    
import os
import subprocess
import sys
import shutil
import binascii
import textwrap

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
	hook(rom, table['command'], 0x038A50, 2) # 08250A34
	hook(rom, table['move_hook'], 0x01D040, 2)
	hook(rom, table['move_button_hook'], 0x02EC10, 0)
	hook(rom, table['exit_battle_hook'], 0x0159DC, 0)
	hook(rom, table['faint_hook'], 0x0326C4, 3)
	hook(rom, table['level_string_hook'], 0x0483A4, 1)
	#hook(rom, table['mega_level_icon_hook'], 0x049E18, 1)
	hook(rom, table['load_graphics_hook'], 0x03495C, 1)
	#hook(rom, table['show_graphics_hook'], 0x047FD8, 1)
	# Add the fucking animation
	#loc = table['mega_animation_script_data'] + 0x08000000
	#move_index = 33 # tackle
	#rom.seek(move_index * 4 + 0x1C68F4)
	#rom.write(loc.to_bytes(4, 'little'))
	
	# FFS Copy hex
	stupid = {
		'anim_rainbow_y': 24,
		'anim_rainbow_x': 106,
		'anim_mega_stone_x': 24,
		'anim_mega_symbol_x': 24,
		'mega_animation_script_data': 0xAF
	}
	
	for name, length in stupid.items():
		loc = table[name]
		#print('{} (0x{:04X}):'.format(name, loc))
		rom.seek(loc)
		data = rom.read(length)
		bleh = ' '.join('{:02X}'.format(c) for c in data)
		#print('\n'.join(textwrap.wrap(bleh)))
		#print()
	
	width = max(len(key) for key in table.keys())
	
	for key in sorted(table.keys()):
		print(('{:' + str(width) + '} {:08X}').format(key + ':', table[key] + 0x08000000))
	
	
    
