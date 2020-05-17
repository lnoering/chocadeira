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
        - PINO (3).
    - DISPLAY
        - PINO (7) - rs
        - PINO (6) - enable
        - PINO (5) - d0
        - PINO (4) - d1
        - PINO (9) - d2 
        - PINO (8) - d3
    - Sensor de temperatura (BME280) i2C
        - PINO (19) - SCL
        - PINO (18) - SDA
    - Real Time Clock (DS3231M) i2C
        - PINO (19) - SCL
        - PINO (18) - SDA
    - Sensor de água
        - PINO (17)
    - Saída para acionar resistência dentro da água.
        - PINO (11)
    - Botões Menu
        - PINO (12) - OK
        - PINO (13) - LEFT
        - PINO (10) - RIGHT 
    - Detect Zero Cross
        - PINO (2)
    - Buzzer para Alarme
        - PINO (16)
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