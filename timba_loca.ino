#include <Keypad.h>
#include <LiquidCrystal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SERIAL_DEBUG_ENABLED 1

#if SERIAL_DEBUG_ENABLED
  #define DebugPrint(str)\
      {\
        Serial.println(str);\
      }
#else
  #define DebugPrint(str)
#endif

#define DebugPrintEstado(estado,evento)\
      {\
        String est = estado;\
        String evt = evento;\
        String str;\
        str = "-----------------------------------------------------";\
        DebugPrint(str);\
        str = "EST-> [" + est + "]: " + "EVT-> [" + evt + "].";\
        DebugPrint(str);\
        str = "-----------------------------------------------------";\
        DebugPrint(str);\
      }
//----------------------------------------------


#define PUERTO_SERIAL 9600
#define CANT_NULA_DE_CARACTERES 0

#define APUESTA_MAXIMA  	1000000
#define APUESTA_MAX_CIFRAS	7
#define APUESTA_NULA		0
#define DOBLAR_APUESTA  	2
#define TRIPLICAR_APUESTA   3

#define VAL_MINIMO_TIRO 2
#define VAL_MAXIMO_TIRO 13
#define VAL_TIRO_PWM    23

//**			macros valores de la suma de los dados **//
#define DADOS_VALOR_CERO	0
#define DADOS_VALOR_UNO		1
#define DADOS_VALOR_DOS		2
#define DADOS_VALOR_TRES	3
#define DADOS_VALOR_CUATRO	4
#define DADOS_VALOR_CINCO	5
#define DADOS_VALOR_SEIS	6
#define DADOS_VALOR_SIETE	7
#define DADOS_VALOR_OCHO	8
#define DADOS_VALOR_NUEVE	9
#define DADOS_VALOR_DIEZ	10
#define DADOS_VALOR_ONCE	11
#define DADOS_VALOR_DOCE	12
//**			fin macros valores de la suma de los dados **//

//**           macros potenciometro 					   **//
#define UMBRAL_PERDER_MIN 683
#define UMBRAL_PERDER_MAX 1024
#define UMBRAL_GANAR_MIN  0
#define UMBRAL_GANAR_MAX  342
#define UMBRAL_NOP_MIN 	  343
#define UMBRAL_NOP_MAX    682
//**           fin macros potenciometro 				   **//

//**		   macros teclado							   **//
#define FILAS_TECLADO 	 4
#define COLUM_TECLADO 	 3
#define TECLA_DE_FIN 	'#'
#define TECLA_BORRAR 	'*'
//**	       fin macros teclado    					   **//

//**		   macros buzzer							   **//
#define NOTE_Do  261
#define NOTE_Re  293
#define NOTE_Mi  329
#define NOTE_Fa  349
#define NOTE_Sol 391
#define NOTE_La  440
#define NOTE_Si  493
#define NOTE_DoS 277
#define NOTE_ReS 311
//**		   fin macros buzzer					       **//
//tipo de dato t_nota
typedef int t_nota;
bool reproduciendo_musica;
#define TIEMPO_CORTE_DE_NOTA 65
//DEFINO MELODIA 1 -> USADA EN EL CALCULO DEL RESULTADO
#define CANT_NOTAS_MELODIA_CALC_RES 5
#define TIEMPO_ENTRE_NOTAS_MELODIA_CALC_RES 30
const t_nota melodia_calc_res[] {
	NOTE_Sol,
	NOTE_La,
	NOTE_Sol,
	NOTE_Fa,
	NOTE_Fa
};

//DEFINO MELODIA 2 -> USADA AL FINAL DEL JUEGO
#define CANT_NOTAS_MELODIA_FINAL 4
#define TIEMPO_ENTRE_NOTAS_MELODIA_FINAL   30
const t_nota melodia_final[] {
	NOTE_Fa,
	NOTE_Sol,
	NOTE_La,
	NOTE_Do,
};

//variables musica
#define ITERADOR_VALOR_INCIAL 0
int iterador_melodia;

//--- CANTIDAD DE EVENTOS MAXIMOS A CONTAR

#define CANT_EVS_CONT_MAX  30000
#define CANT_EVS_CONT_TIMEOUT 150000
//----------------------------------------------

