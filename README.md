Matriz de LED com Raspberry Pi Pico W

Este projeto utiliza um Raspberry Pi Pico W para controlar uma matriz de LEDs WS2812B, permitindo exibir números de 0 a 9. O sistema também inclui um buzzer para efeitos sonoros e botões para interação.

📌 Funcionalidades

1. Exibição de Números na Matriz de LEDs

Representação de números de 0 a 9 utilizando uma matriz 5x5 de LEDs WS2812B.

Conversão de padrões numéricos em cores RGB.

Controle da intensidade dos LEDs.

2. Controle de Entrada

Botão A (GPIO 5): Avança para o próximo número.

Botão B (GPIO 6): Retorna ao número anterior.

Botão J (GPIO 22): Interação adicional (pode ser configurado para outras funções).

3. Indicação Sonora

Buzzer (GPIO 10 e 21): Emite um som ao pressionar os botões.

Frequência do som ajustável.

4. Controle via PIO

Uso do PIO (Programável I/O) para comunicação eficiente com a matriz de LEDs.

Escrita assíncrona dos valores RGB nos LEDs.

5. Gestão de Estados e Debounce

Verificação do tempo entre pressionamentos dos botões para evitar leituras duplicadas.

Registro de última ação para controle preciso da exibição.

🚀 Uso

Este projeto permite exibir números de 0 a 9 em uma matriz de LEDs WS2812B, utilizando botões para alternar entre os números e um buzzer para indicar interações.

📖 Referências

Documentação do Raspberry Pi Pico

SDK do Raspberry Pi Pico

🔗 Repositório

Código-fonte no GitHub

Criado por Valter Machado Silva Filho

