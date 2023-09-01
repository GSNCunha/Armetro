#ifndef CONTROLE_MOTOR_H
#define CONTROLE_MOTOR_H


extern float distancia;
extern float tempoFaltando;

extern float tempoEntrePulsos;

extern float pulsos;
extern float rotacoes_80kmh;
extern float rotacoes_0kmh;
extern int motor_calibrado;
extern float velocidadeAtual;

void pwm_motor(float velocidade_desejada);
void configura_motor();
void calibra_motor();
float km_hParaGiros_s(float km_h);
float km_hParaGiros_h(float km_h);
void RTC_timer_configura();
void RTC_timer_reseta();
#endif