//variables del teclado4x4
const char teclas[FILAS_TECLADO][COLUM_TECLADO] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte teclado4x4_filas_pins[FILAS_TECLADO] = {8,7,6,5};//Defino los pins de las filas del teclado
byte teclado4x4_colum_pins[COLUM_TECLADO] = {4,A5,A4};//Defino los pins de las columnas del teclado
Keypad teclado4x4 = Keypad(makeKeymap(teclas),teclado4x4_filas_pins,teclado4x4_colum_pins,FILAS_TECLADO,COLUM_TECLADO); 
//**				macros del display lcd  	**//
#define LCD_CANTIDAD_FILAS 2
#define LCD_CANTIDAD_COLUM 16
//	pines lcd
#define LCD_RS 			 9
#define LCD_ENABLE 		 A0
#define LCD_D4 			 10
#define LCD_D5 			 11
#define LCD_D6 			 12
#define LCD_D7 			 13
//	filas lcd
#define LCD_PRIMER_FILA    0
#define LCD_SEGUNDA_FILA   1
//	columnas lcd
#define LCD_PRIMER_COLUM   0
#define LCD_SEGUNDA_COLUM  1
#define LCD_TERCERA_COLUM  2
#define LCD_CUARTA_COLUM   3
#define LCD_QUINTA_COLUM   4
#define LCD_SEXTA_COLUM    5
#define LCD_SEPTIMA_COLUM  6
#define LCD_OCTAVA_COLUM   7
#define LCD_NOVENA_COLUM   8
#define LCD_DECIMA_COLUM   9
//**			fin macros del display lcd		**//

//variables lcd
LiquidCrystal lcd(LCD_RS,LCD_ENABLE,LCD_D4,LCD_D5,LCD_D6,LCD_D7); //Defino los puertos para manipular el display LCD

//variables teclado y apuesta
#define NUM_CARACTER_INICIAL 0
char caracter_ob_teclado;
String str_apuesta_ingresada;
bool teclado4x4_bloqueado = false;
long apuesta_final;

//variables juego
int valor_tiro;
bool juego_terminado;
#define CANT_VALORES_DE_DADOS 11
bool *ptr_tiros_marcados = NULL;

//variables contador de tiempo
#define CONTADOR_VALOR_INICIAL 0
int cont_ev_continuar;
int cantidad_de_letras_escritas;

//pines actuadores

#define LED_PIN				3
#define POTENCIOMETRO_PIN	A1 
#define BUZZER_PIN			2

////////////////////////////////
//	variables timeout
#define UMBRAL_DIFERENCIA_TIMEOUT 50
bool timeout;
long lct;
long musica_temporizador;

enum estados 		  {	ST_EMBEBID_NO_PREPARADO,   ST_INACTIVO,	  ST_PEND_DE_TIRO, 	  ST_TRAMPA  ,  ST_POT_LECT,   ST_TIRO_FINALIZADO,   ST_JUEGO_TERMINADO,   ST_TIMEOUT,   ST_ERROR} estado_act;
String estados_s [] = {"ST_EMBEBID_NO_PREPARADO", "ST_INACTIVO", "ST_PEND_DE_TIRO" , "ST_TRAMPA" , "ST_POT_LECT", "ST_TIRO_FINALIZADO", "ST_JUEGO_TERMINADO", "ST_TIMEOUT", "ST_ERROR"};

enum eventos 		  { EV_CONT,   EV_INGR_TECLA,   EV_INGR_APUESTA,   EV_POT_GANAR  , EV_POT_PERDER  ,  EV_POT_NOP,  EV_DESCONOCIDO} nuevo_evento;
String eventos_s [] = {"EV_CONT", "EV_INGR_TECLA", "EV_INGR_APUESTA", "EV_POT_GANAR" , "EV_POT_PERDER", "EV_POT_NOP", "EV_DESCONOCIDO"};

#define CANT_ESTADOS 9
#define CANT_EVENTOS 7

typedef void (*transition)();

