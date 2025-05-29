/*******************************************************************************************************************************
Autor............: Equipe Eng Robots
Disciplina.......: APS
Data.............: 05/2025
*******************************************************************************************************************************/


//================================================INCLUINDO AS BIBLIOTECAS============================================
  #include "Ultrasonic.h"
  #include "BluetoothSerial.h"

//================================================INCLUINDO AS FUNÇÕES================================================
  //ADICIONANDO FUNÇÕES PARA OS IR
    void leiturasIR();
    void mostrarIR();
    void estadoIR();

  //ADICIONANDO FUNÇÕES PARA OS 
    void leiturasUltra();
    void mostrarUltra();
    void estadoUltra();

  //ADICIONANDO FUNÇÕES PARA OS MOTORES
    void virar_pra_esquerda();
    void virar_pra_direita();
    void move_pra_frente();
    void dar_re();
    void pararMotores();

//================================================CONFIGURANDO OS PINOS===============================================
  //DEFININDO OS PINO DO SENSOR ULTRASSÔNICO 
    HC_SR04 SensorUF(26,25);             // (Trigger,Echo) Sensor Ultrassônico dá Frente
    HC_SR04 SensorUT(33,32);             // (Trigger,Echo) Sensor Ultrassônico de Trás
    HC_SR04 SensorUD(4,16);              // (Trigger,Echo) Sensor Ultrassônico do LADO DIREITO olhando da rampa frontal
    HC_SR04 SensorUE(17,18);             // (Trigger,Echo) Sensor Ultrassônico do LADO ESQUERDO olhando da rampa frontal

  //DEFININDO OS PINO DA PONTE H
    #define pin1 23
    #define pin2 13
    #define pin3 12
    #define pin4 27

  //DEFININDO OS PINOS DO IR
    #define IRFD 19
    #define IRFC 21
    #define IRFE 22
    #define IRTD 35
    #define IRTE 34

//COR DA LEITURA IR
  #define branco 0
  #define preto 1

//VELOCIDADE DO MOTOR
  #define velocidade 150
  #define velocidade2 80

//DISTANCIA DETECTAÇÃO  
  #define distancia 70

//CONFIGURANDO A COMUNICAÇÃO BLUETOOTH 
  BluetoothSerial serialBLE;

//ADICIONANDO VARIAVEIS DO BLUETOOTH 
  char dados;
  bool conexao;

//ADICIONANDO VARIAVEIS DO IR
  int estadoIRFD, estadoIRFC, estadoIRFE, estadoIRTD, estadoIRTE;

//ADICIONANDO VARIAVEIS DO ULTRASSÔNICO 
  int estadoUF, estadoUT, estadoUD, estadoUE;

void setup() 
{
  //INICIANDO A COMUNICAÇÃO SERIAL E BLUETOOTH 
    Serial.begin(9600);
    serialBLE.begin("EngRobots");

  //CONFIGURAÇÃO MOTORES
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
    pinMode(velocidade, OUTPUT);

  //CONFIGURAÇÃO SENSORES IR
    pinMode(IRFD, INPUT);
    pinMode(IRFC, INPUT);
    pinMode(IRFE, INPUT);
    pinMode(IRTD, INPUT);
    pinMode(IRTE, INPUT);
  
  //INICIANDO MOTORES DESLIGADOS
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
    analogWrite(pin3, 0);
    analogWrite(pin4, 0);
}

void loop() 
{
  //INICIANDO MOTORES DESLIGADOS
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
    analogWrite(pin3, 0);
    analogWrite(pin4, 0);

  //LENDO OS SENSORES
    leiturasIR();
    leiturasUltra();
  
  //MOSTRANDO DADOS DOS SENSORES NO MONTOR SERIAL
    //mostrarIR();
    //mostrarUltra();

  //COMANDOS BLUETOOTH
    conexao = serialBLE.available();
    if (conexao == true)
    {
      dados = serialBLE.read();
      if (dados == 'L')
      {
        while (dados != 'D')
        {
          leiturasUltra();
          estadoUltra();
          leiturasIR();
          estadoIR();
          dados = serialBLE.read();
        }
      }
    }
}

//================================================FUNÇÃO LEITURA IR=====================================================
  void leiturasIR()
  {
    //NOMEANDO E LENDO ESTADOS DOS SENSORES
      estadoIRFD = digitalRead(IRFD);
      estadoIRFC = digitalRead(IRFC);
      estadoIRFE = digitalRead(IRFE);
      estadoIRTD = digitalRead(IRTD);
      estadoIRTE = digitalRead(IRTE);
  }

