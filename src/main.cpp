#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ezTime.h>
#include <ArduinoJson.h>
// Prototipo das funções
void conexaoWifi();
void reconexaoWifi();
void iniLCD();
void defineDados();
void monitorS();
void lcdApi();
void anoInteiro();
// CONEXAO WI-FI
const char *SSID = "SENAI EDUCACIONAL";
const char *SENHA = "info@134";
// TEMPO DE CONEXAO E TEMPO RECONEXAO
const unsigned long tempoConexao = 10000;
const unsigned long tempoReconexao = 10000;
// ENDEREÇO
const String enderecoApi = "https://wttr.in/Sao+Caetano+do+Sul?lang=pt&format=";
// CRIAÇÃO DE VARIAVEIS
float temperatura, sensacaoTermica;
int umidade, pressao;
String condicao, nascerSol;
int diasRestantes;
int dia, mes, ano;
int minutos, segundos, hora, formato;
bool am, pm;
// DEFINIÇÃO DO LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
// função diferente do void(que so recebe/faz nao revolver
String consultaApi(String codigo);
// MILLIS
unsigned long tempoAnterior = 0;
const unsigned long intervalo = 1000;

//==========================================//
bool reserva = true;
//==========================================//
int hora_anterior;
int minutos_anterior;
int segundos_anterior;
float temperatura_anterior;
float sensacaoTermica_anterior;
int umidade_anterior;
int am_anterior;
int dia_anterior;
int mes_anterior;
int ano_anterior;
int pm_anterior;
// DEFINIÇÃO DA ZONA
Timezone sp;

void setup()
{
  Serial.begin(115200);
  conexaoWifi();

  waitForSync();

  iniLCD();

  sp.setLocation("America/Sao_Paulo");
}

void loop()
{
  // RECONECTA O WIFI
  reconexaoWifi();

  // DEFINIÇÃO DE DADOS
  defineDados();
  // MONITOR SERIAL
  monitorS();
  // LCD
  lcdApi();
}

void conexaoWifi()
{
  // conexão wifi
  Serial.printf("Conectando ao wifi: %s", SSID); // permite que misture texto com valor de variaveis|| %s substitui o valor pelo oque ta depois da virgula

  WiFi.begin(SSID, SENHA); // ele vai iniciar o wifi da pl  aca, e vai conectar na rede

  unsigned long tempoWifi = millis();

  // codigo pra esperar conectar
  while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi Conectado Com Sucesso!");
    Serial.print("Endereço De IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Falha Ao Se Conectar");
  }
}

void reconexaoWifi()
{
  unsigned long tempoAtual = millis();
  static unsigned long tempoUltimaConexao = 0;

  if (tempoAtual - tempoUltimaConexao > tempoReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexão Wifi Perdida");
      conexaoWifi();
    }
    tempoUltimaConexao = tempoAtual;
  }
}

// como consultar a api
String consultaApi(String codigo)
{
  HTTPClient http; // cria o http

  http.begin(enderecoApi + codigo);
  int codigoHTTP = http.GET();
  String resposta = "";

  if (codigoHTTP == 200)
  {
    resposta = http.getString();
  }

  else
  {
    Serial.println("Deu erro. Codigo :" + String(codigoHTTP));
  }

  http.end();
  return resposta;
}

