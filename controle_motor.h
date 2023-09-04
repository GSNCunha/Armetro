#ifndef CONTROLE_MOTOR_H
#define CONTROLE_MOTOR_H

extern float velocidadeAtual;

void pwm_motor(float velocidade_desejada);
void configura_motor();
void calibra_motor();
#endif