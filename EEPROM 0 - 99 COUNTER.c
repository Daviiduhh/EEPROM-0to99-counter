/*
PRACTICA 8 - “CALCULADORA ARITMÉTICA DE 2 DÍGITOS”
EQUIPO V 
8H1 - ELECTRONICA DIGITAL V
DAVID MADRID NAPOLES 17100201
CARLOS CAMACHO MATA 17100057
HECTOR SAMUEL RIVERA RUIZ 17100287
20/10/2020

Al comenzar el programa la LCD muestra la señalización base (nombres, registros,
grupo, etc.), después la LCD solicita la velocidad la velocidad ascendente 
(200-900ms) del contador (0-99), en el teclado matricial, se presiona y soltar
* para comenzar el conteo hasta el 99, al presionar y soltar *se interrumpe la
cuenta y regresa a velocidad, al presionar y soltar A se guarda el valor de la
cuenta y lo muestra por 1s, al presionar y soltar D muestra el ultimo valor 
guardado por 1s, al resetear comienza la cuenta desde el ultimo valor guardado.

*/

#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#FUSES INTRC_IO,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT
#FUSES NOIESO,NOFCMEN,NOLVP,NODEBUG,NOWRT,BORV21

#USE FAST_IO(A)
#USE FAST_IO(B)
#USE FAST_IO(C)
#USE FAST_IO(D)

#INCLUDE <LCD1.C>

INT Z = 0, Y = 0;
INT I = 0, J = 0, AUX = 0, CUENTA = 0, L = 0;
LONG U = 0, D = 0, C = 0, V = 0;

INT RV [16] = {1, 2, 3, 101, 4, 5, 6, 102, 7, 8, 9, 103, 200, 0, 201, 104};

VOID INICIO (){
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "      CETI      \n     COLOMOS    ");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "TGO. EN CONTROL \nAUTOMATICO E INS");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "   ELECTRONICA  \n    DIGITAL V   ");         
   DELAY_MS(500);
   
   FOR(J = 0; J <= 2; J++){ // EN CADA ITERACION MUESTRA EL R Y N DE UN ALM
      SWITCH (J){
         CASE 0:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "DAVID MADRID     \n17100201            ");
            BREAK;
               
         CASE 1:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "HECTOR RUIZ     \n17100287          ");
            BREAK;
                     
         CASE 2:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "CAMACHO MATA     \n17100057         ");
            BREAK;
            }
      DELAY_MS(200);
   }
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "CONTADOR 0-99 CON\n MEMORIA EEPROM ");         
   DELAY_MS(500);
}
VOID TECLADO (){ //SE ENCARGA DE DETECTAR PULSACIONES EN EL TECLADO
   AUX = 0XEF;
   CUENTA = 0; //LLEVARA LA CUENTA, DETECTANDO LA CUENTA PRESIONADO
   FOR(I=0;I<4;I++){//EN CADA ITERACION REVISA UNA FILA DIFERENTE
         
      AUX=AUX>>1; //DESPLAZA UN CERO A LA DERECHA
      OUTPUT_A((INPUT_A()&0XF0)|(AUX&0X0F));
         
      IF(INPUT(PIN_A7)==0) //REVISDA ESTADO COLUMNA A7
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A6)==0) //REVISDA ESTADO COLUMNA A6
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A5)==0) //REVISDA ESTADO COLUMNA A5
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A4)==0) //REVISDA ESTADO COLUMNA A4
      BREAK;
      CUENTA++;
         
      DELAY_MS(20); //TIEMPO DE ESPERA NECESARIO PARA DETECTAR CAMBIOS
   }
}

VOID MAIN (){

   SET_TRIS_A(0XF0); //DECLARAR SALIDAS
   OUTPUT_A(INPUT_A()&0XF0); //LIMPIAR BITS DE SALIDA
   LCD_INIT(); //INICIAR LCD
   
   INICIO ();
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, " SELECCIONAR LA \n VELOCIDADAD MS "); 
   
   WHILE(TRUE){
      WHILE(Z == 0){ //ETAPA 1
      
         TECLADO ();
         IF(CUENTA<16){ //SI SE PRESIONA EL TECLADO MATRICIAL
            
            IF(RV[CUENTA]<10){ //SI SE PRESIONA TECLA DIFERENTE A LAS DE NUMERO            
               IF(D!=0){ //REVISA QUE DECENAS TENGA UN VALOR
                  C = D; //TRANSFIERE SU VALOR A DECENAS
               }
               IF(U!=0){ //REVISA QUE UNIDADES TENGA UN VALOR
                  D = U; //TRANSFIERE SU VALOR A LAS DECENAS
               }
               U = RV[CUENTA]; //ASIGAN EL VALOR LEIDO A U
               LCD_GOTOXY(1,1);
               PRINTF(LCD_PUTC, "VELOCIDAD = %Lu%Lu%Lu     \n                "C,D,U);      
               V = (C*100)+(D*10)+(U); //GUARDA EL VALOR REAL DE U, D Y C
               IF((V<200)|(V>900)){
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, " FUERA DE RANGO ");
               }
               ELSE{
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "                ");
               }
            }
            
            IF(CUENTA==12){ //SI SE PRESIONA EL ASTERISCO
               Z=1; //PASA A ETAPA 2
            }
            DO{
               TECLADO();
            }WHILE(CUENTA<16);
         }
      }
      
      WHILE(Z == 1){ //ETAPA 2
         IF(READ_EEPROM(1)!=0XFF&&Y==0){
            L=READ_EEPROM(1);
         }
         WHILE((L<100)&&(Z==1)){
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "CUENTA = %u     "L);
            TECLADO ();
            IF(CUENTA<16){           
               IF(CUENTA==12){ //SI SE PRESIONA EL ASTERISCO
                  LCD_GOTOXY(1,1);
                  PRINTF(LCD_PUTC, " SELECCIONAR LA \n VELOCIDADAD MS ");
                  //L = 0;
                  Z = 0; //PASA A ETAPA 1
                  DO{
                     TECLADO();
                  }WHILE(CUENTA<16);
               }
               IF(CUENTA == 3){ //SI SE PRESIONA A
                  WRITE_EEPROM(1,L);
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "VALOR = %u       ", READ_EEPROM(1));
                  DELAY_MS(1000);
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "                ");
                  
               }IF(CUENTA == 15){//SI SE PRESIONA D
                  READ_EEPROM(1);
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "VALOR = %u       ", READ_EEPROM(1));
                  DELAY_MS(1000);
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, "                ");
                  
               }               
            }
            DELAY_MS(V);
            L++;
            IF(L == 100){
               L = 0;
               Y = 1;
            }
         }        
      }
   }
}
