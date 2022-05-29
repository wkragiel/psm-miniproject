#include "lcd.h"

static int lcd_print(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(lcd_print, NULL, _FDEV_SETUP_WRITE);

static int lcd_print(char c, FILE *stream){
	lcd_write_data(c);
	return 0;
}


void lcdinit(void){
	
	LCD_PORT_DIR = 0xff;
	LCD_PORT = 0x00;
	_delay_ms(50);
	
	stdout = &mystdout;
	
	LCD_PORT &= ~(1<<LCD_RS);
	
	for(uint8_t i=0 ;i<3 ; i++){
		LCD_PORT |= 1<<LCD_E;
		LCD_PORT = (LCD_PORT & 0x0f) | 0x30;
		_delay_us(1);
		LCD_PORT &= ~(1<<LCD_E);
		_delay_ms(5);
	}
	
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | 0x20;
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);
	//0101000 -> 0x28
	lcd_write_command(0x28);
	//1100 -> 0x0c
	lcd_write_command(0x0c);
	//110 -> 0x06
	lcd_write_command(0x06);
	lcd_clear();
}

void lcd_write_command(uint8_t data){
	LCD_PORT &= ~(1<<LCD_RS);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | (data & 0xf0);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | ((data & 0x0f)<<4);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);	
}

void lcd_clear(void){
	lcd_write_command(1);
	_delay_ms(2);
}

void lcd_write_data(uint8_t data){
	LCD_PORT |= (1<<LCD_RS);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | (data & 0xf0);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | ((data & 0x0f)<<4);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);	
}

void lcd_set_xy(uint8_t r, uint8_t k){
	lcd_write_command(0x80 | (0x40 * r + k));
}

void lcd_write_text_xy(uint8_t r, uint8_t k, char *text){
	lcd_set_xy(r,k);
	while(*text!=0){
		lcd_write_data( (uint8_t)*text);
		text++;
	}
}

void lcd_write_text_P(const char *text){
	register uint8_t ch;
	while( (ch = pgm_read_byte(text++) )){
		lcd_write_data(ch);
	}
}
	
