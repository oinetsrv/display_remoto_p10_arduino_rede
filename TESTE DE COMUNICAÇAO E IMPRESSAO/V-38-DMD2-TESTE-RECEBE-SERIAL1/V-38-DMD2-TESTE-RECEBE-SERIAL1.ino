/*
Projeto: Controle de DISPLAY 
CONTROLADOR 1
|- RECEBE DADOS VIA REDE
|- ENVIA VIA SERIAL1
CONTROLADOR 2
|- RECEBE VIA SERIAL1
|- IMPRIME DISPLAY COM 4 MODULOS

AUTOR: STENIO RODRIGUES
DATA: 18/12/2020
OBJETIVO:
IMPRIMIR NO DISPLAY USANDO DMD2
    Serial.println("V-35-DMD2-RETORNO-REDE-150-ENVIA--SERIAL1:");
// CONTROLADOR UNICO PARA OS 4 DISPLAYS CONECTOR
// CONECTOR DISPLAY
________________________
|1 pin_noe 2  pin_a     |
|3  GND    4  pin_b     |
|5  GND    6  N/C       |
|7  GND    8  pin_sck  S|
|9  GND    10 pin_clk  L|
|11 GND    12 pin_r_datR|
________________________

// PAINEL           MEGA
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9
//byte GND          GND 
//byte pin_a        6
//byte pin_b        7
//byte pin_sck      52
//byte pin_clk      8
//byte pin_r_data   51
///////////////////////
CONECTOR REDE
________________________
|1 CLK    2  INT /NT   |
|3 WOL    4  SO        |
|5 SI     6  SCK       |
|7 CS     8  RST       |
|9 VCC    10 GND       |
________________________

//ADAPTADOR REDE    MEGA
1 CLK               N/C
3 WOL               N/C
5 SI                51
7 CS                53
9 VCC               3.3
2  INT /NT          2
4  SO               50     
6  SCK              52
8  RST              N/C
10 GND              GND

PARA MONITORAR MENSAGEM RECEBIDA ACESSE
http://192.168.0.x:2000/
       // escolha QUAL DISPLAY - QUAL FONTE 
       // EXEMPLO: http://192.168.0.151:2000/S1234*
       //-->> DISPLAY SAIDA FONTE Arial14
       //http://192.168.0.151:2000/9*
       //-->> DISPLAY POTENCIA MAXIMA
       //http://192.168.0.151:2000/8*
       //-->> DISPLAY POTENCIA MINIMO
       // PRIMEIRO DIGITO ESCOLHE QUAL DISPLAY
       // S -> SAIDA
       // E -> ENTRADA
       // 9 -> BRILHO MAXIMO
       // 8 -> BRILHO MINIMO
       // SEGUNDO DIGITO ESCOLHE QUAL TIPO DE FONTE
       // 0 -> SystemFont5x7
       // 1 -> Droid_Sans_12
       // 2 -> Arial14
       // 3 -> Arial_Black_16
 

*/
#define led 13

void setup() {

Serial.begin(9600);
Serial1.begin(9600);
pinMode(led,OUTPUT);
digitalWrite(led,LOW);
Serial.println("V-38-TESTE-VARREDURA-REDE-RECEBE:");  
}

void loop(){
if (Serial1.available()>0)
{
  char c = Serial1.read();
  Serial.println("RECEBIDO: ");
  Serial.println(c);
  delay(2);
}

}