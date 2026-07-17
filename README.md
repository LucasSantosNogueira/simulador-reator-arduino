# Simulador de Reator Químico: Malha de Controle de Nível e Sistema de Segurança (SIS)

Este projeto consiste no desenvolvimento de um protótipo funcional em microcontrolador (Arduino) para simular o comportamento de uma malha fechada de controle de nível proporcional e uma camada ativa de proteção independente, conhecida na indústria química como **SIS (Sistema Instrumentado de Segurança)** ou **Intertravamento de Emergência (ESD - Emergency Shutdown)**.

O objetivo principal é demonstrar de forma prática a coexistência entre o controle operacional suave de processos e as ações discretas e imediatas de mitigação de riscos industriais.

---

## 🚀 Arquitetura do Sistema

O projeto foi estruturado em duas camadas hierárquicas claras de automação:

### 1. Camada de Controle Operacional (Malha Fechada Proporcional)
* **Sensor (LT-01):** O sensor ultrassônico atua como Transmissor de Nível.
* **Atuador (LCV-01):** O servo motor simula uma Válvula de Controle de Vazão de descarga.
* **Lógica:** Através de uma função de mapeamento linear (`map`), o sistema calcula o ângulo exato da válvula. Quando o nível do reator sobe (distância do sensor diminui), a válvula abre proporcionalmente para drenar o fluido. Quando o nível baixa, a válvula fecha para reter o inventário.

### 2. Camada de Proteção e Intertravamento (SIS / ESD)
* **Condição Crítica:** Setpoint de nível criticamente alto definido em **8 cm** de distância do sensor.
* **Ação Crítica (Interlock):** Caso a malha de controle falhe ou ocorra um distúrbio severo no processo, o SIS assume o controle de forma prioritária, forçando a abertura da válvula (LCV-01) em 100% (180°).
* **Alarmes (LAH - Light/Audible Alarm High):** Ativação síncrona e intermitente de um alarme visual (LED) e sonoro (Buzzer Passivo via modulação de frequência por software).

---

## 🛠️ Hardware Utilizado (Bill of Materials)

* 01x Placa Arduino (Uno/Mega/Nano)
* 01x Sensor Ultrassônico HC-SR04 (Transmissor de Nível)
* 01x Micro Servo Motor SG90 (Válvula de Controle)
* 01x LED Difuso (Alarme Visual)
* 01x Resistor de 220 Ohms (Proteção do LED)
* 01x Buzzer Passivo (Alarme Sonoro)
* 01x Protoboard e Cabos Jumpers

---

## 📌 Mapeamento de Pinos e Conexões

| Componente | Pino do Componente | Pino do Arduino | Função Industrial |
| :--- | :--- | :--- | :--- |
| **Sensor Ultrassônico** | VCC | 5V (Barramento) | Alimentação (+5V) |
| **Sensor Ultrassônico** | Trig | Pino 12 | Disparo de Sinal (Output) |
| **Sensor Ultrassônico** | Echo | Pino 11 | Retorno de Sinal (Input) |
| **Sensor Ultrassônico** | GND | GND (Barramento) | Referência Neutra (0V) |
| **Servo Motor** | Amarelo (Sinal) | Pino 3 | Controle da Posição da Válvula |
| **Servo Motor** | Vermelho (VCC) | 5V (Barramento) | Alimentação (+5V) |
| **Servo Motor** | Preto (GND) | GND (Barramento) | Referência Neutra (0V) |
| **LED** | Perna Longa ($+$) | Pino 9 | Sinal de Ativação do Alarme Visual |
| **LED / Resistor** | Perna Curta ($-$) | GND (Barramento) | Retorno do Circuito |
| **Buzzer Passivo** | Perna do `+` | Pino 10 | Modulação de Frequência do Alarme |
| **Buzzer Passivo** | Outra Perna | GND (Barramento) | Retorno do Circuito |

---

## 📈 Demonstração do Comportamento Dinâmico

O comportamento das variáveis foi monitorado em tempo real através de ferramentas de plotagem gráfica (Serial Plotter), simulando a interface de um sistema supervisório industrial clássico (**SCADA**).

---

## 💻 Como Executar este Repositório

1. Baixe a estrutura de código presente em `/src` (ou clone o repositório).
2. Abra o arquivo na **Arduino IDE**.
3. Certifique-se de ter a biblioteca padrão `Servo.h` instalada no seu ambiente.
4. Conecte o hardware seguindo a tabela de pinagem descrita.
5. Faça o *Upload* do código para a sua placa.
6. Abra o **Plotador Serial (Serial Plotter)** configurado em `9600 baud` para visualizar o gráfico das variáveis do processo químico simulado.

---

## 🙋‍♂️ Autor

Desenvolvido por **Lucas Santos Nogueira**
* 💼 **LinkedIn:** [Acesse meu perfil profissional](https://linkedin.com/in/lucas-santos-nogueira)
* ✉️ **E-mail:** [Entre em contato comigo](mailto:lucassantosnogueira8@gmail.com)
