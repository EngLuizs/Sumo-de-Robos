# 🤖 Robô Sumô - EngRobots

Este projeto apresenta um robô sumô autônomo controlado por ESP32, equipado com sensores IR e ultrassônicos, e capaz de receber comandos via Bluetooth. Desenvolvido para competições de robótica, o robô detecta bordas da arena e localiza o oponente para atacá-lo ou se reposicionar estrategicamente.

## 🚀 Funcionalidades

- Detecção da borda da arena com 5 sensores infravermelhos (IR)
- Detecção de oponente com 4 sensores ultrassônicos (frente, trás, esquerda, direita)
- Controle dos motores por ponte H (duas rodas motrizes)
- Conexão Bluetooth para controle remoto
- Comportamento autônomo de ataque e evasão
- Código modular com funções para sensores e movimentos

## 🧩 Componentes Utilizados

- ESP32
- 4 Sensores Ultrassônicos HC-SR04
- 5 Sensores Infravermelhos (IR)
- Ponte H (L298N ou similar)
- 2 Motores DC com redução (ex: N20 1000RPM)
- Fonte de alimentação (pack de baterias)
- Módulo Bluetooth (via ESP32 integrado)
- Estrutura impressa em 3D

## 📐 Pinagem

| Componente         | Pinos ESP32       |
|--------------------|-------------------|
| Ultrassônico Frente (Trigger, Echo) | 26, 25 |
| Ultrassônico Trás   | 33, 32            |
| Ultrassônico Direito| 4, 16             |
| Ultrassônico Esquerdo| 17, 18           |
| IR Frente Dir (IRFD) | 19              |
| IR Frente Centro (IRFC) | 21          |
| IR Frente Esq (IRFE) | 22             |
| IR Trás Dir (IRTD)  | 35               |
| IR Trás Esq (IRTE)  | 34               |
| Ponte H (Motor 1)   | 23, 13            |
| Ponte H (Motor 2)   | 12, 27            |

## 📲 Comandos Bluetooth

O robô aceita comandos via aplicativo Bluetooth ou monitor serial Bluetooth (ex: Serial Bluetooth Terminal):

- **`L`**: Inicia o modo de luta (autônomo)
- **`D`**: Interrompe o modo de luta

Nome do dispositivo Bluetooth: **EngRobots**

## 🧠 Lógica de Decisão

- Se sensores IR detectam borda → realiza manobra de recuo e reposicionamento
- Se sensores ultrassônicos detectam oponente à frente → avança
- Se oponente está nos lados → gira em direção ao oponente
- Se não detecta nada → para os motores

## 🎮 Controle de Movimento

- `move_pra_frente()`
- `dar_re()`
- `virar_pra_direita()`
- `virar_pra_esquerda()`
- `pararMotores()`

## 🛠️ Como Usar

1. Faça upload do código para o ESP32 usando o Arduino IDE.
2. Conecte os sensores e motores conforme a pinagem.
3. Alimente o robô com bateria adequada.
4. Emparelhe o Bluetooth com seu celular/computador.
5. Envie o comando **`L`** para iniciar a luta.

## 📎 Licença

Este projeto é de uso educacional e livre para modificações.

---

Desenvolvido por **EngRobots** 🤖⚡

