#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "include/frames.c"
#include "include/keyPad.c"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define CONTADOR_LED 25
#define PINO_MATRIZ_LED 7
#define BOTAO_A 5
#define BOTAO_B 6
#define LED_R 13


// Definição de pixel GRB
struct pixel_t
{
	uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t LED_da_matriz;

// Declaração do buffer de pixels que formam a matriz.
LED_da_matriz leds[CONTADOR_LED];

// Variáveis para uso da máquina PIO.
PIO maquina_pio;
uint variavel_maquina_de_estado;

//-----PROTÓTIPOS-----
void inicializacao_maquina_pio(uint pino);
void iniciar_pino_gpio();
void atribuir_cor_ao_led(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensidade);
void limpar_o_buffer();
void escrever_no_buffer();
void desenho(int num);
void piscar_led();

//-----VARIÁVEIS-----
static volatile uint32_t last_time = 0;

//-----FUNÇÕES COMPLEMENTARES-----
static void interrupcao(uint gpio, uint32_t events);

// ------MATRIZ-----
int tamanho_matriz = 5;
// Matriz 5x5
uint matrizint[5][5] = {
		{24, 23, 22, 21, 20},
		{15, 16, 17, 18, 19},
		{14, 13, 12, 11, 10},
		{5, 6, 7, 8, 9},
		{4, 3, 2, 1, 0}};

//-----VARIÁVEIS GLOBAIS-----
uint8_t _intensidade_ = 255;
int num = 0;
int BOTAO;

//-----FUNÇÃO PRINCIPAL-----
int main(void)
{
	// Inicializa a máquina PIO para controle da matriz de LEDs.
	inicializacao_maquina_pio(PINO_MATRIZ_LED);
	iniciar_pino_gpio(); // Inicializa os pinos GPIO
	limpar_o_buffer(); // Limpa o buffer de pixels
	_intensidade_ = 255; // Intensidade inicial

	// Configura o botão 0 para interromper a execução e chamar a função gpio_irq_handler quando o botão 0 for pressionado.
	gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
	gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao);

	// Inicia a execução.
	while (true)
	{
		piscar_led();
		desenho(num);
		if (gpio_get(BOTAO_A) == 0)
		{
			BOTAO = BOTAO_A;
		}
		if (gpio_get(BOTAO_B) == 0)
		{
			BOTAO = BOTAO_B;
		}
		escrever_no_buffer();
	}
	return 0;
}

//-----FUNÇÕES COMPLEMENTARES-----
// Inicializa a máquina PIO para controle da matriz de LEDs.
void inicializacao_maquina_pio(uint pino)
{
	uint programa_pio, i;
	// Cria programa PIO.
	programa_pio = pio_add_program(pio0, &ws2818b_program);
	maquina_pio = pio0;

	// Toma posse de uma máquina PIO.
	variavel_maquina_de_estado = pio_claim_unused_sm(maquina_pio, false);
	if (variavel_maquina_de_estado < 0)
	{
		maquina_pio = pio1;
		variavel_maquina_de_estado = pio_claim_unused_sm(maquina_pio, true); // Se nenhuma máquina estiver livre, panic!
	}

	// Inicia programa na máquina PIO obtida.
	ws2818b_program_init(maquina_pio, variavel_maquina_de_estado, programa_pio, pino, 800000.f);

	// Limpa buffer de pixels.
	for (i = 0; i < CONTADOR_LED; ++i)
	{
		leds[i].R = 0;
		leds[i].G = 0;
		leds[i].B = 0;
	}
}

void iniciar_pino_gpio()
{
	// Inicializa os pinos GPIO.
	gpio_init(LED_R);
	gpio_set_dir(LED_R, GPIO_OUT);
	//botoes
	gpio_init(BOTAO_A);
	gpio_set_dir(BOTAO_A, GPIO_IN);
	gpio_pull_up(BOTAO_A);
	gpio_init(BOTAO_B);
	gpio_set_dir(BOTAO_B, GPIO_IN);
	gpio_pull_up(BOTAO_B);
}

