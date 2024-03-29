/*
 ----------------------------------------------------------
|   PROJETO  : Controle de display via rede 485            |
|   AUTOR    : STENIO RODRIGUES                            |
|   DATA     : 09/08/2021                                  |
|   OBJETIVO :                                             |
|              Imprimir mensagens recebidas via rede 485   |
|              aplicação industria de fertilizantes        |
 ----------------------------------------------------------
      Organizando código.
    // =================================================================================
    // --- Bibliotecas Auxiliares       --- //
    // =================================================================================
    // --- Mapeamento de Hardware       --- //
    // =================================================================================
    // --- Instâncias                   --- //
    // =================================================================================
    // --- Protótipo das Funções        --- //
    // =================================================================================
    // --- Variáveis Globais            --- //
    // =================================================================================
    // --- Interrupções                 --- //
    // =================================================================================
    // --- Configurações Iniciais SETUP --- //
    // =================================================================================
    // --- Loop Infinito                --- //
    // =================================================================================
    // --- Desenvolvimento das Funções  --- //
    // =================================================================================

   Mapeamento hardware
 -----------------------
|1 pin_noe 2  pin_a     |
|3  GND    4  pin_b     |
|5  GND    6  N/C       |
|7  GND    8  pin_sck   |
|9  GND    10 pin_clk   |
|11 GND    12 pin_r_data|
 -----------------------
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

// PAINEL           MEGA      UNO
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9         9
//byte GND          GND       GND
//byte pin_a        6         6
//byte pin_b        7         7
//byte pin_sck      52        12    
//byte pin_clk      8         8
//byte pin_r_data   51        11  
Etapas: INICIO EM 10/09/2021 --> PREVISÃO DE CONCLUSÃO UMA SEMANA!!!!
1-001 - Desenvolvimento base de manipulação do display:
        OK  - Escolha de fontes
        OK  - Definir tamanho msg
1-002 - Desenvolvimento base de manipulação de rede serial:
        OK  - recebimento comandos via serial
        OK  - recebimento de comandos via serial1
1-003 - Desenvolvimento base de manipulação de rede 485:
        OK  - montagem de hardware para rede 485 
        OK  - recebimento comandos via 485
        OK  - desenvolvimento de protocolo para multiplos players
1-004 - Montagem de protótipos com 3 displays na rede:
        - criar nó usando raspberry para mudar protocolo para ethernet
        OK  - deixar em teste os 3 displays e o raspberry
1-005 - Desenvolvimento base de manipulação de rede serial:
        OK  - envio de status de comando executado via serial
        OK  - envio de status de comando executado via serial1
        OK  - envio de status de comando executado via 485
1-006 - Integrar rede telnet dos displays a rede de termometria:

USANDO VSCODE
F1 + arduino select sketch (pasta) marque o arquivo que vc quer copilar
F1 + arduino: verify, upload, open, close serial monitor, board manager, change baud rate,
change board type, select programmer, select serial port,
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    PROTOCOLO DE REDE DISPLAY
            // PRIMEIRO DIGITO:  ENDEREÇO CLIENTE
                // COMO FILTRO UTILIZAR CARACTERES ESPECIAIS EXEMPLO:@,#,$
            // SEGUNDO  DIGITO:  TIPO DE FONTE
                // CONFIGURA EM: 0=SystemFont5x7, 1=Droid_Sans_12, 2=Arial14, 3=Arial_Black_16
            // TERCEIRO DIGITO: TEXTO LIVRE
            // ULTIMO   DIGITO: MARCADOR FINAL MENSAGEM *
            //E0ABCDE*
            //E1ABCD*
            //E2ABC*
            //E3ABC*
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&     */

// --- Bibliotecas Auxiliares       --- //
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_12.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_16.h>
#include <avr/io.h>  // wachdog
#include <avr/wdt.h> // wachdog
// =================================================================================
#define pinled 13
#define MASTER 17

