#include <Servo.h>

// =========================================================================
// PROJETO: Simulador de Malha de Controle de Nível e Sistema de Segurança (SIS)
// Aplicação: Controle de Processos Químicos e Prevenção de Transbordo (ESD)
// =========================================================================

// Mapeamento de Hardware (Padrão de Engenharia)
const int pinoTrig   = 12;  // Transmissor de Nível (LT-01) - Envio do pulso
const int pinoEcho   = 11;  // Transmissor de Nível (LT-01) - Leitura do eco
const int pinoServo  = 3;   // Válvula de Controle de Vazão (LCV-01) - Servo Motor
const int pinoLED    = 9;   // Alarme de Nível Alto - Indicador Visual (LAH)
const int pinoBuzzer = 10;  // Alarme de Nível Alto - Indicador Sonoro (LAH)

// Parâmetros de Ajuste do Processo (Setpoints)
const float LIMITE_CRITICO_ESD = 8.0; // Ponto de Intertravamento de Segurança (8 cm do sensor)
const float NIVEL_VAZIO        = 20.0; // Limite operacional de tanque vazio (20 cm do sensor)
const float NIVEL_CHEIO        = 9.0;  // Limite operacional de tanque cheio (9 cm do sensor)

Servo valvulaControle; // Representação física da nossa válvula industrial

void setup() {
  // Inicializa a porta serial para enviar dados ao sistema "supervisório" do PC
  Serial.begin(9600);
  
  // Configurações de Entrada e Saída (I/O)
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  
  // Associa o objeto servo ao pino físico
  valvulaControle.attach(pinoServo);
  
  // Inicializa o processo com a válvula totalmente fechada (Segurança)
  valvulaControle.write(0);
}

void loop() {
  // --- 1. LEITURA DA VARIÁVEL DE PROCESSO (NÍVEL DO FLUIDO) ---
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  long duracao = pulseIn(pinoEcho, HIGH);
  float nivelDistancia = (duracao * 0.0343) / 2.0;

  // Filtro de segurança para evitar ruídos de leitura física além do reator
  if (nivelDistancia > 25.0) {
    nivelDistancia = 25.0;
  }

  int aberturaValvula = 0;

  // --- 2. LÓGICA DO SISTEMA INSTRUMENTADO DE SEGURANÇA (SIS) ---
  // Se o fluido subir além do limite crítico (distância menor ou igual a 8cm)
  if (nivelDistancia <= LIMITE_CRITICO_ESD) {
    
    // Interlock (Intertravamento): Força a abertura total da válvula para esvaziar o reator
    aberturaValvula = 180; 
    valvulaControle.write(aberturaValvula);
    
    // Ativação do protocolo de Parada de Emergência (ESD): Alarme sonoro e visual pulsante
    digitalWrite(pinoLED, HIGH);
    tone(pinoBuzzer, 1500); // Emite som agudo de alerta de 1500 Hz
    delay(100);
    digitalWrite(pinoLED, LOW);
    noTone(pinoBuzzer);
    delay(100);
  } 
  
  // --- 3. LÓGICA DE CONTROLE OPERACIONAL NORMAL (PROPORCIONAL) ---
  else {
    // Desliga todos os alarmes de emergência
    digitalWrite(pinoLED, LOW);
    noTone(pinoBuzzer);
    
    // Controle Proporcional: Calcula a abertura da válvula de forma linear
    // Se estiver em 20cm (Vazio) -> Válvula em 0 graus (Fechada)
    // Se estiver em 9cm (Cheio) -> Válvula em 180 graus (Aberta 100%)
    aberturaValvula = map(nivelDistancia, NIVEL_CHEIO, NIVEL_VAZIO, 180, 0);
    
    // Garante que o sinal de controle enviado não ultrapasse os limites físicos do motor (0 a 180)
    aberturaValvula = constrain(aberturaValvula, 0, 180); 
    
    // Atua fisicamente na válvula
    valvulaControle.write(aberturaValvula);
    
    // Aguarda o tempo de acomodação do processo físico (resposta dinâmica do sistema)
    delay(150); 
  }

  // --- 4. EXPORTAÇÃO DE DADOS PARA COMPARTILHAR NO GRAPH PLOTTER ---
  // Enviamos os dados formatados exatamente como a IDE precisa para gerar o gráfico real
  Serial.print("Nivel_Cm:");
  Serial.print(nivelDistancia);
  Serial.print(",");
  Serial.print("Abertura_Valvula_Graus:");
  Serial.println(aberturaValvula);
}
