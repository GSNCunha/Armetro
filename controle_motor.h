#ifndef CONTROLE_MOTOR_H
#define CONTROLE_MOTOR_H


static volatile float distancia;
static volatile float tempoFaltando;

static volatile float tempoEntrePulsos;

static volatile float pulsos;
static volatile float rotacoes_80kmh;
static volatile float rotacoes_0kmh;
static volatile int motor_calibrado;
static volatile float velocidadeAtual;

void pwm_motor(float velocidade_desejada);
void configura_motor();
void calibra_motor();
float km_hParaGiros_s(float km_h);
float km_hParaGiros_h(float km_h);
void RTC_timer_configura();
void RTC_timer_reseta();
#endif