//================================================FUNÇÃO MOSTRAR IR NO SERIAL===========================================
  void mostrarIR()
  {
    Serial.print("Valor Lido pelo IRFD = ");
    Serial.println(estadoIRFD);
    delay(1000);

    Serial.print("Valor Lido pelo IRFC = ");
    Serial.println(estadoIRFC);
    delay(1000);
    
    Serial.print("Valor Lido pelo IRFE = ");
    Serial.println(estadoIRFE);
    delay(1000);
    
    Serial.print("Valor Lido pelo IRTD = ");
    Serial.println(estadoIRTD);
    delay(1000);
    
    Serial.print("Valor Lido pelo IRTE = ");
    Serial.println(estadoIRTE);
    delay(1000);
  }

//================================================FUNÇÃO ESTADO IR======================================================
  void estadoIR()
  {
    //SE OS SENSORES IR DA FRENTE ESTIVEREM NA LINHA BRANCA
    if ((estadoIRFD == branco) || (estadoIRFC == branco) || (estadoIRFE == branco))
    {
      dar_re();
      delay(200);
      do
      {
        virar_pra_direita();
        leiturasUltra();
      }while(estadoUF <= distancia);    
    }
    else 
    {
      return;
    }
    
    //SE OS SENSORES IR DE TRÁS ESTIVEREM NA LINHA BRANCA
    if ((estadoIRTD == branco) || (estadoIRTE == branco))
    {
      virar_pra_direita();
      delay(200);
      virar_pra_esquerda();
      delay(200);
      move_pra_frente();
    } 
    else
    {
      return;
    }
  }
//================================================FUNÇÃO LEITURA ULTRASSÔNICO===========================================
  void leiturasUltra()
  {
    estadoUF = SensorUF.distance();
    estadoUT = SensorUT.distance();
    estadoUD = SensorUD.distance();
    estadoUE = SensorUE.distance();
  }

//================================================FUNÇÃO MOSTRAR ULTRASSÔNICO===========================================
  void mostrarUltra()
  {
    Serial.print("Valor lido pelo Ultrassônico da Frente = ");
    Serial.print(SensorUF.distance());
    Serial.println(" cm");
    delay(1000);

    Serial.print("Valor lido pelo Ultrassônico de Trás = ");
    Serial.print(SensorUT.distance());
    Serial.println(" cm");
    delay(1000);

    Serial.print("Valor lido pelo Ultrassônico de Lado Direito = ");
    Serial.print(SensorUD.distance());
    Serial.println(" cm");
    delay(1000);

    Serial.print("Valor lido pelo Ultrassônico de Lado Esquerdo = ");
    Serial.print(SensorUE.distance());
    Serial.println(" cm");
    delay(1000);
  }

//================================================FUNÇÃO ESTADO ULTRASSÔNICO============================================
  void estadoUltra() {
  leiturasUltra(); // atualiza os valores dos sensores

  if (estadoUF <= distancia) {
    move_pra_frente();
  }
  else if (estadoUD <= distancia) {
    do
    {
      leiturasUltra();
      virar_pra_esquerda();
    } while (estadoUF > distancia);

  }
  else if (estadoUE <= distancia) {
    do
    {
      leiturasUltra();
      virar_pra_direita();
    } while (estadoUF > distancia);

  }
    else if (estadoUT <= distancia) {
    do
    {
      leiturasUltra();
      virar_pra_direita();
    } while (estadoUF > distancia);
  }
  else {
    pararMotores(); // para se nenhum obstáculo estiver perto
  }
}
//================================================FUNÇÃO MOVER PRA FRENTE===============================================
  void move_pra_frente()
  {
    analogWrite(pin1, velocidade);
    analogWrite(pin2, 0);
    analogWrite(pin3, velocidade);
    analogWrite(pin4, 0);
  }

//=========================================ULTRASSÔNICO=======FUNÇÃO DAR RÉ=========================================================
  void dar_re()
  {
    analogWrite(pin1, 0);
    analogWrite(pin2, velocidade);
    analogWrite(pin3, 0);
    analogWrite(pin4, velocidade);
  }

//================================================FUNÇÃO VIRAR PRA ESQUERDA=============================================
  void virar_pra_esquerda()
  {
    analogWrite(pin1, velocidade);
    analogWrite(pin2, 0);
    analogWrite(pin3, 0);
    analogWrite(pin4, 0);
  }

//================================================FUNÇÃO VIRAR PRA DIREITA==============================================
  void virar_pra_direita()
  {
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
    analogWrite(pin3, velocidade);
    analogWrite(pin4, 0);
  }

  //================================================FUNÇÃO VIRAR PRA DIREITA==============================================
  void pararMotores()
  {
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
    analogWrite(pin3, 0);
    analogWrite(pin4, 0);
  }
