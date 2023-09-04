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

int nr_digitados = 0; //variavel q armazena o numero de caracteres digitados na senha
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'}; //variavel q armazenamos a senha para dps comparar
char senhaAdm[] = "082479";
char senhaOp1[] = "126963";
char senhaOp2[] = "082479";

char estado[16] = "inicio"; //variavel q armazena em que interface o usuario esta. começa como "inicio" para entrar direto na tela padrao de login

void lerSenha(){
	for(nr_digitados = 0; nr_digitados < 6; nr_digitados++){ //enquanto tiver menos q 6 caracteres le a senha
		lerTeclado();
		send_data('*');
		if(nr_digitados <= 5){
			senha[nr_digitados] = tecla;
		}
		if(nr_digitados == 5){
			if(!strcmp(senha,senhaAdm)){ //se a senha digitada bater com a do adm, faz o estado atual virar "adm" e volta para o loop principal, onde o codigo ira cair dentro do menu de adm
				strcpy(estado, "adm");
				telaAdmLogado();
				break;
			}else if(!strcmp(senha,senhaOp1)){ //mesma ideia do estado adm, porem pra se logar como maquinista
				telaSenhaCorreta();
				strcpy(estado, "logado");
				break;
			}else if(!strcmp(senha,senhaOp2)){ //mesma ideia do estado adm, porem pra se logar como maquinista
				telaSenhaCorreta();
				strcpy(estado, "logado");
				break;
			}else{ 		//se a senha tiver incorreta, volta pro estado de inicio, ou seja, menu principal
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
	telaEditarParametros(); //exibe a tela de edição de parametros 
	tecla = 0; //seta a ultima tecla apertada como nula
	while(tecla != '1' && tecla != '2' && tecla != '3' && tecla != '4'){
		lerTeclado(); //espera até q o usuario digite uma tecla de 1 até 4
	}
	if(tecla == '1'){
		edicaoDist(); //1 vai para edição de distancia 
	}
	if(tecla == '2'){
		edicaoTempo();//2 vai para edição de tempos 
	}
	if(tecla == '3'){
		edicaoVel();//3 vai para edição de vel maxima  
	}
	if(tecla == '4'){
		strcpy(estado, "inicio"); //4 faz logout e retorna ao menu principal
	}
}

void edicaoVel(){
	telaEditarVel(); //printa no lcd a tela de edicao de vel maxima
	tecla = 0;
	while(tecla != '1' && tecla != '2' && tecla != '3'){
		lerTeclado(); //espera q o usuario digite uma opção de 1 a 3
	}
	if(tecla == '1'){
		telaNovaVel(); //se for 1, pede p ler a nova velocidade
		char novaVel[3];
		novaVel[2] = '\0'; //ultimo caractere da string com o sinalizador de final de string
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[0][3] = atoi(novaVel); //transforma a nova velocidade de string pra inteiro
	}
	else if(tecla == '2'){
		telaNovaVel(); //se for 2, pede p ler a nova velocidade maxima da estação 2
		char novaVel[3];
		novaVel[2] = '\0'; //ultimo caractere da string com o sinalizador de final de string
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado();
			send_data(tecla);
		}
		parametros[1][3] = atoi(novaVel);
	}
	else if(tecla == '3'){
		telaNovaVel(); //se for 3, pede p ler a nova velocidade maxima da estação 3
		char novaVel[3]; //a nova velocidade terá 2 caracteres sempre
		novaVel[2] = '\0'; //ultimo caractere da string com o sinalizador de final de string
		for(int i=0; i<2; i++){
			novaVel[i] = lerTeclado(); //le os 2 caracteres do teclado
			send_data(tecla);
		}
		parametros[2][3] = atoi(novaVel);
	}
}

void edicaoDist(){ //segue a mesma estrutura das funções anteriores, porem agora pra distancia
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

void edicaoTempo(){ //pede pro usuario selecionar se é tempo de viagem ou parada q quer editar
	
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

void edicaoTempoParada(){ //segue a mesma estrutura das funções anteriores, porem agora pra tempo
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

void edicaoTempoViagem(){ //segue a mesma estrutura das funções anteriores, porem agora pra tempo de viagem
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

void selecaoModoMaquinista(){ //pede pro usuario digitar se quer modo manual ou automático
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