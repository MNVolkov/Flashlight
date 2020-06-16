/*
	(С) Волков Максим 2019 ( mr-volkov+bip@yandex.ru )
	Фонарик для Amazfit Bip
	
	Функции:
	1. При запуске устанавливается максимальная яркость экрана, фон экрана белый, отключается таймаут выхода
	2. При нажатии на экран, устанавливается обычная яркость экрана, фон черный.

	TODO:
	1. При свайпе вверх/вниз меняется цветовая схема (белый, красный, голубой, зеленый)
	
*/

#include <libbip.h>
#include "flash.h"
// #define DEBUG_LOG

//	структура меню экрана калькулятора
struct regmenu_ menu_flash_screen = {
						55,
						1,
						0,
						dispatch_flash_screen,
						key_press_flash_screen, 
						flash_screen_job,
						0,
						show_flash_screen,
						0,
						0
					};

static char backlight_percent[6] = {0, 10, 20, 40, 60, 90};

int main(int param0, char** argv){	//	переменная argv не определена
	show_flash_screen((void*) param0);
}

void show_flash_screen (void *param0){
struct flashlight_** 	flash_p = get_ptr_temp_buf_2(); 		//	указатель на указатель на данные экрана 
struct flashlight_ *	flash;						//	указатель на данные экрана

if ( (param0 == *flash_p) && get_var_menu_overlay()){ // возврат из оверлейного экрана (входящий звонок, уведомление, будильник, цель и т.д.)

	flash = *flash_p;						//	указатель на данные необходимо сохранить для исключения 
											//	высвобождения памяти функцией reg_menu
	*flash_p = NULL;						//	обнуляем указатель для передачи в функцию reg_menu	

	// 	создаем новый экран, при этом указатели temp_buf_1 и temp_buf_2 были равны 0 и память не была высвобождена	
	reg_menu(&menu_flash_screen, 0); 		// 	menu_overlay=0
	
	*flash_p = flash;					//	восстанавливаем указатель на данные после функции reg_menu

//	set_flash_status(flash->flash_status);
} else { // если запуск функции произошел из меню, 


	// создаем экран
	reg_menu(&menu_flash_screen, 0);

	// выделяем необходимую память и размещаем в ней данные
	*flash_p = (struct flashlight_ *)pvPortMalloc(sizeof(struct flashlight_));
	flash = *flash_p;		//	указатель на данные
	
	// очистим память под данные
	_memclr(flash, sizeof(struct flashlight_));
	
	flash->proc = param0;	

	// запомним адрес указателя на функцию в которую необходимо вернуться после завершения данного экрана
	if ( param0 && flash->proc->elf_finish ) 			//	если указатель на возврат передан, то возвращаемся на него
		flash->ret_f = flash->proc->elf_finish;
	else					//	если нет, то на циферблат
		flash->ret_f = show_watchface;
	
	

flash->flash_status = FLASH_STATUS_ON;

}

// таймер на job
//set_update_period(1, INACTIVITY_PERIOD_FLASH_OFF);
set_flash_status(flash->flash_status);
}


void set_flash_status(int status){
	switch (status){
		default:
		case FLASH_STATUS_OFF:{
		set_bg_color(COLOR_BLACK);
		fill_screen_bg();
		
		set_fg_color(COLOR_AQUA);
		
		set_graph_callback_to_ram_1();
		load_font(); // подгружаем шрифты
		
		// иконка солнышка 57х57
		show_res_by_id(ICON_SUNNY, (176-57)/2, (176-57-get_text_height()-V_MARGIN)/2);
		
		int pos_y = (176-57-get_text_height()-V_MARGIN)/2 + 57 + V_MARGIN;
		
		// надпись
		switch (get_selected_locale()){
				case locale_ru_RU:{
					text_out_center("Включить", 88, pos_y);
					break;
				}
				case locale_it_IT:{
					text_out_center("Spegni", 88, pos_y);
					break;
				}
				case locale_es_ES:{
					text_out_center("Encender", 88, pos_y);
					break;
				}
				case locale_fr_FR:{
					text_out_center("Activer", 88, pos_y);
					break;
				}
				case locale_el_GR:{
					text_out_center("Ενεργοποίηση", 88, pos_y);
					break;
				}
				default:{
					text_out_center("Turn ON", 88, pos_y);
					break;
				}
		}
	
		// 
		set_display_state_value(8, 1);	// 1 - не выходить из экрана
		set_display_state_value(4, 0);	// 1 - не выключать подсветку
		set_display_state_value(2, 0);	// 1 - 

	
		// подсветка на установленную
		int bl_index = get_backlight_value();
	
		
		// плавное угасание подсветки до уставноленного в настройках уровня
		/*
		for (int i = backlight_percent[5]; i > backlight_percent[bl_index]; i--){
			set_backlight_percent(i);
			set_backlight_state(1);
			vTaskDelay(7);
		};
		*/
		set_backlight_percent(backlight_percent[bl_index]);
		set_backlight_state(1);

		// таймер бездействия
		set_update_period(1, 10000);

		break;
		}
		
		case FLASH_STATUS_ON:{
		set_bg_color(COLOR_WHITE);
		fill_screen_bg();
		
		set_fg_color(COLOR_BLUE);
		
		set_graph_callback_to_ram_1();
		load_font(); // подгружаем шрифты
		
		// иконка солнышка 57х57
		//show_res_by_id(ICON_SUNNY, (88-57)/2, (88-57-get_text_height()-V_MARGIN)/2);
		
		int pos_y = (176-57-get_text_height()-V_MARGIN)/2 + 57 + V_MARGIN;
		
		// надпись
		switch (get_selected_locale()){
				case locale_ru_RU:{
					text_out_center("Выключить", 88, pos_y);
					break;
				}
				case locale_it_IT:{
					text_out_center("Accendi", 88, pos_y);
					break;
				}
				case locale_es_ES:{
					text_out_center("Apagar", 88, pos_y);
					break;
				}
				case locale_fr_FR:{
					text_out_center("Éteindre", 88, pos_y);
					break;
				}
				case locale_el_GR:{
					text_out_center("Απενεργοποίηση", 88, pos_y);
					break;
				}
				default:{
					text_out_center("Turn OFF", 88, pos_y);
					break;
				}
		}
		
		// при бездействии не выходить
		set_display_state_value(8, 1);
		set_display_state_value(4, 1);
		set_display_state_value(2, 1);
				
		// подсветка на всю
		set_backlight_percent(90);
		set_backlight_state(1);
		
		//	отключим таймер бездействия
		set_update_period(0, 0);

		break;
		}		
	}
}


