# Projeto - Interface de Comunicação Serial RP2040

Este projeto implementa um sistema de comunicação serial usando o microcontrolador RP2040 na placa BitDogLab, integrando diferentes periféricos e protocolos de comunicação.

## Funcionalidades

### 1. Controle de LEDs RGB
- **Botão A**: Liga/desliga LED Verde
- **Botão B**: Liga/desliga LED Azul
- LED Vermelho pisca continuamente

### 2. Display OLED SSD1306
- Mostra mensagens de status dos LEDs
- Exibe caracteres recebidos via Serial Monitor
- Comunicação via I2C

### 3. Matriz WS2812 (5x5)
- Exibe números (0-9) em diferentes cores
- Acionada quando números são recebidos via Serial Monitor

### 4. Comunicação Serial
- Recebe caracteres via USB Serial Monitor
- Exibe feedback no display OLED
- Mostra números na matriz de LEDs

## Hardware Necessário
- Placa BitDogLab com RP2040
- Matriz 5x5 LEDs WS2812 (GPIO 7)
- LED RGB (GPIOs 11, 12, 13)
- Display OLED SSD1306 (I2C - GPIO 14 e 15)
- Botões (GPIO 5 e 6)

## Pinagem
| Componente    | GPIO |
| ------------- | ---- |
| LED Verde     | 11   |
| LED Azul      | 12   |
| LED Vermelho  | 13   |
| Botão A       | 5    |
| Botão B       | 6    |
| Matriz WS2812 | 7    |
| I2C SDA       | 14   |
| I2C SCL       | 15   |

## Como Compilar e Executar
1. Configure o ambiente:
```bash
mkdir build
cd build
cmake ..
make
```
2. Carregue o programa no Pico:
- Copie o arquivo `.uf2` gerado para o Pico em modo bootloader
3. Use o Serial Monitor:
- Abra o VS Code
- Conecte à porta COM do Pico
- Use `Ctrl+Shift+P` -> "Serial Monitor"
- Digite números ou letras para testar

## Estrutura do Projeto
```
projeto/
├── CMakeLists.txt
├── TarefaSerial.c
├── matriz_leds.c
├── pio_matrix.pio
└── incre/
    ├── ssd1306.c
    ├── ssd1306.h
    └── font.h
```

## Notas de Implementação
- Usa interrupções para os botões
- Implementa debounce via software
- Comunicação I2C para o display OLED
- Comunicação serial via USB
- Controle de LED RGB via GPIO
- Matriz WS2812 controlada via PIO

## Requisitos do Sistema
- Raspberry Pi Pico SDK
- CMake
- Build tools (`gcc-arm-none-eabi`)
- VS Code com extensão Pico-W-Go

## Como Contribuir
1. Faça um fork do repositório
2. Crie um branch para suas alterações (`git checkout -b minha-feature`)
3. Faça commit das suas mudanças (`git commit -m 'Adicionei nova funcionalidade'`)
4. Faça push para o branch (`git push origin minha-feature`)
5. Abra um Pull Request

## Vídeo Demonstrativo
Confira um vídeo demonstrativo do projeto:
[![Vídeo no YouTube](https://img.youtube.com/vi/NRTx64LAKVc/0.jpg)](https://youtu.be/NRTx64LAKVc)

## Licença
Este projeto é distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.
