#include <AccelStepper.h>

/* Projeto Curto Circuito - A4988 & Arduino: Controle para motores Bipolar */
/*---- Pinos de controle A4988--- */
#define ENA 9 /* Enable ativa os drivers do motor */
#define MS1 8 /* Seleciona a resolução do passo */
#define MS2 7 /* Seleciona a resolução do passo */
#define MS3 6 /* Seleciona a resolução do passo */
#define RST 5 /* Desativa os drivers de saída */
#define SLP 4 /* Inicia um descanso para algumas funções do drive */
#define STP 3 /* Avanço do passo */
#define DIR 2 /* Direção do passo */
/*---- Variáveis de controle ---- */
int PPR = 0;      /* pulsos por resolução */
int passo = 0;    /* passos */
int temp = 1000;  /* tempo entre os passos */

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(SLP, LOW);   /* Desativa o modo sleep */
  delay(10);
  Serial.begin(9600);
  Serial.println("Pressione: 1 - para Full-Step / 2 - para Half-Step / 3 - para Quarter-Step / 4 - para Eighth-Step / 5 - para Sixteenth-Step");
}

void loop() {
  // Se o monitor serial estiver disponível
  if (Serial.available() > 0) {
    char choice = Serial.read(); // Lê o caractere do monitor serial

    // Executa a ação correspondente à escolha
    switch (choice) {
      case '1':
        full();
        volta();
        break;
      case '2':
        half();
        volta();
        break;
      case '3':
        quarter();
        volta();
        break;
      case '4':
        eighth();
        volta();
        break;
      case '5':
        sixteenth();
        volta();
        break;
      case '6':
        rst();
        break;
      case '7':
        Sleepon();
        break;
      case '8':
        Sleepoff();
        break;
      default:
        break;
    }
  }
}

void full() {
  PPR = 200;               /* Em Full - 200 passos */
  Serial.println("Modo Passo Completo (Full-Step)");
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}

void half() {
  PPR = 400;               /* Em Half - 400 passos */
  Serial.println("Modo Meio Passo (Half-Step)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}

void quarter() {
  PPR = 800;               /* Em Quarter - 800 passos */
  Serial.println("Modo Quarter-Step (1/4)");
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}

void eighth() {
  PPR = 1600;              /* Em Eighth - 1600 passos */
  Serial.println("Modo Eighth-Step (1/8)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}

void sixteenth() {
  PPR = 3200;              /* Em Sixteenth - 3200 passos */
  Serial.println("Modo Sixteenth-Step (1/16)");
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}

void rst() {
  digitalWrite(RST, HIGH);  /* Ativa o reset dos drivers de saída */
  delay(1);
  digitalWrite(RST, LOW);   /* Desativa o reset dos drivers de saída */
  delay(1);
  Serial.println("Drivers reiniciados.");
}

void Sleepon() {
  digitalWrite(SLP, HIGH);  /* Ativa o modo sleep */
  Serial.println("Modo sleep ativado.");
}

void Sleepoff() {
  digitalWrite(SLP, LOW);   /* Desativa o modo sleep */
  Serial.println("Modo sleep desativado.");
}

void volta() {
  digitalWrite(ENA, LOW);   /* Ativa os drivers do motor */
  digitalWrite(DIR, HIGH);  /* Define a direção do passo como horário */
  for (int i = 0; i < PPR; i++) {
    digitalWrite(STP, HIGH);   /* Aciona o passo */
    delayMicroseconds(temp);
    digitalWrite(STP, LOW);    /* Desativa o passo */
    delayMicroseconds(temp);
  }
  digitalWrite(DIR, LOW);   /* Define a direção do passo como anti-horário */
  for (int i = 0; i < PPR; i++) {
    digitalWrite(STP, HIGH);   /* Aciona o passo */
    delayMicroseconds(temp);
    digitalWrite(STP, LOW);    /* Desativa o passo */
    delayMicroseconds(temp);
  }
  digitalWrite(ENA, HIGH);   /* Desativa os drivers do motor */
  Serial.println("Movimento concluído.");
}