void key_press_flash_screen(){
	struct flashlight_** 	flash_p = get_ptr_temp_buf_2(); 		//	указатель на указатель на данные экрана 
	struct flashlight_ *	flash = *flash_p;			//	указатель на данные экрана
	
	flash->flash_status = FLASH_STATUS_OFF;
	set_flash_status(flash->flash_status);
	repaint_screen_lines(0, 176);	
	show_menu_animate(flash->ret_f, (int) show_flash_screen, ANIMATE_RIGHT);	
			

};


void flash_screen_job(){

struct flashlight_** 	flash_p = get_ptr_temp_buf_2(); 		//	указатель на указатель на данные экрана 
struct flashlight_ *	flash = *flash_p;			//	указатель на данные экрана


// при достижении таймера обновления выходим
flash->flash_status = FLASH_STATUS_OFF;
set_flash_status(flash->flash_status);
repaint_screen_lines(0, 176);
show_menu_animate(flash->ret_f, (int) show_flash_screen, ANIMATE_RIGHT);

}

int dispatch_flash_screen (void *param){
	struct flashlight_** 	flash_p = get_ptr_temp_buf_2(); 		//	указатель на указатель на данные экрана 
	struct flashlight_ *	flash = *flash_p;			//	указатель на данные экрана
	
	
	// продлить таймер выхода при бездействии через INACTIVITY_PERIOD с
	//set_update_period(1, INACTIVITY_PERIOD);
	
	//	char text_buffer[32];	
	 struct gesture_ *gest = param;
	 int result = 0;
	 
	switch (gest->gesture){
		case GESTURE_CLICK: {
			// вибрация при любом нажатии на экран
			vibrate (1, 40, 0);

			flash->flash_status = flash->flash_status?FLASH_STATUS_OFF:FLASH_STATUS_ON;
			set_flash_status(flash->flash_status);
			
			repaint_screen_lines(0, 176);
			break;
		};
				
		case GESTURE_SWIPE_RIGHT: 	//	свайп направо
		case GESTURE_SWIPE_LEFT: {	// справа налево
	
			if ( get_left_side_menu_active()){
					
					// подсветка на установленную в настройках
					set_backlight_percent(backlight_percent[get_backlight_value()]);
					set_backlight_state(1);
					
					// запускаем dispatch_left_side_menu с параметром param в результате произойдет запуск соответствующего бокового экрана
					// при этом произойдет выгрузка данных текущего приложения и его деактивация.
					void* show_f = get_ptr_show_menu_func();
					dispatch_left_side_menu(param);
										
					if ( get_ptr_show_menu_func() == show_f ){
						// если dispatch_left_side_menu отработал безуспешно (листать некуда) то в show_menu_func по прежнему будет 
						// содержаться наша функция show_calend_screen, тогда просто игнорируем этот жест
						
						// если выход не произошел вернуть подсветку как было
						//set_flash_status(flash->flash_status);
						return 0;
					}
					
					
					//	если dispatch_left_side_menu отработал, то завершаем наше приложение, т.к. данные экрана уже выгрузились
					// на этом этапе уже выполняется новый экран (тот куда свайпнули)
					
					Elf_proc_* proc = get_proc_by_addr(main);
					proc->ret_f = NULL;
					
					elf_finish(main);	//	выгрузить Elf из памяти
					return 0;
				} else { 			//	если запуск не из быстрого меню, обрабатываем свайпы по отдельности
					switch (gest->gesture){
						case GESTURE_SWIPE_RIGHT: {	//	свайп направо
							flash->flash_status = FLASH_STATUS_OFF;
							set_flash_status(flash->flash_status);
							//repaint_screen_lines(0, 176);
							return show_menu_animate(flash->ret_f, (int) main, ANIMATE_RIGHT);	
							break;
						}
						case GESTURE_SWIPE_LEFT: {	// справа налево
							//	действие при запуске из меню и дальнейший свайп влево
							
							
							break;
						}
					} /// switch (gest->gesture)
				}

			break;
		};	//	case GESTURE_SWIPE_LEFT:
		
		
		
		
		case GESTURE_SWIPE_UP: {	// свайп вверх
			
			break;
		};
		case GESTURE_SWIPE_DOWN: {	// свайп вниз
			
			break;
		};		
		default:{	// что-то пошло не так...
			break;
		};		
		
	}
	
	return result;
};