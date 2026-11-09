# Estação Meteorológica e Relógio com ESP32 e LCD 20x4 🌤️🕰️

Este projeto utiliza um **ESP32** para monitorar o clima e o tempo em tempo real. Ele se conecta à internet via Wi-Fi, consome dados meteorológicos da API pública [wttr.in](https://wttr.in/) (configurado para São Caetano do Sul) e obtém a data/hora exata via NTP, exibindo todas as informações em um display **LCD I2C 20x4** e no **Monitor Serial**.

## 🚀 Funcionalidades

- **Conexão Wi-Fi Automática:** Conecta à rede e tenta reconectar automaticamente em caso de queda.
- **Relógio em Tempo Real (NTP):** Sincroniza a data e hora via internet com fuso horário ajustável (padrão: `America/Sao_Paulo`).
- **Monitoramento do Clima:** Consulta dados como Temperatura, Sensação Térmica, Umidade, Pressão, Condições Climáticas e horário do Nascer do Sol.
- **Display LCD 20x4:** Interface gráfica simples informando Temperatura, Sensação Térmica, Umidade, Data (Dia/Mês/Ano), Horário (Hora:Minuto:Segundo) e indicadores AM/PM.
- **Saída Serial JSON:** Envia os dados estruturados em formato JSON para fácil integração com outros sistemas ou leitura no Monitor Serial.

## 🛠️ Hardware Necessário

- 1x Placa ESP32
- 1x Display LCD 20x4 com módulo I2C (Endereço padrão: `0x27`)
- Jumpers para conexão

### Conexões Padrão (ESP32 -> LCD I2C)
* **GND** -> GND
* **VCC** -> VIN ou 5V (dependendo do módulo LCD)
* **SDA** -> GPIO 21
* **SCL** -> GPIO 22

## 📚 Bibliotecas Utilizadas

Para compilar este código, você precisará instalar as seguintes bibliotecas na Arduino IDE (via *Library Manager*):

- `WiFi.h` (Nativa do pacote ESP32)
- `HTTPClient.h` (Nativa do pacote ESP32)
- [LiquidCrystal I2C](https://github.com/johnrickman/LiquidCrystal_I2C) por Frank de Brabander
- [ezTime](https://github.com/ropg/ezTime) por Rop Gonggrijp (Para sincronização NTP e Fuso Horário)
- [ArduinoJson](https://arduinojson.org/) por Benoit Blanchon (Para formatação dos dados no monitor serial)

## ⚙️ Configuração e Uso

1. Abra o arquivo do projeto na **Arduino IDE** (ou VSCode/PlatformIO).
2. Modifique as credenciais de Wi-Fi no código para as da sua rede:
   ```cpp
   const char *SSID = "NOME_DA_SUA_REDE";
   const char *SENHA = "SUA_SENHA";