void lcdApi()
{

  if (reserva)
  {
    lcd.setCursor(2, 0);
    lcd.print(temperatura, 1);
    lcd.print(" ");
    lcd.setCursor(2, 1);
    lcd.print(sensacaoTermica, 1);
    lcd.print(" ");
    lcd.setCursor(2, 2);
    lcd.print(umidade);
    lcd.print(" ");
    lcd.setCursor(3, 3);
    lcd.print(am);
    lcd.print(" ");
    //==========================================//
    lcd.setCursor(10, 0);
    lcd.print(dia);
    lcd.setCursor(10, 1);
    lcd.print(mes);
    lcd.setCursor(10, 2);
    lcd.print(ano);
    lcd.setCursor(11, 3);
    lcd.print(pm);
    //==========================================//
    lcd.setCursor(17, 0);
    lcd.print(segundos);
    lcd.setCursor(17, 1);
    lcd.print(minutos);
    lcd.setCursor(17, 2);
    lcd.print(hora);

    reserva = false;
  }

  if (temperatura != temperatura_anterior)
  {
    lcd.setCursor(2, 0);
    lcd.print(temperatura, 1);
    lcd.print(" ");
  }
  if (sensacaoTermica != sensacaoTermica_anterior)
  {
    lcd.setCursor(2, 1);
    lcd.print(sensacaoTermica, 1);
    lcd.print(" ");
  }
  if (umidade != umidade_anterior)
  {
    lcd.setCursor(2, 2);
    lcd.print(umidade);
    lcd.print(" ");
  }
  if (am != am_anterior)
  {
    lcd.setCursor(3, 3);
    lcd.print(am);
    lcd.print(" ");
  }
  //==========================================//

  if (dia != dia_anterior)
  {
    lcd.setCursor(10, 0);
    lcd.print(dia);
    dia_anterior = dia;
  }
  if (mes != mes_anterior)
  {
    lcd.setCursor(10, 1);
    lcd.print(mes);
    mes_anterior = mes;
  }
  if (ano != ano_anterior)
  {
    lcd.setCursor(10, 2);
    lcd.print(ano);
    ano_anterior = ano;
  }
  if (pm != pm_anterior)
  {
    lcd.setCursor(11, 3);
    lcd.print(pm);
    pm_anterior = pm;
  }

  //==========================================//

  if (segundos != segundos_anterior)
  {
    lcd.setCursor(17, 0);
    lcd.print(segundos);
    segundos_anterior = segundos;
  }
  if (minutos != minutos_anterior)
  {
    lcd.setCursor(17, 1);
    lcd.print(minutos);
    minutos_anterior = minutos;
  }
  if (hora != hora_anterior)
  {
    lcd.setCursor(17, 2);
    lcd.print(hora);
    hora_anterior = hora;
  }
}

void monitorS()
{
  JsonDocument doc;

  doc["temperatura"] = temperatura; // dentro do doc cria um espaço chamado tempo e guarda esse valor ai pra mim
  doc["sensacao"] = sensacaoTermica;
  doc["umidade"] = umidade;
  doc["pressao"] = pressao;
  doc["condicao"] = condicao;
  doc["nascer do sol"] = nascerSol;

  // Serial.println(sp.dateTime("d/m/Y H:i:s")); // padrao pra dia mes ano, e horas minutos e segundos

  doc["dia"] = dia;
  doc["mes"] = mes;
  doc["ano"] = ano;
  //==========================================//
  doc["segundos"] = segundos;
  doc["minutos"] = minutos;
  doc["hora"] = hora;
  doc["formato"] = formato;
  //================================================//
  doc["manha"] = am;
  doc["tarde"] = pm;

  Serial.printf("temp: %f", temperatura);
  Serial.println("");
  Serial.printf("sensacao: %f", sensacaoTermica);
  Serial.println("");
  Serial.printf("umidade: %i", umidade);
  Serial.println("");
  Serial.printf("pressao: %i", pressao);
  Serial.println("");
  Serial.println("condicao: " + condicao);
  Serial.println("");
  Serial.println("nascer sol: " + nascerSol);
  Serial.println("");
  Serial.println("-----------------");
}

void defineDados()
{
  temperatura = consultaApi("%t").toFloat();
  sensacaoTermica = consultaApi("%f").toFloat();
  // dados numericos inteiro
  umidade = consultaApi("%h").toInt();
  pressao = consultaApi("%P").toInt();
  // dados sobre o float
  condicao = consultaApi("%C");
  nascerSol = consultaApi("%S");
  //================================================//
  dia = sp.day();
  mes = sp.month();
  ano = sp.year();
  //================================================//
  am = sp.isAM();
  pm = sp.isPM();
  //================================================//
  segundos = sp.second();
  minutos = sp.minute();
  hora = sp.hour();
}

void iniLCD()

{
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.setCursor(0, 1);
  lcd.print("S: ");
  lcd.setCursor(0, 2);
  lcd.print("U: ");
  lcd.setCursor(0, 3);
  lcd.print("am: ");
  //==============================//

  lcd.setCursor(8, 0);
  lcd.print("D: ");
  lcd.setCursor(8, 1);
  lcd.print("M: ");
  lcd.setCursor(8, 2);
  lcd.print("Y: ");
  lcd.setCursor(8, 3);
  lcd.print("pm: ");
  //=============================//

  lcd.setCursor(15, 0);
  lcd.print("s: ");
  lcd.setCursor(15, 1);
  lcd.print("m: ");
  lcd.setCursor(15, 2);
  lcd.print("h: ");
}
