#include "validacao_senhas.h"
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
	{1700, 100, 30, 63}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em metros por segundo.

int nr_digitados = 0;
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
const char* validacao;	
char admLogin = 0;
char estado[] = "inicio";

void lerSenha(){
	for(nr_digitados = 0; nr_digitados < 6; nr_digitados++){
		lerTeclado();
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
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4'){
		lerTeclado();
	}
	if(tecla == '1'){
		strcpy(estado, "editdist");
	}
	if(tecla == '2'){
		strcpy(estado, "edittempo");
	}
	if(tecla == '3'){
		strcpy(estado, "editvel");
	}
	if(tecla == '4'){
		strcpy(estado, "inicio");
	}
}

void edicaoVel(){
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	if(tecla == '1'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i; i<2; i++){
			novaVel[i] = lerTeclado();
		}
		parametros[0][3] = atoi(novaVel);
	}
	else if(tecla == '2'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i; i<2; i++){
			novaVel[i] = lerTeclado();
		}
		parametros[1][3] = atoi(novaVel);
	}
	else if(tecla == '3'){
		telaNovaVel();
		char novaVel[3];
		novaVel[2] = '\0';
		for(int i; i<2; i++){
			novaVel[i] = lerTeclado();
		}
		parametros[2][3] = atoi(novaVel);
	}
}

void edicaoDist(){
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado();
	}
	if(tecla == '1'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i; i<4; i++){
			novaDist[i] = lerTeclado();
		}
		parametros[0][0] = atoi(novaDist);
	}
	else if(tecla == '2'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i; i<4; i++){
			novaDist[i] = lerTeclado();
		}
		parametros[1][0] = atoi(novaDist);
	}
	else if(tecla == '3'){
		telaNovaDist();
		char novaDist[5];
		novaDist[4] = '\0';
		for(int i; i<4; i++){
			novaDist[i] = lerTeclado();
		}
		parametros[2][0] = atoi(novaDist);
	}
}

void edicaoTempo(){
	

}
