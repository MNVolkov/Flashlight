/*
	(С) Волков Максим 2019 ( mr-volkov+bip@yandex.ru )
	Фонарик для Amazfit Bip
	
*/
#ifndef __FLASH_H__
#define __FLASH_H__

#define INACTIVITY_PERIOD_FLASH_ON		120000		//	время по прошествии которого выходим
#define INACTIVITY_PERIOD_FLASH_OFF		5000		//	время по прошествии которого выходим

// состояние фонарика
#define FLASH_STATUS_OFF		0
#define FLASH_STATUS_ON			1
#define FLASH_STATUS_POLICE		2
#define FLASH_STATUS_LSD		3

// отступ между иконкой и текстом
#define V_MARGIN	10

// текущие данные
struct flashlight_ {
		void* 	ret_f;			//	адрес функции возврата
		unsigned char	color_scheme;	//	цветовая схема
		unsigned char	flash_status;	//	статус фонарика
		Elf_proc_* proc;				//	указатель на данные процесса
};


void 	show_flash_screen (void *return_screen);
void 	key_press_flash_screen();
int 	dispatch_flash_screen (void *param);
void	flash_screen_job();

void set_flash_status(int status);
					
#endif