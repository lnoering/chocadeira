## Chocadeira

### Plataforma
    - Arduino Uno

### Objetivo
    Controlar a temperatura, humidade e o tempo para a incubação de ovos.


### Melhorias a serem feitas
    - Criar Menu
    - Criar INPUT para verificar a bandeja de água.
    - Criar controle PID na saída digital que irá acionar o aquecimento.
    - Definir o que deve aparecer nas telas.
    - Criar classe de ALARMES.

### IO
    Obs.: PINOS conforme a placa do arduino.
    - PWM para o controle de temperatura. (PID)
        - PINO (9).
    - DISPLAY
        - PINO (7) - rs
        - PINO (6) - enable
        - PINO (5) - d0
        - PINO (4) - d1
        - PINO (3) - d2
        - PINO (2) - d3
    - Sensor de temperatura (BME280) i2C
        - PINO (4) - SCL
        - PINO (5) - SDA
    - Real Time Clock (DS3231M) i2C
        - PINO (4) - SCL
        - PINO (5) - SDA
    - Sensor de água
        - PINO (3)
    - Saída para acionar resistência dentro da água.
        - PINO (2)
    - Botões Menu
        - PINO (12) - OK
        - PINO (13) - LEFT
        - PINO (11) - RIGHT

### Menu
> "Set Point Temperatura"
-   Setar a temperatura que manterá a saída acionada.

> "Offset Rele"
-   Tempo em segundo para acontecer mudança do status da saída. (Prevenir mundanças consecutivas, para não queimar o motor).

> "Offset Temperatura"
-   Decrementar diretamente da leitura do sensor.

> "Salvar e Sair"
-   Grava na EEPROM e sai do menu.

> "Sair"
-   Apenas sai do Menu.