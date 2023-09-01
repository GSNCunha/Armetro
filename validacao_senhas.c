#include <MKL25Z4.h>
#include "validacao_senhas.h"
#include "controle_motor.h"
#include "lcdio.h"
#include "teclado.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char senhaAdm[] = "082479";
char senhaOp1[] = "126963";
char senhaOp2[] = "082479";

double parametros[3][4] =
	{{1300, 100, 20, 60},
	{2100, 130, 20, 70},
	{1700, 100, 30, 63}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em km/h.

int nr_digitados = 0;
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
const char* validacao;	
char admLogin = 0;
char estado[] = "inicio"; 
int portas_abertas = 0;

int ida = 1;

void lerSenha(){
	for(nr_digitados = 0; nr_digitados < 6; nr_digitados++){
		lerTeclado();
		send_data('*');
		if(nr_digitados <= 5){
			senha[nr_digitados] = tecla;
		}
		if(nr_digitados == 5){
			if(!strcmp(senha,senhaAdm)){
				strcpy(estado, "adm");
				telaAdmLogado();
				break;
			}else if(!strcmp(senha,senhaOp1)){
				telaSenhaCorreta();
				strcpy(estado, "logado");
				break;
			}else if(!strcmp(senha,senhaOp2)){
				telaSenhaCorreta();
				strcpy(estado, "logado");
				break;
			}else{
				nr_digitados = 0;
				telaSenhaIncorreta();
				limpa_reseta_cursor();
				strcpy(estado, "inicio");
				break;
			}
		}
	}
}

void edicaoParametros(){
	telaEditarParametros();
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4'){
		lerTeclado();
	}
	if(tecla == '1'){
		edicaoDist();
	}
	if(tecla == '2'){
		edicaoTempo();
	}
	if(tecla == '3'){
		edicaoVel();
	}
	if(tecla == '4'){
		strcpy(estado, "inicio");
	}
}

void edicaoVel(){
	telaEditarVel();
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	if(tecla == '1'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[0][3] = atoi(novaVel);
		atraso(1, 's');
	}
	else if(tecla == '2'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[1][3] = atoi(novaVel);
	}
	else if(tecla == '3'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[2][3] = atoi(novaVel);
	}
}

