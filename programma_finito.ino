/**************************************************************************    
 *     Programma sviluppato per le olimpiadi della robotica di Ancona 2018
 *     da :
 *     Bertuccioli Marco
 *     Micheli Jacopo
 *     Vicini Andrea
 *************************************************************************/






/******************************************************************
                             Librerie
 ******************************************************************/

#include <Servo.h>

/******************************************************************
                Definizione dei pin e delle variabili
 ******************************************************************/

/* Definizioni parametri */
  
long distanzaSumo = 40;     /*   Distanza alla quale il robot attacca       */
int vaLum;
long cmSx;
long cmDx;
long cm;
int S = 0;
int D = 0;

/* Definizione dei PIN usati sulla scheda */

#define pinSinistraRuota         8   /*   Sinistra servo Ruota       */
#define pinDestraRuota           9   /*   Destra servo Ruota       */

/*  Dichiarazione dei pin usati */

int trigPin = 2;
int ecoPin = 3;
int trigPinSx = 4;
int ecoPinSx = 5;
int trigPinDx = 6;
int ecoPinDx = 7;
int lumDx = 10;                      
int lum = 11;                          
int lumSx = 12;                         

/* Definition of the values ​​that can take continuous rotation servo,
  that is, the ruotas */
  
#define ruotaStopValore 90
#define sinistraRuotaAvantiValore 0
#define sinistraRuotaIndietroValore 180
#define destraRuotaAvantiValore 121
#define destraRuotaIndietroValore 0

/* Bauderate of the Bluetooth*/

#define MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH    38400
#define BQ_ZUM_BLUETOOTH                       19200

/* Size of the received data buffer */

#define bufferSize 5

/* Default delay */

#define defaultDelay        10

/* Creazione oggetti per la libreria Servo */

Servo sinistraRuota;                    /*  Valores from 0 to 180  */
Servo destraRuota;                      /*  Valores from 0 to 180  */

/*  A char buffer to storage the received data from the Bluetooth
    Serial */
    
char dataBuffer[bufferSize];

/* Buffer iterator */

int i = 0;

/* Number of characters availables in the Serial */

int numChar = 0;


/******************************************************************
                       Definizione delle funzioni
 ******************************************************************/

void stopRuote() {
  sinistraRuota.write(ruotaStopValore);
  delay(defaultDelay);
  destraRuota.write(ruotaStopValore);
  delay(defaultDelay);
}

void goAvanti() {
  sinistraRuota.write(sinistraRuotaAvantiValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaAvantiValore);
  delay(defaultDelay);
}

void goIndietro() {
  sinistraRuota.write(sinistraRuotaIndietroValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaIndietroValore);
  delay(defaultDelay);
}

void goSinistra() {
  sinistraRuota.write(ruotaStopValore+1);
  delay(defaultDelay);
  destraRuota.write(destraRuotaAvantiValore);
  delay(defaultDelay);
}

void goDestra() {
  sinistraRuota.write(sinistraRuotaAvantiValore);
  delay(defaultDelay);
  destraRuota.write(ruotaStopValore);
  delay(defaultDelay);
}

void goSinistraLabirinto() {
  /*  Differisce dal goSinistra perchè ruota su se stesso invece di ruotare su una ruota  */
  sinistraRuota.write(sinistraRuotaIndietroValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaAvantiValore);
  delay(defaultDelay);
}

void goDestraLabirinto() {
  /*  Differisce dal goDestra perchè ruota su se stesso invece di ruotare su una ruota  */
  goAvanti();
  delay(100);
  stopRuote();
  sinistraRuota.write(sinistraRuotaAvantiValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaIndietroValore);
  delay(defaultDelay);
}

void Labirinto() {
  /*  Funzione per svolgere il labirinto  */
  
  /*  Dichiarazioni variabili utilizzate  */
  bool statoSx=false;
  bool statoDx=false;
  
  do
  {
    do
      {
        /*  Misurazioni delle varie distanze lette dai sensori a ultrasuoni */
        delay(15);
        cmSx = distanza(trigPinSx, ecoPinSx);
        delay(15);
        cmDx = distanza(trigPinDx, ecoPinDx);
        delay(15);
        cm = distanza(trigPin, ecoPin);
        delay(15);
        goAvanti();
      }
      while(cm > cmSx && cm > cmDx);
      /*  Se la distanza ai lati è maggiore di quella avanti esce dal loop e scegle dove girare */
      delay(475);
      stopRuote();
      if( cmSx > cmDx && statoSx == true)
      {
        goSinistraLabirinto();
        statoSx = !statoSx;
      }
      else
      {
        goDestraLabirinto();
      }
      delay(500);
      stopRuote();
      goAvanti();
      delay(125 n0);
      stopRuote();
      /*  Verifica se il bluetooth ha ricevuto messaggi, in caso affermativo esce
       *  dalla funzione labirinto  */
      if (Serial.available() > 0)
        break;
   }
   while(true);
}

long distanza(int trigPinTemp, int echoPinTemp){
  /*  Funzione che dato il sensore da usare ti misura la destinza */
  /*  La distanza che ritorna è una media di 5 valori rilevati così da diminuire
   *  la possibilità di errore  */
  long distanza = 0;
  for(short i = 0; i<5; i++) {
    digitalWrite(trigPinTemp, LOW);
    digitalWrite(trigPinTemp, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinTemp, LOW);
    distanza += 0.034 * pulseIn(echoPinTemp, HIGH) / 2;
  }
  distanza = distanza/5;
  return distanza;
}

