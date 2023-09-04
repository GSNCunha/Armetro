#include <MKL25Z4.h>
#include "menus.h"
#include "controle_motor.h"
#include "lcdio.h"
#include "teclado.h"
#include "timers.h"
#include "rotinas_viagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nr_digitados = 0;
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
char senhaAdm[] = "082479";
char senhaOp1[] = "126963";
char senhaOp2[] = "082479";
char admLogin = 0;
char estado[16] = "inicio"; 

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