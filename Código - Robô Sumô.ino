/*******************************************************************************************************************************
Autor............: Equipe Eng Robots
Disciplina.......: APS
Data.............: 06/2025
*******************************************************************************************************************************/

// INCLUINDO AS BIBLIOTECAS
#include "Ultrasonic.h"
#include "BluetoothSerial.h"

// FUNÇÕES UTILIZADAS
  void leiturasIR();
  void estadoIR();
  void estadoUltra(); // AGORA RETORNA UM VALOR
  void leiturasUltra();
  void virar_pra_esquerda();
  void virar_pra_direita();
  void move_pra_frente();
  void dar_re();
  void pararMotores();
  void virar_180();
  void buzzer_on();
  void buzzer_off();

// DEFININDO PINOS SENSOR ULTRASSÔNICOS
  HC_SR04 SensorUF(26,25);  //(TRIG,ECHO)
  HC_SR04 SensorUT(33,32);
  HC_SR04 SensorUD(4,16);
  HC_SR04 SensorUE(17,18);

// DEFININDO PINOS PONTE H
  #define anti_horarioE 23
  #define horarioE 13
  #define anti_horarioD 12
  #define horarioD 27
  #define PINO_BUZZER 2

// DEFININDO PINOS SENSORES IR
  #define IRFD 19
  #define IRFC 21
  #define IRFE 22
  #define IRTD 35
  #define IRTE 34

// DEFININDO COR LIDA PELO SENSOR IR
  #define branco 0
  #define preto 1

// DEFININDO AS VELOCIDADES
  #define velocidade 255
  #define velocidade2 210
  #define velocidade3 75
  #define velocidade4 50

// DISTÂNCIA MAXIMA DE DETECÇÃO
  #define distancia 80

//CONFIGURANDO A COMUNICAÇÃO BLUETOOTH 
  BluetoothSerial serialBLE;

//ADICIONANDO VARIAVEIS DO BLUETOOTH 
  char dados;
  bool conexao;

// VARIÁVEIS
  int estadoIRFD, estadoIRFC, estadoIRFE, estadoIRTD, estadoIRTE;
  int estadoUF, estadoUT, estadoUD, estadoUE;

void setup() {
  Serial.begin(9600);
  serialBLE.begin("EngRobots");

// CONFIURANDO MOTORES E VELOCIDADES
  pinMode(anti_horarioE, OUTPUT);
  pinMode(horarioE, OUTPUT);
  pinMode(anti_horarioD, OUTPUT);
  pinMode(horarioD, OUTPUT);
  pinMode(velocidade, OUTPUT);
  pinMode(velocidade2, OUTPUT);
  pinMode(velocidade3, OUTPUT);
  pinMode(velocidade4, OUTPUT);

// CONFIGURANDO IR's COMO ENTRADA
  pinMode(IRFD, INPUT);
  pinMode(IRFC, INPUT);
  pinMode(IRFE, INPUT);
  pinMode(IRTD, INPUT);
  pinMode(IRTE, INPUT);

// CONFIGURANDO BUZZER E INICIANDO DESLIGADO
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  // FORÇANDO A INICIALIZAÇÃO COM OS MOTORES DESLIGADOS
  pararMotores();
}

void loop() 
{

  leiturasUltra(); //LENDO ULTRA
  estadoUltra();   //AÇÃO DO ULTRA
  leiturasIR();    //LENDO IR
  estadoIR();      //AÇÃO IR

//COMANDOS BLUETOOTH
    conexao = serialBLE.available();
    if (conexao == true) // SE A CONEXÃO FOR VERDADEIRA, SE FAZ A LEITURA DOS DADOS ENVIADOS
    {
      dados = serialBLE.read();
      if (dados == 'D')  // SE DADOS FOR IGUAL A "D" DESIGA O ROBÔ
      {
        while (true)
        {
          pararMotores();
          if (serialBLE.available())
          {
            dados = serialBLE.read();  // SE DADOS FOR IGUAL A "L" ELE LIGA O ROBÔ DEPOIS DE DESLIGADO
            if (dados == 'L')
            {
              break; // Sai do while
            }
          }
        }
      }
    }
}


// LEITURA DOS SENSORES IR
  void leiturasIR() {
    estadoIRFD = digitalRead(IRFD);
    estadoIRFC = digitalRead(IRFC);
    estadoIRFE = digitalRead(IRFE);
    estadoIRTD = digitalRead(IRTD);
    estadoIRTE = digitalRead(IRTE);
  }

// AÇÕES COM BASE NOS SENSORES IR
  void estadoIR() {
    if ((estadoIRFD == branco) || (estadoIRFC == branco) || (estadoIRFE == branco)) { 
      dar_re();
      delay(250);
      virar_180();
    }

    if ((estadoIRTD == branco) || (estadoIRTE == branco)) {
      move_pra_frente();
    } 
  }

// AÇÕES COM BASE NOS ULTRASSONICOS
  void estadoUltra() {

    if (estadoUF <= distancia) 
    {
      move_pra_frente();
      buzzer_on();
      delay(150);
      buzzer_off();
    } 
      
    else if (estadoUD <= distancia)
    {
      virar_pra_direita();
    } 
      
    else if (estadoUE <= distancia) {
      virar_pra_esquerda();
    } 
      
    else if (estadoUT <= distancia)
    {
      virar_180();
    } 
      
    else 
    {
      move_pra_frente_lento();
    }
  }

// LEITURA ULTRASSÔNICO
  void leiturasUltra() 
  {
    estadoUF = SensorUF.distance();
    estadoUT = SensorUT.distance();
    estadoUD = SensorUD.distance();
    estadoUE = SensorUE.distance();
  }

// CONTROLE DOS MOTORES
  void move_pra_frente() 
  {
    analogWrite(anti_horarioE, velocidade);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, velocidade);
    analogWrite(horarioD, 0);
  }

  void move_pra_frente_lento() 
  {
    analogWrite(anti_horarioE, velocidade4);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, velocidade4);
    analogWrite(horarioD, 0);
  }

  void dar_re() 
  {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, velocidade);
  }

  void virar_pra_esquerda() 
  {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade3);
    analogWrite(anti_horarioD, velocidade2);
    analogWrite(horarioD, 0);
  }

  void virar_pra_direita() 
  {
    analogWrite(anti_horarioE, velocidade2);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, velocidade3);
  }

  void virar_180() 
  {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade2);
    analogWrite(anti_horarioD, velocidade);
    analogWrite(horarioD, 0);
  }

  void pararMotores() 
  {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, 0);
  }

// BUZZER
  void buzzer_on() 
  {
    digitalWrite(PINO_BUZZER, HIGH);
  }

  void buzzer_off() 
  {
    digitalWrite(PINO_BUZZER, LOW);
  }