transition tabla_de_estados[CANT_ESTADOS][CANT_EVENTOS] =
{
      {iniciar             , nop			  , nop              , nop             , nop            , nop              , error       } , // state ST_EMBEBID_NO_PREPARADO
      {nop                 , ingr_tecla       , ingresar_apuesta , nop             , nop            , nop              , error       } , // state ST_INACTIVO
      {esperar_timeout     , realizar_tiro    , nop              , nop             , nop            , nop              , error       } , // state ST_PEND_DE_TIRO
      {espera_calc_res     , nop              , nop              , nop             , nop            , nop              , error       } , // state ST_TRAMPA
      {nop 				   , nop              , nop              , trampa_ganar    , trampa_perder  , mostrar_fin_tiro , error       } , // state ST_POT_LECT
	  {esperar_timeout     , terminar_turno   , nop              , nop             , nop            , nop              , error       } , // state ST_TIRO_FINALIZADO
	  {fin_juego_tocar_msc , reiniciar_juego  , nop              , nop             , nop            , nop              , error       } , // state ST_JUEGO_TERMINADO
	  {nop                 , reiniciar_juego  , nop              , nop             , nop            , nop              , error       } , // state ST_TIMEOUT
      {error               , error            , error            , error           , error          , error            , error       }   // state ST_ERROR
						                                                                                            
     //EV_CONT             , EV_INGR_TECLA    , EV_INGR_APUESTA  , EV_POT_GANAR    , EV_POT_PERDER  , EV_POT_NOP     , EV_DESCONOCIDO
};

#define EXISTE_ESTADO(A) (A>=0 && A<CANT_ESTADOS)
#define EXISTE_EVENTO(A) (A>=0 && A<CANT_EVENTOS)

//---------------------- FUNCIONES MAQUINA DE ESTADOS --------------------\\

void get_nuevo_evento( )
{
	long ct = millis();
	int  diferencia = (ct - lct);
	timeout = (diferencia > UMBRAL_DIFERENCIA_TIMEOUT)? (true):(false);

	if( timeout )
	{
		timeout = false;
		lct     = ct;
    
		if(verificar_estado_sensor_teclado4x4() == true || verificar_estado_sensor_potenciometro()==true)
		  return;
		
	}
 
	nuevo_evento = EV_CONT;
}

void maquina_estados_juego()
{
	get_nuevo_evento();

	if( EXISTE_ESTADO(estado_act) && EXISTE_EVENTO(nuevo_evento) )	
		tabla_de_estados[estado_act][nuevo_evento]();
	else
	{ //Si se detecta un estado desconocido
		estado_act = ST_ERROR;
		DebugPrintEstado(estados_s[ST_ERROR], eventos_s[EV_DESCONOCIDO]);
	}
		
	nuevo_evento = EV_CONT;
}

//----------------------------- FUNCIONES DE ARDUINO --------------\\

void setup()
{
	Serial.begin(PUERTO_SERIAL);
	lcd.begin(LCD_CANTIDAD_COLUM,LCD_CANTIDAD_FILAS);
	lcd.print("inic. sistema");
	
	// Inicializo el evento inicial
	estado_act = ST_EMBEBID_NO_PREPARADO;
	
	// Inicializo el led
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	
	// inicializo el potenciometro
	pinMode(POTENCIOMETRO_PIN,INPUT);
	
	timeout = false;
	lct = millis();
	cont_ev_continuar = CONTADOR_VALOR_INICIAL;
}

void loop()
{
	maquina_estados_juego();
}

//-------------------------- VERIFICACIÓN DE SENSORES ------------- \\
//						

bool verificar_estado_sensor_teclado4x4()
{	
	bool hubo_actualizacion = false;
	caracter_ob_teclado = teclado4x4.getKey();
	
	if( caracter_ob_teclado != NO_KEY )
	{		
		 Serial.print("Tecla ingresada: ");
      	 Serial.println(caracter_ob_teclado);
		
		if( caracter_ob_teclado == TECLA_DE_FIN)
			nuevo_evento=EV_INGR_APUESTA;
		else 
			nuevo_evento=EV_INGR_TECLA;
		
		hubo_actualizacion = true;
	}
  
	return hubo_actualizacion;
}

