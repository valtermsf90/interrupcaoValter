#ifndef FUNCOES_H
#define FUNCOES_H

void inicializacao_maquina_pio(uint pino);
void escrever_no_buffer();
void limpar_o_buffer();
void atribuir_cor_ao_led(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensidade);
void desenho(int num);

#endif
