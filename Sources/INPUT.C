#include "input.h"

unsigned char raw_key, kbd[128];
	
_go32_dpmi_seginfo old_key_handler,new_key_handler;

int keys_active=0;
	
void key_handler(void)
{
        unsigned char al;

        asm("cli; pusha");
	
        raw_key=inportb(0x60);
		
        al=inportb(0x61);
        al |= 0x82;
        outportb(0x61, al);
        al &= 0x7f;
        outportb(0x61, al);

        if(raw_key < 128)
        {
                if(!kbd[raw_key])
                {
                        kbd[raw_key]=1;
                        keys_active++;
                }
        }
        else
        {
                if(kbd[raw_key-128])
                {
                        kbd[raw_key-128] = 0;
                        keys_active--;
                }
        }
        outportb(0x20, 0x20);
        asm("popa; sti");
}

void key_init(void)
{
        int index;
        new_key_handler.pm_offset   = (int)key_handler;
        new_key_handler.pm_selector = _go32_my_cs();
        _go32_dpmi_get_protected_mode_interrupt_vector(0x9, &old_key_handler);
        _go32_dpmi_allocate_iret_wrapper(&new_key_handler);
        _go32_dpmi_set_protected_mode_interrupt_vector(0x9,&new_key_handler);
        for(index=0;index<128;index++)
        kbd[index] = 0;
}

void key_delete(void)
{
        _go32_dpmi_set_protected_mode_interrupt_vector(0x9,&old_key_handler);
}