// --- Variáveis Globais            --- //
    const int COUNTDOWN_FROM = 3;
    int counter = COUNTDOWN_FROM;
    int QDISPLAY = 1;
    const int tam_msg = 10;
    char charRecebida[tam_msg];
    String str = "";
    int ultimos_digitos = 0;
    int cont = 0,cont1 = 0,marca=0;
// --- Instâncias                   --- //
SoftDMD     dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
DMD_TextBox box(dmd, 1, 2); // "box" provides a text box to automatically write to/scroll the display
//DMD_TextBox box1(dmd, 31, 2);
//DMD_TextBox box2(dmd, 63, 2);
//DMD_TextBox box3(dmd, 95, 2);
// =================================================================================
// --- Protótipo das Funções        --- //
void teste_display_contagem     ();
void Comunica_serial            ();// RECEBE VIA 232
void Comunica_serial1E          ();// RECEBE VIA 485 E IMPRIME POR MARCADOR E
void Comunica_serial1S          ();// RECEBE VIA 485 E IMPRIME POR MARCADOR S
void Comunica_serial1T (int marca);// RECEBE VIA 485 E IMPRIME POR MARCADOR T


// =================================================================================
// --- Configurações Iniciais SETUP --- //
void setup() {
    wdt_disable   (     ); // desativa o dog
    wdt_enable    (WDTO_8S); // ativa o wachdog
   /*#define WDTO_15MS,30MS,60MS,120MS,250MS,500MS,1S,2S,4S,8S*/
    Serial.begin(9600);
    Serial1.begin(9600);
    dmd.setBrightness(255);
    dmd.selectFont(Arial14);
    dmd.begin();
    pinMode(pinled, OUTPUT);
    pinMode(MASTER, OUTPUT);
    digitalWrite(pinled, LOW);
    digitalWrite(MASTER, LOW);
    Serial.println("\n\nProjeto controle rede de displays fabrica de fertilizantes.");
    //Serial.println("Reset de 8 segundos  Comunica_serialR();                     ");
    Serial.println("Reset de 8 segundos  Comunica_serial1T(marca);                 ");
    Serial.println("Nome arquivo:V-1-DMD2-DISPLAY-BASE.ino...                      ");
    Serial.println("\n\n");
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(1, 0, "...");
    delay(1000);
    dmd.clearScreen();
    delay(1);
} // end setup
// =================================================================================

