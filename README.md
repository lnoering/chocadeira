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