bool verificar_estado_sensor_potenciometro()
{
	bool hubo_actualizacion = false;
	int valor_pot_act = analogRead(POTENCIOMETRO_PIN); //leo el potenciomentro
	
	if(valor_pot_act>=UMBRAL_GANAR_MIN && valor_pot_act<=UMBRAL_GANAR_MAX) // si el potenciometro esta dentro del umbral para forzar la victoria
	{
		nuevo_evento = EV_POT_GANAR;
		hubo_actualizacion = true;
	}
	else if(valor_pot_act>=UMBRAL_PERDER_MIN && valor_pot_act<=UMBRAL_PERDER_MAX) //si el potenciometro esta dentro del umbral para forzar la derrota
	{
		nuevo_evento = EV_POT_PERDER;
		hubo_actualizacion = true;
	}
	else if (valor_pot_act >= UMBRAL_NOP_MIN && valor_pot_act<=UMBRAL_NOP_MAX)
	{
		nuevo_evento = EV_POT_NOP;
		hubo_actualizacion = true;
	}
	
	return hubo_actualizacion;
}

//------------------------------------ FUNCIONES LCD --------------------- \\
//

void lcd_mostrar_msj(String msj1, String msj2)
{
	lcd.clear();
	lcd.print(msj1);
	lcd.setCursor(LCD_PRIMER_COLUM,LCD_SEGUNDA_FILA);
	lcd.print(msj2);		
}

//------------------------------------ FUNCIONES MUSICA --------------------- \\
//

void tocar_musica(int duracion_nota , int pausa_entre_notas, void (*tocar)(void))
{
	long tiempo_act = millis();
	
	if(reproduciendo_musica) //si se esta reproduciendo una nota
	{
		if(tiempo_act-musica_temporizador>=duracion_nota) // si ya transcurrio la duracion de la nota
		{
			musica_temporizador=tiempo_act; //actualizo el valor para el proximo temporizador
			noTone(BUZZER_PIN);
			reproduciendo_musica = false;
		}	
	}
	else if (tiempo_act - musica_temporizador>=pausa_entre_notas) // si ya transcurrio la duracion de pausa entre notas
	{
		musica_temporizador=tiempo_act; //actualizo el valor para el proximo temporizador
		iterador_melodia = (iterador_melodia+1)%CANT_NOTAS_MELODIA_FINAL;  //hago que el iterador se reinicie a 0 una vez recorrida toda la melodia
		tocar(); //se toca la melodia correspondiente
		reproduciendo_musica=true;
	}
}

//------------------------------------ EJECUCION DE EVENTOS --------------------- \\
//						


void tocar_musica_final()
{
	tone(BUZZER_PIN,melodia_final[iterador_melodia]);
}

void tocar_musica_calc_res()
{
	tone(BUZZER_PIN,melodia_calc_res[iterador_melodia]);
}


void iniciar()
{
	DebugPrintEstado(estados_s[estado_act], eventos_s[nuevo_evento]);
	
	//vector de booleanos utilizado para saber cuando un tiro quedo marcado
	ptr_tiros_marcados = (bool*)calloc(CANT_VALORES_DE_DADOS,sizeof(bool)); //el calloc setea los booleanos en falso
	
	reproduciendo_musica=false;
	juego_terminado=false;
		
	iterador_melodia = ITERADOR_VALOR_INCIAL;
		
	lcd_mostrar_msj("apuesta y desp#","");	
	cantidad_de_letras_escritas = CONTADOR_VALOR_INICIAL;
	musica_temporizador = millis();
	estado_act = ST_INACTIVO ;
} 

void ingr_tecla()
{	
	if(caracter_ob_teclado == TECLA_BORRAR && cantidad_de_letras_escritas!=CANT_NULA_DE_CARACTERES)  //si la tecla es '*' y se escribio algo
	{	// borro la letra escrita por el usuario
		str_apuesta_ingresada = str_apuesta_ingresada.substring(NUM_CARACTER_INICIAL, cantidad_de_letras_escritas-1);
		cantidad_de_letras_escritas--;
		
	    lcd.setCursor(cantidad_de_letras_escritas,LCD_SEGUNDA_FILA);
		lcd.print(' ');
		lcd.setCursor(cantidad_de_letras_escritas,LCD_SEGUNDA_FILA);
	}
	else if(caracter_ob_teclado != TECLA_BORRAR) //si la tecla es distinta de '*'
	{ // añado la letra escrita por el usuario
		str_apuesta_ingresada+=caracter_ob_teclado; 
		cantidad_de_letras_escritas++;
		lcd.print(caracter_ob_teclado);
	}
}