// --- Loop Infinito                --- //
void loop() {
    wdt_reset();
    marca = 3; // 1 Marcador S || 2 Marcador E || 3 Marcador T 
    if (Serial1.available()) {
      Comunica_serial1T(marca);  
      delay(50);
    }else{
        if (str.length()>1){
            Serial.println("EXCLUIU DADO RECEBIDO!");
            str.remove(0, str.length());
            for (int i = 0; i <= tam_msg; i++) {
                 charRecebida[i] = ' ';
             }// END FOR
             cont=0;
        }
        delay(50);

    }
    //Comunica_serial1T(marca);
    //Comunica_serialR();
} // end loop
// =================================================================================
// --- Desenvolvimento das Funções  --- //
void teste_display_contagem (){
    Serial.print(counter);
    Serial.println(F("..."));
    box.print(' ');
    box.print(counter);
    box.println(F("..."));
    counter--;
    delay(1000);
        if (counter == 0) {
            dmd.fillScreen(true);
            delay(500);
            dmd.clearScreen();
            delay(500);
            box.clear();
            counter = COUNTDOWN_FROM;
            delay(1000);
        } // end if(counter == 0)
}// end teste_display_contagem
// =================================================================================
void Comunica_serialR(){
    while (Serial.available()) {
        char request = Serial.read();
        charRecebida[cont] = request;
        cont++;
        str = request;
        if (str.endsWith("*")) {
            Serial.print("Serial enviada!!!:   ");
            digitalWrite( MASTER, HIGH);
            delay(10);
             for (int i = 0; i <= cont; i++) {
                 Serial.print ( charRecebida[i]);
                 Serial.flush ();
                 Serial1.print( charRecebida[i]);
                 Serial1.flush();
                 charRecebida[i] = ' ';
             }// END FOR
             cont=0;
             delay(10);
             digitalWrite(MASTER, LOW  );
             Serial.print(" DESTRUIDA!\n\n ");
             str.remove(-1, str.length());
             delay(1); 
        }// END str.endsWith("*")    
    } // end while  
    while (Serial1.available()) {
        char request = Serial1.read();
        str.concat(request);
        int tamanho_string = str.length();
        cont1++;
    } // end while
    while (!Serial1.available() && cont1>1) {
        Serial.print("STRING IMPRESSA!: ");
        Serial.print(str);
        Serial.print("\n");
        Serial.flush();
        str.remove(-1, str.length());
        delay(1); 
        cont1=0;
    } // end while  
}// end Comunica_serial()
// =================================================================================
void Comunica_serial1T(int marca) {
    while (Serial1.available()) {
        char request = Serial1.read();
        str.concat(request);
        int tamanho_string = str.length();
        int marcador = -1;
        charRecebida[cont] = request;
        cont++;
        // ONDE INDENTIFICO O ENDERECO(E,S,T)   ///////
        if (str.endsWith("*")) { //S0ABCDE*
            int marcadorS = -1;
            int marcadorE = -1;
            int marcadorT = -1;
            int tamanho_corte = str.length();
            Serial.print("String str brutaaa: ");
            Serial.println (str); // ENVIA NA SERIAL O QUE RECEBEU SERIAL1
            Serial.flush ();    // ESPERA DADOS SEREM TODOS ENVIADOS
            for (int i = 0; i < tamanho_string; i++) {
                if (charRecebida[i] == 'S' && marca == 1) {
                    if (charRecebida[i + 1] == '0' || charRecebida[i + 1] == '1' || charRecebida[i + 1] == '2' || charRecebida[i + 1] == '3') {
                        marcador = i;
                        marcadorS = 1;
                        Serial.print("Marcador posicao S: ");
                        Serial.print(marcador);
                        Serial.flush (); 
                    } // END IF
                } //END IF
                if (charRecebida[i] == 'E' && marca == 2) {
                    if (charRecebida[i + 1] == '0' || charRecebida[i + 1] == '1' || charRecebida[i + 1] == '2' || charRecebida[i + 1] == '3') {
                        marcador = i;
                        marcadorE = 1;
                        Serial.print("Marcador posicao E: ");
                        Serial.print(marcadorE);
                        Serial.flush (); 
                    } // END IF
                } //END IF
                if (charRecebida[i] == 'T' && marca == 3) {
                    if (charRecebida[i + 1] == '0' || charRecebida[i + 1] == '1' || charRecebida[i + 1] == '2' || charRecebida[i + 1] == '3') {
                        marcador = i;
                        marcadorT = 1;
                        Serial.print("Marcador posicao T: ");
                        Serial.print(marcadorT);
                        Serial.flush (); 
                    } // END IF
                } //END IF
                if (charRecebida[i] == '*') {
                    //Serial.print("Marcador posicao *: ");
                    tamanho_corte = i;
                    //Serial.println(tamanho_corte);
                    //Serial.flush (); 
                } //END IF
            } // end for
            // END ONDE INDENTIFICO O ENDERECO(E,S,T)   ///////
            // DEFININDO FONTE  E SEPARANDO TEXTO//////////
            String strTemp = str.substring(marcador + 2, tamanho_corte);
            // DEFININDO QUAL O MARCADOR FOI SELECIONADO!
            if (marcadorS != -1 && marca == 1 || marcadorE != -1 && marca == 2 || marcadorT != -1 && marca == 3) {
                
                if (charRecebida[marcador + 1] == '0') {
                    Serial.println("SystemFont5x7: ");
                    dmd.selectFont(SystemFont5x7);
                    dmd.clearScreen();
                    delay(1);
                    dmd.drawString(1, 0, strTemp);
                    dmd.drawString(1, 8, strTemp);

                    Serial.print ("String impressa: ");
                    Serial.print (strTemp            );
                    Serial.print ("\n               ");
                    Serial.flush (); 
                    delay(10);
                    digitalWrite(pinled, HIGH);
                    digitalWrite(MASTER, HIGH);
                    delay(10);
                    //Serial1.print(str);// DEVOLVENDO A STRING PARA REDE
                    //Serial1.flush (); 
                    delay(10);
                    digitalWrite(pinled, LOW);
                    digitalWrite(MASTER, LOW);
                    cont=0;

                } // end charRecebida 
                if (charRecebida[marcador + 1] == '1') {
                    Serial.println("Droid_Sans_12: ");
                    dmd.selectFont(Droid_Sans_12);
                    dmd.clearScreen();
                    delay(1);
                    dmd.drawString(2, 3, strTemp);

                    Serial.print ("String impressa: ");
                    Serial.print (strTemp            );
                    Serial.print ("\n               ");
                    Serial.flush (); 
                    delay(10);
                    digitalWrite(pinled, HIGH);
                    digitalWrite(MASTER, HIGH);
                    delay(10);
                    //Serial1.print(str);// DEVOLVENDO A STRING PARA REDE
                    //Serial1.flush (); 
                    delay(10);
                    digitalWrite(pinled, LOW);
                    digitalWrite(MASTER, LOW);
                    cont=0;
                    
                } // end if
                if (charRecebida[marcador + 1] == '2') {
                    Serial.println("Arial14: ");
                    dmd.selectFont(Arial14);
                    dmd.clearScreen();
                    delay(1);
                    dmd.drawString(2, 2, strTemp);

                    
                    Serial.print ("String impressa: ");
                    Serial.print (strTemp            );
                    Serial.print ("\n               ");
                    Serial.flush (); 
                    delay(10);
                    digitalWrite(pinled, HIGH);
                    digitalWrite(MASTER, HIGH);
                    delay(10);
                    //Serial1.print(str);// DEVOLVENDO A STRING PARA REDE
                    //Serial1.flush (); 
                    delay(10);
                    digitalWrite(pinled, LOW);
                    digitalWrite(MASTER, LOW);
                    cont=0;
                    
                } // end if
                if (charRecebida[marcador + 1] == '3') {
                    Serial.println("Arial_Black_16: ");
                    dmd.selectFont(Arial_Black_16);
                    dmd.clearScreen();
                    delay(1);
                    dmd.drawString(2, 2, strTemp);
                    
                    Serial.print ("String impressa: ");
                    Serial.print (strTemp            );
                    Serial.print ("\n               ");
                    Serial.flush (); 
                    delay(10);
                    digitalWrite(pinled, HIGH);
                    digitalWrite(MASTER, HIGH);
                    delay(10);
                    //Serial1.print(str);// DEVOLVENDO A STRING PARA REDE
                    //Serial1.flush (); 
                    delay(10);
                    digitalWrite(pinled, LOW);
                    digitalWrite(MASTER, LOW);
                    cont=0;
                    
                } // end if
            } // end marcadorS com marca 
            delay(10);
        } // end if (str.endsWith("*")) //E3ABC*
        // destruindo string temp
        if (str.endsWith("*")) {
            cont = 0;
            Serial.print("String DESTRUIDA!: ");
            delay(10);
            int limpar_string = str.indexOf('*') + 1;
            str.remove(0, str.length());
            Serial.print("\n");
        } // end if str.endsWith
    } // end while
    //LIMPANDO NA TORRA!!!
        digitalWrite(pinled, LOW);
        digitalWrite(MASTER, LOW);
} // end Comunica_serialT()
// =================================================================================
// =================================================================================
void Comunica_serial(){
    while (Serial.available()) {
        //Serial.println("CONECTADO-SERIAL1:");
        char request = Serial.read();
        str.concat(request);
        int tamanho_string = str.length();
        int marcador = 0;
        // PARA MARCADOR EM SSSSSSSS
            // concatenando string para char mais facil de manipular PARA ENTRADA S
            if (str.endsWith("*")) { //S0ABCDE*
            int marcadorS = str.indexOf('S');
            int marcadorE = str.indexOf('E');
            if (marcadorS+1 == str.indexOf('0') || marcadorS+1 == str.indexOf('1') || marcadorS+1 == str.indexOf('2') || marcadorS+1 == str.indexOf('3')  ){
                marcador = str.indexOf('S');
                Serial.println("Encontrou marcador S e 0,1,2,3");
            }// end if
            if (marcadorE+1 == str.indexOf('0') || marcadorE+1 == str.indexOf('1') || marcadorE+1 == str.indexOf('2') || marcadorE+1 == str.indexOf('3')  ){
                marcador = str.indexOf('E');
                Serial.println("Encontrou marcador E e 0,1,2,3");
            }
            int cont=0;
                    for (int i = marcador; i <= tamanho_string; i++) {
                        charRecebida[cont] = str[i];
                        cont++;
                    }//  end for
                    Serial.print  ("charRecebida:   ");
                    Serial.print  ( charRecebida     );
                    Serial.println("                ");
                    delay(1); 
            } // end if str.endsWith
            // impressão da mensagem tratada
            if (str.endsWith("*")) {
                    int tamanho_corte = str.indexOf('*'); // final  texto
                    String strTemp = str.substring(marcador+2, tamanho_corte);
                    int tamanho_temp = strTemp.length(); // tamanho
                        if (charRecebida[1] == '0') {
                            Serial.println("SystemFont5x7: ");
                            dmd.selectFont(SystemFont5x7);
                            // TEMPORARIO
                            dmd.clearScreen();
                            delay(1);
                            dmd.drawString(1,0,strTemp);
                            //strTemp = "12345";
                            dmd.drawString(1,8,strTemp);
                            // END TEMPORARIO
                            // Serial.println("  ");
                        }// end charRecebida 
                        if (charRecebida[1] == '1') {
                            Serial.println("Droid_Sans_12: ");
                            dmd.selectFont(Droid_Sans_12);
                            // TEMPORARIO
                            dmd.clearScreen();
                            delay(1);
                            dmd.drawString(2,3,strTemp);
                            // END TEMPORARIO
                            //Serial.println("  ");
                        }// end if
                        if (charRecebida[1] == '2') {
                            Serial.println("Arial14: ");
                            dmd.selectFont(Arial14);
                            // TEMPORARIO
                            dmd.clearScreen();
                            delay(1);
                            dmd.drawString(2,2,strTemp);
                            // END TEMPORARIO
                            //Serial.println("  ");
                        } // end if
                        if (charRecebida[1] == '3') {
                            Serial.println("Arial_Black_16: ");
                            dmd.selectFont(Arial_Black_16);
                            // TEMPORARIO
                            dmd.clearScreen();
                            delay(1);
                            dmd.drawString(2,2,strTemp);
                            // END TEMPORARIO
                            //Serial.println("  ");
                        } // end if
                    // retirar digitos significativos da impressao
                    // FIM ESCOLHA FONTE
                    Serial.print  ("str enviada!!!:   ");
                    Serial.print(str);
                    digitalWrite(pinled, HIGH);
                    digitalWrite(MASTER, HIGH);
                    delay(100);
                    Serial1.print(str);
                    delay(1000);
                    digitalWrite(pinled, LOW);
                    digitalWrite(MASTER, LOW);
                    //delay(1000);
                    Serial.println("");
            }// end if (str.endsWith("*"))

            // destruindo string temp
            if (str.endsWith("*")) {  
                // testando  '\0'
                    Serial.println("string DESTRUIDA!: ");
                    for (int i = 0; i <= tam_msg; i++) {
                        Serial.print(charRecebida[i]  ) ;  
                    }//  end for
                    Serial.println("");
                    int limpar_string = str.indexOf('*') + 1;
                    str.remove(0, limpar_string);
                    delay(1); 
            } // end if str.endsWith
    } // end while
}// end Comunica_serial()
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
// =================================================================================
