#include <kernel/keyboard.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/io.h>
#include <kernel/tty.h>
#include <stdint.h>

static char* _qwertyuiop = "qwertyuiop"; // 0x10-0x1c
static char* _asdfghjkl = "asdfghjkl";
static char* _zxcvbnm = "zxcvbnm";
static char* _num = "1234567890";

static char keyboard_to_ascii(uint8_t key)
{
	if(key == ENTER_PRESSED) return '\n';
	if(key == SPACE_PRESSED) return ' ';
	if(key == BACKSPACE_PRESSED) return '\r';
	if(key == POINT_PRESSED) return '.';

	if(key >= ONE_PRESSED && key <= ZERO_PRESSED)
	{
		return _num[key - ONE_PRESSED];
	}

	if(key >= Q_PRESSED && key <= P_PRESSED)
	{
		return _qwertyuiop[key - 0x10];
	} 
	else if(key >= A_PRESSED && key <= L_PRESSED)
	{
		return _asdfghjkl[key - 0x1E];
	} 
	else if(key >= Z_PRESSED && key <= M_PRESSED)
	{
		return _zxcvbnm[key - 0x2C];
	}
	return 0;
}

extern void keyboard_irq();

void keyboard_irq_handler()
{
    uint8_t key = insb(0x60);
    char str[2];
    str[0] = keyboard_to_ascii(key);
    str[1] = 0;
    print(str);
    pic_send_EOI(1);
}

void keyboard_init()
{
    idt_set(0x21, keyboard_irq);
}