void edicaoDist(){
	telaEditarDist();
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	if(tecla == '1'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i=0; i<4; i++){
			novaDist[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[0][0] = atoi(novaDist);
	}
	else if(tecla == '2'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i=0; i<4; i++){
			novaDist[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[1][0] = atoi(novaDist);
	}
	else if(tecla == '3'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i=0; i<4; i++){
			novaDist[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[2][0] = atoi(novaDist);
	}
}

void edicaoTempo(){
	
	tecla = 0;
	telaEditarTempos();
	while(tecla != '1' && tecla != '2'){
		lerTeclado();
	}
	if(tecla == '1'){
		edicaoTempoViagem();
	}else if(tecla == '2'){
		edicaoTempoParada();
	}
}

void edicaoTempoParada(){
	telaEditarTempoParada();
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	
	telaTempoParada();
	
	if(tecla == '1'){
		char novoTempoParada[3];
		novoTempoParada[2] = '\0';
		for(int i=0; i<2; i++){
			novoTempoParada[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[0][2] = atoi(novoTempoParada);
		
	}else if(tecla == '2'){
		char novoTempoParada[3];
		novoTempoParada[2] = '\0';
		for(int i=0; i<2; i++){
			novoTempoParada[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[1][2] = atoi(novoTempoParada);
		
	}else if(tecla == '3'){
		char novoTempoParada[3];
		novoTempoParada[2] = '\0';
		for(int i=0; i<2; i++){
			novoTempoParada[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[2][2] = atoi( novoTempoParada);
	}
}

void edicaoTempoViagem(){
	telaEditarTempoViagem();
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	telaTempoViagem();
	if(tecla == '1'){
		char novoTempoViagem[4];
		novoTempoViagem[3] = '\0';
		for(int i=0; i<3; i++){
			novoTempoViagem[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[0][1] = atoi(novoTempoViagem);
	}else if(tecla == '2'){
		char novoTempoViagem[4];
		novoTempoViagem[3] = '\0';
		for(int i=0; i<3; i++){
			novoTempoViagem[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[1][1] = atoi(novoTempoViagem);
	}else if(tecla == '3'){
		char novoTempoViagem[4];
		novoTempoViagem[3] = '\0';
		for(int i=0; i<3; i++){
			novoTempoViagem[i] = lerTeclado();
			send_data(tecla);
		}	
		parametros[2][1] = atoi(novoTempoViagem);
	}
}

void selecaoModoMaquinista(){
	telaModosMaquinista();
	tecla = 0;
	while(tecla != '1' && tecla != '2'){
		lerTeclado();
	}
	if(tecla == '1'){
		strcpy(estado, "modoAuto");
	}else if(tecla == '2'){
		strcpy(estado, "modoManual");
	}
}

void abrePortasEspera(){
	PORTA_PCR5 |= ((1<< 8) + 3) | (10 << 16); //ativa interrupção na coluna dos botoes da porta, teclas A e B
	portas_abertas=1;
	if(portas_abertas == 1){
		setup_PIT0();
		GPIOC_PSOR |= (1<<10 | 1<<11);
	}
	while(portas_abertas==1);
	GPIOC_PCOR |= (1<<10 | 1<<11);
	PORTA_PCR5 &= ~(10 << 16);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void realizar_viagem(int estacao){ //
			
			abrePortasEspera();
			atraso(5, 's');
			if(estacao == 3){
				atraso(10, 's');
			}
			estacao--;
			float vel = (parametros[estacao][0])/((parametros[estacao][1])-5);
			int duracao_viagem = parametros[estacao][1];
			char str_duracao_viagem [10];
	
	//rampa subida:
			for(int i=1;i <6; i++){
				pwm_motor(vel*i);
				atraso(1, 's');
				duracao_viagem--;
				tostring(str_duracao_viagem, duracao_viagem);
				limpa_reseta_cursor();
				send_string("DURACAO VIAGEM:");
				proxima_linha();
				send_string(str_duracao_viagem);
			}
			//vel cte:
			for(int i = 0; i < (parametros[estacao][1]-15); i++){
				atraso(1, 's');
				duracao_viagem--;
				tostring(str_duracao_viagem, duracao_viagem);
				limpa_reseta_cursor();
				send_string("DURACAO VIAGEM:");
				proxima_linha();
				send_string(str_duracao_viagem);
			}
			limpa_reseta_cursor();
			send_string("CHEGANDO NA EST.:");
			proxima_linha();
			if(estacao+1 ==1){
				if(ida){
					send_string("ESTACAO 2");
				}else{
					send_string("ESTACAO 1");
				}
			}else if(estacao+1 ==2){
				if(ida){
					send_string("ESTACAO 3");
				}else{
					send_string("ESTACAO 2");
				}
			}else if(estacao+1 ==3){
				if(ida){
					send_string("FIM DA LINHA");
				}else{
					send_string("ESTACAO 3");
				}
			}
			atraso(5, 's');
			for(int i=4;i>=0; i--){
				pwm_motor(vel*i);
				atraso(1, 's');
			}

}

void modoAuto(){
	char estado_viagem[16];
	strcpy(estado_viagem, "estacao_1"); 
	//int ida = 1;
	while(1){
		
		if (strcmp(estado_viagem, "estacao_1") == 0){ //pede senha pro maquinista se logar
			//faço ele ir pra parada 1
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 1");
			ida = 1;
			realizar_viagem(1);
			strcpy(estado_viagem, "estacao_2");
		}else if (strcmp(estado, "estacao_2") == 0){
			//faço ele ir pra parada 2
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 2");
			if(ida == 1){
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_3");
			}else{
				realizar_viagem(1);
				strcpy(estado_viagem, "estacao_1");
			}
		}else if (strcmp(estado, "estacao_3") == 0){
			//faço ele ir pra parada 3
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 3");
			
			if(ida == 1){
				realizar_viagem(3);
				strcpy(estado_viagem, "fim_da_linha");
			}else{
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_2");
			}
		}else if (strcmp(estado, "fim_da_linha") == 0){
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("FIM DA LINHA");
			ida = 0;
			realizar_viagem(3);
			strcpy(estado_viagem, "estacao_3");
			//
		}
	}
}


