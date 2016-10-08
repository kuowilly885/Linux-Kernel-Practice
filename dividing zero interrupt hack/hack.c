#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/desc.h>

#define DIVIDE_ERROR 0x00
 
char msg[200];
char *str = "test";
struct desc_ptr newidtr, oldidtr,tempidtr;
gate_desc *newidt, *oldidt, *tempidt;
int counter = 0;
unsigned long old_stub = 0xc15d9c64;
struct tty_struct *my_tty;

extern asmlinkage void new_stub(void);
 
int write_console (char *str)
{
         struct tty_struct *my_tty;
         if((my_tty=current->signal->tty) != NULL)
         {
                ((my_tty->driver->ops->write) (my_tty,str,strlen(str)));
                return 0;
         }
         else return -1;
}
 
static void load_IDTR(void *addr)
{
    asm volatile("lidt %0"::"m"(*(unsigned short *)addr));
}
 
/*C handler */
void my_func(void)
{
    sprintf(msg, "Counter = %d \r\n", ++counter);
     ((my_tty->driver->ops->write)(my_tty,msg,strlen(msg)));
}

/*assembly handler */
void my_dummy(void)
{
        __asm__ (
        ".globl new_stub    \n\t"
        ".align 4, 0x90     \n\t"
        "new_stub:      \n\t"
        "pushfl             \n\t"
        "pushal             \n\t"
        "call my_func       \n\t"
        "popal              \n\t"
        "popfl              \n\t"
        "jmp *old_stub      \n\t"
         ::);
}
  
int __init hook_init(void){
 
    write_console("William hack interrupt_0\r\n");
    my_tty = current->signal->tty;
    store_idt(&oldidtr);
    oldidt = (gate_desc *)oldidtr.address;
    newidtr.address = __get_free_page(GFP_KERNEL);
    if(!newidtr.address)
        return -1;
    newidtr.size = oldidtr.size;
    newidt = (gate_desc *)newidtr.address; 
    memcpy(newidt, oldidt, oldidtr.size);
    pack_gate(&newidt[DIVIDE_ERROR], GATE_INTERRUPT, (unsigned long)new_stub, 0, 0, __KERNEL_CS);
    smp_call_function(load_IDTR,(void *)&newidtr, 0);
 
    return 0; 
} 
void __exit hook_exit(void){

    write_console("William recovered interrupt_0 \r\n");
    sprintf(msg, "Interrupt_0 handled during hijacking = %d \r\n", 3);
    write_console(msg);
    smp_call_function(load_IDTR, (void *)&oldidtr, 0);
    if(newidtr.address)
        free_page(newidtr.address); 
}
  
module_init(hook_init);
module_exit(hook_exit);
MODULE_LICENSE("GPL");
