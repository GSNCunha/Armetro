#ifndef LCDIO_H_

#define LCDIO_H_

void trigger_EN(void);

void send_data(char _data_);

void send_command(char command);

void send_string(char *string);

void limpa_reseta_cursor(void);

void config_lcd_padrao(void);

void proxima_linha(void);

//////TELAS///////

	void telaArmMetro();
	
	void telaSenhaCorreta();
		

#endif