void Line (){
  /*  Funzione per seguire la linea */
  do
  {
    /*  Legge i valori dei sensori di luminosità e scegle dove andare in modo
     *  da tenere la linea nera sotto al sensore centrale e bianco sotto gli altri
     *  due, allineare bene i sensori prima della prova in base alla grandezza
     *  della linea  */
    int vaLumDx = digitalRead( lumDx );     // lettura sensore dx
    int vaLum = digitalRead( lum );         // lettura sensore centrale
    int vaLumSx = digitalRead( lumSx );     // lettura sensore sx
    if ( vaLumSx == 0 )  \                      
    {
      if ( vaLum == 1 )
      {
        if ( vaLumDx == 0)\
        {
          goAvanti();     //101 
        }   
        else
        {
          goDestra();    //100
        }
      }
      else if ( vaLum == 0)\
      {
        if ( vaLumDx == 0)\
        {
             stopRuote(); //111
        }
        else
        {
            goDestra();    //110
        }
      }
    }
    
    else if ( vaLumSx == 1)
    {
      if ( vaLum == 1 )
      {
        if ( vaLumDx == 0)\
        {
          goSinistra(); //001
        }
        else
        {
          goAvanti();  //000
        }
      }
      else if ( vaLum == 0)\
      {
        if ( vaLumDx == 0)\
        {
          goSinistra(); //011
        }
        else
        {
           stopRuote();//010
        }
      }
    }
    if (Serial.available() > 0)
      break;
  }
  while(true);
}

void sumo(){
  /*  Funzione per il sumo  */
  
  /*  Deve partire dopo 5 secondi dal via */
  delay(5000);
  /*  Legge il sensore davanti e se l'obbiettivo è più vicino della distanza
   *  preimpostata il robot va avanti se no gira su se stesso alla 
   *  ricerca dell'avversario   */
  int vaLum;
  do
  {
    long cm = distanza( trigPin , ecoPin );
    if (cm < distanzaSumo)
    {
      goAvanti();
    }
    else
    {
      goSinistraLabirinto();
      delay(200);
      stopRuote();
      delay(200);
    }
    
    vaLum = digitalRead( lum );         // lettura sensore centrale
    if (vaLum == 0)
    {
      goIndietro();
      delay(500);
      stopRuote();
    }
    if (Serial.available() > 0)
        break;
  }
  while(true);
}

/* Manage the buffer of data */
void checkData(char* data) {

  switch (data[0]) {
    
    
    /* Line follower mode button pressed */
    case 'I':
      Line();
      break;

    /* Light avoider mode button pressed */
    case 'G':
      sumo();
      break;

    /* Obstacles avoider mode button pressed */
    case 'B':
      Labirinto();
      break;

    /* Manual control mode button pressed */
    case 'M':
      stopRuote();
      break;

    case'S':
      /* Stop button pressed */
      stopRuote();
      break;

    case 'U':
      /* Up button pressed */
      goAvanti();
      break;

    case 'D':
      /* Down button pressed */
      goIndietro();
      break;

    case 'L':
      /* Sinistra button pressed */
      goSinistra();
      break;

    case 'R':
      /* Destra button pressed */
      goDestra();
      break;
  }

  /* Empty the Serial */
  Serial.flush();
}

void setup() {

  /* Open the Bluetooth Serial and empty it */
  Serial.begin(BQ_ZUM_BLUETOOTH);
  //Serial.begin(MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH);
  Serial.flush();

  /* Define the appropiate pin to each object */
  sinistraRuota.attach(pinSinistraRuota);
  destraRuota.attach(pinDestraRuota);

  /* The robot is stopped at the beginning */
  stopRuote();

  /* Dichiarazioni pin sensori */
  
  pinMode(trigPin, OUTPUT);
  pinMode(ecoPin, INPUT);
  pinMode(trigPinSx, OUTPUT);
  pinMode(ecoPinSx, INPUT);
  pinMode(trigPinDx, OUTPUT);
  pinMode(ecoPinDx, INPUT);
  pinMode(lumDx, INPUT);
  pinMode(lum, INPUT);
  pinMode(lumSx, INPUT);
 
}

void loop() {

  /* If there is something in the Bluetooth serial port */
  if (Serial.available() > 0) {

    /* Reset the iterator and clear the buffer */
    i = 0;
    memset(dataBuffer, 0, sizeof(dataBuffer));

    /* Wait for let the buffer fills up. Depends on the length of
       the data, 1 ms for each character more or less */
    delay(bufferSize);

    /* Number of characters availables in the Bluetooth Serial */
    numChar = Serial.available();

    /* Limit the number of characters that will be read from the
       Serial to avoid reading more than the size of the buffer */
    if (numChar > bufferSize) {
      numChar = bufferSize;
    }

    /* Read the Bluetooth Serial and store it in the buffer */
    while (numChar--) {
      dataBuffer[i++] = Serial.read();
      /* As data trickles in from your serial port you are
        grabbing as much as you can, but then when it runs out
        (as it will after a few bytes because the processor is
        much faster than a 9600 baud device) you exit loop, which
        then restarts, and resets i to zero, and someChar to an
        empty array.So please be sure to keep this delay */
      delay(3);
    }
    
    /* Manage the data */
    checkData(dataBuffer);
  }
}
