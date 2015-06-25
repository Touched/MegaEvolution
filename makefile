CC=arm-none-eabi-gcc
CXX=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objcopy

DEFINES=-DBPRE -DSOFTWARE_VERSION=0
CFLAGS=-mthumb -mno-thumb-interwork -mcpu=arm7tdmi -fno-inline -mlong-calls -march=armv4t -Wall -O2 $(DEFINES)
CXXFLAGS=-mthumb -mthumb-interwork -mcpu=arm7tdmi -mlong-calls -march=armv4t -Wall -O2 $(DEFINES)
ASFLAGS=-mthumb
LDFLAGS=-z muldefs

all: command  move_exec_hook anim strings mega button revert_hook level_string_hook images static_assets shaker_hooks
	$(LD) $(LDFLAGS) -T linker.lsc -T BPRE.sym -o build/linked.o build/bs_command.o build/anim.o build/anim_script.o build/animscript.o build/move_exec_hook.o build/strings.o build/mega.o build/move_menu_hook.o build/button.o build/exit_battle_hook.o build/faint_hook.o build/healthbar.o build/healthbar_hooks.o build/images_indicators.o build/images_mega_trigger.o build/level_string_hook.o build/healthbar_shaker_hooks.o
	$(OBJCOPY) -O binary build/linked.o build/output.bin
	
static_assets:
	$(CC) $(CFLAGS) -c src/images/indicators.c -o build/images_indicators.o
	$(CC) $(CFLAGS) -c src/images/mega_trigger.c -o build/images_mega_trigger.o
	
images:
	grit assets/indicators.png -gzl -gB 4 -ftc -fh -pe16 -pT4 -gu8 -o src/images/indicators.c
	grit assets/mega_trigger.png -gzl -gB 4 -ftc -fh -pe16 -pT1 -gu8 -o src/images/mega_trigger.c

command:
	$(CC) $(CFLAGS) -c src/bs_command.c -o build/bs_command.o
	
anim:
	$(AS) $(AFLAGS) -c src/anim.s -o build/anim.o
	$(AS) $(AFLAGS) -c src/animscript.s -o build/animscript.o
	$(CC) $(CFLAGS) -c src/anim_script.c -o build/anim_script.o
	
strings:
	$(CC) $(CFLAGS) -c src/strings.c -o build/strings.o
	
mega:
	$(CC) $(CFLAGS) -c src/mega.c -o build/mega.o

move_exec_hook:
	$(AS) $(AFLAGS) -c src/move_exec_hook.s -o build/move_exec_hook.o
	$(CC) $(CFLAGS) -c src/button.c -o build/button.o
	
level_string_hook:
	$(AS) $(AFLAGS) -c src/healthbar_hooks.s -o build/healthbar_hooks.o
	$(CC) $(CFLAGS) -c src/healthbar.c -o build/healthbar.o
	$(AS) $(AFLAGS) -c src/level_string_hook.s -o build/level_string_hook.o
	
revert_hook:
	$(AS) $(AFLAGS) -c src/exit_battle_hook.s -o build/exit_battle_hook.o
	$(AS) $(AFLAGS) -c src/faint_hook.s -o build/faint_hook.o
	
shaker_hooks:
	$(AS) $(AFLAGS) -c src/healthbar_shaker_hooks.s -o build/healthbar_shaker_hooks.o
	
button:
	$(AS) $(AFLAGS) -c src/move_menu_hook.s -o build/move_menu_hook.o


