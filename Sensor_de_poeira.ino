#include <LiquidCrystal_I2C.h>
//define a quantidade colunas e linhas
int lcdColumns = 16; // 16 colunas
int lcdRows = 2; // 2 linhas

// Define o endereço do display, e seta o parametro de colunas e linhas 
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); //Para encontrar o endereço do display, use código rastreador de I2C
 
  int amostra = 36 ; // Recebe a amostra analogica de dados do sensor GP2Y10 no pino 36
  int IRED = 2; // Seta pino digital para pulso do IRED
 
  // Constantes de tempo para os pulsos do IRED
  int retardo_1 = 280;
  int retardo_2 = 40;
  int retardo_3 = 9680;
 
  // Variaveis auxiliares
  int valor = 0; //Iguala valor coletado no PIN36 à zero no instante inicial
  float ppm = 0; // Iguala Partes por Milhão à zero no instante inicial
  float tensao = 0; //Iguala tensão inicial à zero
  float densidade = 0; //Densidade de particulas
  float ppm_real = 0; //Valor real de PPM
  int i=0; //Zera contador i
  int val_a=ppm_real;
 
  void setup()
  {
     // Inicializa o display
  lcd.init();
  // Liga o LED do display                     
  lcd.backlight();
    pinMode(IRED,OUTPUT); // Seta pino IRED como saída de pulsos
   
    // Imprime saudação inicial no display
    lcd.clear(); //Limpa display
    lcd.setCursor(0,0); //Define posição do bit na coluna e linha
    lcd.print("  DETECTOR  DE  "); //Imprime mensagem formatada
    lcd.setCursor(0,1); //Define posição do bit na coluna e linha
    lcd.print("POEIRA - COULOMB"); //Imprime mensagem formatada
    delay(3000); // Define tempo para a exibição da saudação inicial
  }
 
  void loop()
  {
    i=i+1; // Controle do contador de pulsos
    // Define nível lógico de saída do pino para pulsar IRED onde: LOW = Ativado / HIGH = Desativado
    digitalWrite(IRED,LOW); // LED ativado
    delayMicroseconds(retardo_1); // Retardo de 0,28ms
    valor = analogRead(amostra); // Coleta valor da amostragem recebido no PIN36
    ppm = ppm + valor; // Media ponderada de Vo
    delayMicroseconds(retardo_2); // Retardo de 0,04ms
    digitalWrite(IRED,HIGH); // LED desativado
    delayMicroseconds(retardo_3); // Retardo de 9,68ms
    // retardo_1 + retardo_2 + retardo_3 = 10ms
   
    // Fórmulas matemáticas para cálculo dos valores do sensor GP2Y10
    tensao = ppm/i*0.0049; // Tensão em Volts (media dos valores de ppm obtidos)
    densidade = 0.17*tensao-0.1; // Densidade de particulas em mg/m³
    ppm_real = (tensao-0.0356)*120000; // Concentração de particulas em PPM
    if (ppm_real < 0)
      ppm_real = 0;
    if (densidade < 0 )
      densidade = 0;
    if (densidade > 0.5)
      densidade = 0.5;
   
    // Imprime a tensão em grandeza quantitativa
    lcd.clear(); //Limpa display
    lcd.setCursor(0,0);
    lcd.print("-> TENSAO");
    lcd.setCursor(0,1);
    lcd.print(tensao,3); // Imprime a tensão com 3 casas decimais de precisão
    lcd.print(" V");
    delay(2000);
   
    // Imprime a densidade em grandeza quantitativa.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("-> DENSIDADE");
    lcd.setCursor(0,1);
    lcd.print(densidade,3); // Imprime a densidade com 3 casas decimais de precisão
    lcd.print(" mg x m3");
    delay(2000); 
  }
