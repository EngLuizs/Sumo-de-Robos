/*******************************************************************************************************************************
Autor............: Equipe Eng Robots
Disciplina.......: APS
Data.............: 06/2025
*******************************************************************************************************************************/

#include "Ultrasonic.h"
#include "BluetoothSerial.h"

// FUNÇÕES
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

// SENSORES ULTRASSÔNICOS
  HC_SR04 SensorUF(26,25);
  HC_SR04 SensorUT(33,32);
  HC_SR04 SensorUD(4,16);
  HC_SR04 SensorUE(17,18);

// PONTE H
  #define anti_horarioE 23
  #define horarioE 13
  #define anti_horarioD 12
  #define horarioD 27
  #define PINO_BUZZER 2

// SENSORES IR
  #define IRFD 19
  #define IRFC 21
  #define IRFE 22
  #define IRTD 35
  #define IRTE 34

// LEITURAS
  #define branco 0
  #define preto 1

// VELOCIDADES
  #define velocidade 255
  #define velocidade2 210
  #define velocidade3 75
  #define velocidade4 50

// DISTÂNCIA DE DETECÇÃO
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

// Motores
  pinMode(anti_horarioE, OUTPUT);
  pinMode(horarioE, OUTPUT);
  pinMode(anti_horarioD, OUTPUT);
  pinMode(horarioD, OUTPUT);
  pinMode(velocidade, OUTPUT);
  pinMode(velocidade2, OUTPUT);
  pinMode(velocidade3, OUTPUT);
  pinMode(velocidade4, OUTPUT);

// IR
  pinMode(IRFD, INPUT);
  pinMode(IRFC, INPUT);
  pinMode(IRFE, INPUT);
  pinMode(IRTD, INPUT);
  pinMode(IRTE, INPUT);

// Buzzer
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  pararMotores();
  delay(2000);
  
  analogWrite(anti_horarioE, velocidade4);
  analogWrite(horarioE, 0);
  analogWrite(anti_horarioD, velocidade4);
  analogWrite(horarioD, 0);
  delay(500);
}

void loop() 
{

  leiturasUltra(); //LENDO ULTRA
  estadoUltra();   //AÇÃO DO ULTRA
  leiturasIR();    //LENDO IR
  estadoIR();      //AÇÃO IR

//COMANDOS BLUETOOTH
    conexao = serialBLE.available();
    if (conexao == true)
    {
      dados = serialBLE.read();
      if (dados == 'D')
      {
        while (true)
        {
          pararMotores();
          if (serialBLE.available())
          {
            dados = serialBLE.read();
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

// ESTADO ULTRASSÔNICO COM RETORNO DE PRIORIDADE
  void estadoUltra() {

    if (estadoUF <= distancia) {
      move_pra_frente();
      buzzer_on();
      delay(150);
      buzzer_off();

    } 
      else if (estadoUD <= distancia) {
      virar_pra_direita();
    } 
      else if (estadoUE <= distancia) {
      virar_pra_esquerda();
    } 
      else if (estadoUT <= distancia) {
      virar_180();
    } 
      else {
      move_pra_frente_lento();
    }
  }

// LEITURA ULTRASSÔNICO
  void leiturasUltra() {
    estadoUF = SensorUF.distance();
    estadoUT = SensorUT.distance();
    estadoUD = SensorUD.distance();
    estadoUE = SensorUE.distance();
  }

// CONTROLE DOS MOTORES
  void move_pra_frente() {
    analogWrite(anti_horarioE, velocidade);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, velocidade);
    analogWrite(horarioD, 0);
  }

  void move_pra_frente_lento() {
    analogWrite(anti_horarioE, velocidade4);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, velocidade4);
    analogWrite(horarioD, 0);
  }

  void dar_re() {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, velocidade);
  }

  void virar_pra_esquerda() {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade3);
    analogWrite(anti_horarioD, velocidade2);
    analogWrite(horarioD, 0);
  }

  void virar_pra_direita() {
    analogWrite(anti_horarioE, velocidade2);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, velocidade3);
  }

  void virar_180() {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, velocidade2);
    analogWrite(anti_horarioD, velocidade);
    analogWrite(horarioD, 0);
  }

  void pararMotores() {
    analogWrite(anti_horarioE, 0);
    analogWrite(horarioE, 0);
    analogWrite(anti_horarioD, 0);
    analogWrite(horarioD, 0);
  }

// BUZZER
  void buzzer_on() {
    digitalWrite(PINO_BUZZER, HIGH);
  }

  void buzzer_off() {
    digitalWrite(PINO_BUZZER, LOW);
  }
