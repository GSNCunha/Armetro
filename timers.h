#ifndef TIMERS_H_

#define TIMERS_H_

void delay_s(int segundos);

void delay_ms(int miliSegundos);

void atraso(int num, char param);

void setup_PIT0();
void setup_PIT1();
void desliga_PIT0();
void desliga_PIT1();

#endif