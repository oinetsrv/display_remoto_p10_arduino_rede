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
        - Definir tamanho msg
1-002 - Desenvolvimento base de manipulação de rede serial:
        - recebimento comandos via serial
        - envio de status de comando executado via serial
        - recebimento de comandos via serial1
        - envio de status de comando executado via serial1
1-003 - Desenvolvimento base de manipulação de rede 485:
        - montagem de hardware para rede 485 
        - recebimento comandos via 485
        - envio de status de comando executado via 485
        - desenvolvimento de protocolo para multiplos players
1-004 - Montagem de protótipos com 3 displays na rede:
        - criar nó usando raspberry para mudar protocolo para ethernet
        - deixar em teste os 3 displays e o raspberry
1-005 - Integrar rede telnet dos displays a rede de termometria:
USANDO VSCODE
F1 + arduino select sketch (pasta) marque o arquivo que vc quer copilar
F1 + arduino: verify, upload, open, close serial monitor, board manager, change baud rate,
change board type, select programmer, select serial port,

 */

// --- Bibliotecas Auxiliares       --- //
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_12.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_16.h>
// =================================================================================
// --- Variáveis Globais            --- //
    const int COUNTDOWN_FROM = 3;
    int counter = COUNTDOWN_FROM;
    int QDISPLAY = 1;
    const int tam_msg = 10;
    char charRecebida[tam_msg];
    String str = "";
    int ultimos_digitos = 0;
    int cont = 0;


// --- Instâncias                   --- //
SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
DMD_TextBox box(dmd, 1, 2); // "box" provides a text box to automatically write to/scroll the display
//DMD_TextBox box1(dmd, 31, 2);
//DMD_TextBox box2(dmd, 63, 2);
//DMD_TextBox box3(dmd, 95, 2);
// =================================================================================

// --- Protótipo das Funções        --- //
void teste_display_contagem ();

// =================================================================================

// --- Configurações Iniciais SETUP --- //
void setup() {
    Serial.begin(9600);
    dmd.setBrightness(255);
    dmd.selectFont(Arial14);
    dmd.begin();
    Serial.println("Projeto controle rede de displays fabrica de fertilizantes.\n");
    Serial.println("                                                             ");
    Serial.println("Nome arquivo:V-1-DMD2-DISPLAY-BASE.ino... \n");
} // end setup
// =================================================================================

// --- Loop Infinito                --- //
void loop() {

while (Serial.available()) {
    //Serial.println("CONECTADO-SERIAL1:");
    char request = Serial.read();
    str.concat(request);
    int tamanho_string = str.length();
    // concatenando string para char mais facil de manipular
        if (str.endsWith("*")) {
            int marcador = str.indexOf('E');
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
            int primeiro_marcador = str.indexOf('E'); // PEGA O MARCADOR DE FINAL
            int tamanho_corte = str.indexOf('*'); // final  texto
            String strTemp = str.substring(primeiro_marcador+2, tamanho_corte);
            int tamanho_temp = strTemp.length(); // tamanho

            if (charRecebida[0] == 'E') { //E0test*
                if (charRecebida[1] == '0') {
                    Serial.println("SystemFont5x7: ");
                    dmd.selectFont(SystemFont5x7);
                    // TEMPORARIO
                    dmd.clearScreen();
                    delay(1);
                    dmd.drawString(1,0,strTemp);
                    strTemp = "12345";
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

             //box.println(strTemp); 
             //int n = 1234;
            
             

            Serial.print  ("strTemp:   ");
            Serial.print(strTemp);
            Serial.println("");
        }

    }// end str.endsWith
    // destruindo string temp
    if (str.endsWith("*")) {  
        // testando  '\0'
            Serial.println("charRecebida[i]: ");
                for (int i = 0; i <= tam_msg; i++) {
                    Serial.print(charRecebida[i]  ) ;
                    
                }//  end for
            Serial.println("");
                int limpar_string = str.indexOf('*') + 1;
                str.remove(0, limpar_string);
                
                delay(1); 
                 
            } // end if str.endsWith
            //E0ABCDE*
            //E1ABCD*
            //E2ABC*
            //E3ABC*
} // end while
     
        
 
// =================================================================================

// =================================================================================
} // end loop
// =================================================================================
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


// --- Desenvolvimento das Funções  --- //
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
// =================================================================================
// =================================================================================
// =================================================================================
