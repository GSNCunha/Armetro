#ifndef VALIDACAO_SENHAS_H_
#define VALIDACAO_SENHAS_H_

int nr_digitados;
char senha[7];
const char* validacao;
char admLogin;

void lendo_senha(char tecla);

const char* validar_senha(void);

int resultado_validacao(void);

void digitarSenha(char teclaInterrupcao);

#endif /* VALIDACAO_SENHAS_H_ */