// Atribui uma cor RGB a um LED.
void atribuir_cor_ao_led(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b, uint8_t intensidade)
{

	if (intensidade > 255)
		intensidade = 255;
	if (intensidade < 0)
		intensidade = 0;

	// Ajusta os valores de RGB conforme a intensidade escolhida
	leds[indice].R = (r * intensidade);
	leds[indice].G = (g * intensidade);
	leds[indice].B = (b * intensidade);
}

// Limpa o buffer de pixels.
void limpar_o_buffer()
{
	for (uint i = 0; i < CONTADOR_LED; ++i)
	{
		atribuir_cor_ao_led(i, 0, 0, 0, 0);
	}
}
	// Desenha o número escolhido
void desenho(int num)
{ // Desenha o número escolhido
	char(*matriz)[5];
	if (num == 0)
	{
		matriz = matriz_0;
	}
	if (num == 1)
	{
		matriz = matriz_1;
	}
	if (num == 2)
	{
		matriz = matriz_2;
	}
	if (num == 3)
	{
		matriz = matriz_3;
	}
	if (num == 4)
	{
		matriz = matriz_4;
	}
	if (num == 5)
	{
		matriz = matriz_5;
	}
	if (num == 6)
	{
		matriz = matriz_6;
	}
	if (num == 7)
	{
		matriz = matriz_7;
	}
	if (num == 8)
	{
		matriz = matriz_8;
	}
	if (num == 9)
	{
		matriz = matriz_9;
	}
	// Atualiza a matriz de LEDs
	for (int x = 0; x < tamanho_matriz; x++)
	{
		for (int y = 0; y < tamanho_matriz; y++)
		{  // R, G, B, Y, P
			if (matriz[x][y] == 'R')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 0, 0, _intensidade_);
			}
			if (matriz[x][y] == 'G')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 255, 0, _intensidade_);
			}
			if (matriz[x][y] == 'B')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 0, 255, _intensidade_);
			}
			if (matriz[x][y] == 'Y')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 255, 0, _intensidade_);
			}
			if (matriz[x][y] == 'P')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 0, 255, _intensidade_);
			}
			if (matriz[x][y] == 'C')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 255, 255, _intensidade_);
			}
			if (matriz[x][y] == 'W')
			{
				atribuir_cor_ao_led(matrizint[x][y], 255, 255, 255, _intensidade_);
			}
			if (matriz[x][y] == '*')
			{
				atribuir_cor_ao_led(matrizint[x][y], 0, 0, 0, _intensidade_);
			}
		}
		// Atualiza a matriz de LEDs e espera um tempo antes de mudar a cor
		escrever_no_buffer();
		sleep_ms(10); // 300ms entre as mudanças de cor
	}
}

// Escreve os dados do buffer nos LEDs.
void escrever_no_buffer()
{
	// Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
	for (uint i = 0; i < CONTADOR_LED; ++i)
	{
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].G);
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].R);
		pio_sm_put_blocking(maquina_pio, variavel_maquina_de_estado, leds[i].B);
	}
	sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}
void interrupcao(uint gpio, uint32_t events)
{

	// Obtém o tempo atual em microssegundos
	uint32_t current_time = to_us_since_boot(get_absolute_time());
	// Verifica se passou tempo suficiente desde o último evento
	if (current_time - last_time > 500000) // 500 ms de debouncing
	{
		last_time = current_time; // Atualiza o tempo do último evento
		// Verifica qual botão foi pressionado	

		//
		if (gpio == BOTAO_A)
		{
			if (num == 9) 
			{
				num = 9;
			}
			else
			{
				num++;
			}
		}
		if (gpio == BOTAO_B)
		{
			if (num == 0)
			{
				num = 0;
			}
			else
			{
				num--;
			}
		}
	}
}