void ingresar_apuesta()
{
	apuesta_final=APUESTA_NULA; 
	
	if(str_apuesta_ingresada.length()==CANT_NULA_DE_CARACTERES) //si se ingreso una apuesta
		apuesta_nula_ingresada(); //muestro que la apuesta es nula
	else if(APUESTA_MAX_CIFRAS>=str_apuesta_ingresada.length()) //si la apuesta no supera el maximo de cifras (7)
	{
		apuesta_final = atol(str_apuesta_ingresada.c_str());
		if(apuesta_final==APUESTA_NULA) //si la apuesta es nula
			apuesta_nula_ingresada(); //muestro que la apuesta es nula
		else if(apuesta_final>APUESTA_MAXIMA) //si la apuesta supera el maximo
			apuesta_mayor_maxima_ingresada(); //muestro que se supero el maximo
		else 
		{ //si la apuesta no es nula ni supera el maximo de cifras o de valor
			randomSeed(micros()); //SETEO LA SEMILLA SEGUN LA CANTIDAD DE MICROSEGUNDOS TRANSCURRIDOS
			lcd_mostrar_msj("Tire los dados", "press t.p/ cont");
			estado_act=ST_PEND_DE_TIRO;	
		}
	} 
	else //si la apuesta supera el maximo de cifra
		apuesta_mayor_maxima_ingresada();
	
	str_apuesta_ingresada = "";	 //limpio el string para un proximo ingreso
	cont_ev_continuar=CONTADOR_VALOR_INICIAL;
}

void apuesta_nula_ingresada() //muestra un mensaje de que no se ingreso ninguna apuesta
{
	lcd_mostrar_msj("apuesta no","   ingresada ..");
	estado_act=ST_EMBEBID_NO_PREPARADO;
}

void apuesta_mayor_maxima_ingresada() // muestra un mensaje diciendo que la apuesta supera el maximo
{
	lcd_mostrar_msj("apuesta supera","   el maximo");
	estado_act=ST_EMBEBID_NO_PREPARADO;	
}

void esperar_timeout()
{	
	if(cont_ev_continuar==CANT_EVS_CONT_TIMEOUT) //si se cuentan la cantidad necesaria de eventos continuar
	{
		lcd_mostrar_msj("    timeout", ""); 
		estado_act=ST_TIMEOUT;
	}
	else
		tocar_musica(TIEMPO_CORTE_DE_NOTA, TIEMPO_ENTRE_NOTAS_MELODIA_CALC_RES, tocar_musica_calc_res); //reproduzco musica
	
	cont_ev_continuar++;
}

void realizar_tiro()
{	
	if(reproduciendo_musica==true)
		noTone(BUZZER_PIN);
	
	valor_tiro = random(VAL_MINIMO_TIRO,VAL_MAXIMO_TIRO); //se realiza el tiro
	
	lcd_mostrar_msj(" calculando", "   resultado..");
	
	//Con pwm seteo el valor del led segun que tan bueno fue el tiro
	if(valor_tiro == DADOS_VALOR_SIETE || valor_tiro == DADOS_VALOR_ONCE)
		digitalWrite(LED_PIN, LOW);
	else
	{
		analogWrite(LED_PIN,valor_tiro*VAL_TIRO_PWM);
		Serial.println(valor_tiro*VAL_TIRO_PWM);
	}
		
	
	// ESPERA Y TRAMPA
	cont_ev_continuar=CONTADOR_VALOR_INICIAL;
	estado_act = ST_TRAMPA;
}

void espera_calc_res()
{
	if(cont_ev_continuar==CANT_EVS_CONT_MAX) //si se cuentan la cantidad necesaria de eventos continuar
	{
		noTone(BUZZER_PIN); //para cualquier nota que se este reproduciendo
		cont_ev_continuar=CONTADOR_VALOR_INICIAL; 
		estado_act = ST_POT_LECT;
	}
    
    cont_ev_continuar++;	
}

