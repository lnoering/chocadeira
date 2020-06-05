<h3 align="center">Chocadeira/Brooder</h1>
<p align="center">
    <img alt="logo-chocadeira" class="avatar rounded-2" height="60" src="/icon.png" width="60">
</p>
<p align="center">
    <a href="https://travis-ci.org/lnoering/chocadeira">
        <img src="https://travis-ci.org/lnoering/chocadeira.svg?branch=master" alt="Build Status">
    </a>
</p>


### Plataforma/Platform
    - Arduino Uno


### Objetivo/Objective
    Controlar a temperatura, humidade e o tempo para a incubação de ovos.
    /
    Control temperature, humidity and time for hatching eggs.


### Funcionalidades/Functionalities
    > Controle de temperatura.
        - PID
        - Detecção da passagem da tensão em 0, para controle PWM. (220volts)
    > Controle da Humidade.
        - PID
        - Detecção da passagem da tensão em 0, para controle PWM. (220volts)
    > Alarme
    > Sensor para verificar se tem água no recipiente.
        - Caso não tenha o controle de Humidade não é acionado.
    > Menu [3 teclas para controle. (UP/OK/DOWN)]


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

> "Configurar Data"
-   Configurar a Data.
> "Configurar Hora"
-   Configurar a Hora.
> "Inicio p/ virar ovos"
-   Setar quantos dias deve esperar para virar os ovos.
> "Qtd viradas p/ dia"
-   Setar a quantidade de vezes q ele deve virar por dia
> "Qtd dias do processo"
-   Setar o total de dias para finalizar o processo.
> "Set Point Temp."
-   Valor desejado para a Temperatura.
> "Set Ki Temperature"
-   Var do controle PID.
> "Set Kp Temperature"
-   Var do controle PID.
> "Set Kd Temperature"
-   Var do controle PID.
> "Set Point Humidity"
-   Valor desejado para a Humidade.
> "Set Ki Humidity"
-   Var do controle PID.
> "Set Kp Humidity"
-   Var do controle PID.
> "Set Kd Humidity"
-   Var do controle PID.
> "Salvar e Sair"
-   Grava na EEPROM e sai do menu.
> "Sair"
-   Sai sem salvar.


### Lista de melhorias/List of improvements
    - Criar Menu
        - Tempo total do processo para eclosão.
        - Tempo para virar os ovos.
    - Definir o que deve aparecer nas telas.
    - Criar classe de ALARMES.
    - Não parar as rotinas de controle quando entrar no menu.