void mostrar_fin_tiro()
{
	String msj_tiro_ob = "valor obt: ";
	msj_tiro_ob+=valor_tiro;
	lcd_mostrar_msj(msj_tiro_ob,"press t.p/ cont");
	estado_act=ST_TIRO_FINALIZADO;
}

void trampa_ganar()
{
	valor_tiro = DADOS_VALOR_DOCE;
	mostrar_fin_tiro();
}

void trampa_perder()
{
	if(random(DADOS_VALOR_UNO,DADOS_VALOR_TRES)==DADOS_VALOR_UNO) //elijo aleatoriamente si va a perder con sacando 7 u 11 en la suma de los dados
		valor_tiro = DADOS_VALOR_SIETE;
	else
		valor_tiro = DADOS_VALOR_ONCE;
	mostrar_fin_tiro();
}


void actualizar_apuesta()
{
	
	switch(valor_tiro) //Segun el valor que se obtuvo en el dado
	{ //se dobla, triplica o se pierde la apuesta
		case DADOS_VALOR_DOS: //si saco 2, doblo la apuesta y termina el juego
			apuesta_final=DOBLAR_APUESTA*apuesta_final;
			juego_terminado=true;
			break;
		case DADOS_VALOR_TRES: //si saco de 3 a 6, doblo la apuesta
		case DADOS_VALOR_CUATRO:
		case DADOS_VALOR_CINCO:
		case DADOS_VALOR_SEIS:
			apuesta_final=DOBLAR_APUESTA*apuesta_final;
			break;
		case DADOS_VALOR_SIETE: //si saco 7, pierde la apuesta
			apuesta_final=APUESTA_NULA;
			juego_terminado=true;
			break;
		case DADOS_VALOR_OCHO: //si saco de 8 a 10, dobla la apuesta
		case DADOS_VALOR_NUEVE:
		case DADOS_VALOR_DIEZ:
			apuesta_final=DOBLAR_APUESTA*apuesta_final;
			break;
		case DADOS_VALOR_ONCE: //si saco 11, pierde la apuesta
			apuesta_final=APUESTA_NULA;
			juego_terminado=true;
			break;
		case DADOS_VALOR_DOCE: //si saco 12, triplica la apuesta y termina el juego
			apuesta_final=TRIPLICAR_APUESTA*apuesta_final;
			juego_terminado=true;
			break;
	}
	
	if(juego_terminado==false && *(ptr_tiros_marcados+(valor_tiro-1)) == true) // SI EL JUEGO NO SE TERMINO Y EL TIRO OBTENIDO ESTA MARCADO
		juego_terminado = true; //TERMINO EL JUEGO
	else //SINO
		*(ptr_tiros_marcados+(valor_tiro-1)) = true; //MARCO EL VALOR DEL TIRO
	
}

void terminar_turno()
{
	actualizar_apuesta();
	
	if(juego_terminado==false) //si no se termino el juego, el jugador tiramente los dados
	{
		lcd_mostrar_msj("Tire los dados", "press t.p/ cont");
		estado_act=ST_PEND_DE_TIRO;	
	}
	else
	{ //sino termino el jeugo
		cont_ev_continuar=0;
		lcd_mostrar_msj("dinero obt:","");
		lcd.print(apuesta_final);
		estado_act=ST_JUEGO_TERMINADO;
	}	
}

void fin_juego_tocar_msc()
{
	tocar_musica(TIEMPO_ENTRE_NOTAS_MELODIA_FINAL, TIEMPO_CORTE_DE_NOTA, tocar_musica_final);
}

void reiniciar_juego()
{
	noTone(BUZZER_PIN);
	lcd.clear();
	lcd.print("reinic. sist.");
	free(ptr_tiros_marcados); 
	ptr_tiros_marcados=NULL;
	estado_act=ST_EMBEBID_NO_PREPARADO;
}

void error()
{
	if(ptr_tiros_marcados!=NULL) //si habia memoria reservada, la libero
		free(ptr_tiros_marcados); 
	
	lcd_mostrar_msj("error detectado", "  reiniciando ..");
	estado_act=ST_EMBEBID_NO_PREPARADO;
}

void nop()
{
}