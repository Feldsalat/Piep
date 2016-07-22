  /*

  Spannungsüberwachung mit automatischer Zellenzahlerkennung für 2-4s LiPo.
  Zwei Warnschwellen: "argwenig" und "ganzargwenig"
  Zugehörige Pausenzeiten: "interval1" und "interval2"
  Zuschalten der Last über "minimum", abschalten unter "zuwenig".
  Dirk Bauer 2016
  */


#define Akku A3                      // AnalogPin Messwerterfassung
#define Skalierung 3.73              // je nach Eingangsspannungsteiler errechnen. Bis 17V: + 27k Akku 10k -
#define Piep 9                       // DigitalPin Piepser
#define Last 13                      // Lastabwurf

float argwenig = 3.5;                // nerviges Gepiepse Grenzwert
float ganzargwenig = 3.4;            // ganz arg nerviges Gepiepse Grenzwert
float zuwenig = 3.0;                 // jetzt ist Schluss
float minimum = 3.8;                 // Mindestzellenspannung

unsigned long previousMillis = 0;    // Speicher
long interval;                       // wirkliche Pausenlänge 
unsigned long previousMillisD = 0;   // Pausenlänge 
const long interval1 = 10000;        // Pausenlänge Nervpiep (argwenig)
const long interval2 = 2000;         // Pausenlänge2 Ultranervpiep (ganzargwenig)
uint16_t sensorValue;                // da wird der Wert rein gelesen 
float Akkuspannung;
float Zellenspannung;                
int Typ;                             // Akku-Typ, Teiler für argwenig und ganzargwenig
unsigned long currentMillis;

const int AnzahlMessungen = 30;      // wie oft 

int Messungen[AnzahlMessungen];      // Messungen
int index = 0;                       // Index
int total = 0;                       // gemessen
int mittel = 0;                      // Mittelwert

boolean Abschaltung=true;                  // jetzt ausschalten
boolean zuschalten=false;                   // einschalten


void setup() {
  
    
    //Serial.begin(9600);
    
    for (int thisReading = 0; thisReading < AnzahlMessungen ; thisReading++)
    Messungen[thisReading] = 0; 
    
    pinMode(Piep, OUTPUT); 
    pinMode (Last, OUTPUT);
    // Akkutest:
    // Akku ganz zu Beginn mal testen, was das für einer ist
    // Grenzwerte: Typ 2 <8.45 , Typ 3 >8.45 , Typ 4 >12.65
    sensorValue = analogRead(Akku); 
    Akkuspannung = sensorValue * (5 / 1023.0) * Skalierung;
    if( Akkuspannung > 12.65) { 
      Typ = 4; 
    } else { 
      if( Akkuspannung > 8.45) {
        Typ = 3;
      } else {
        Typ = 2;
      }
    }
    
 // Piep mal Zellenzahl:
 
    if (Akkuspannung > 12.65){
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
      delay (400);
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
      delay (400);
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
      delay (400);
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
    } else{
    
    if (Akkuspannung > 8.45){
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
      delay (400);
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
      delay (400);
      digitalWrite(Piep, HIGH); // mach Piep
      delay (200);
      digitalWrite(Piep, LOW); // aus Piep
    }
     else{
        digitalWrite(Piep, HIGH); // mach Piep
        delay (200);
        digitalWrite(Piep, LOW); // aus Piep
        delay (400);
        digitalWrite(Piep, HIGH); // mach Piep
        delay (200);
        digitalWrite(Piep, LOW); // aus Piep
      }
    }
   delay (4000);
  }
  
void loop() {
  
  total= total - Messungen[index];
  
   Messungen[index] = analogRead(Akku); 
   total= total + Messungen[index];
   index = index + 1;
    
     
   if (index >= AnzahlMessungen) {              // Warten bis Mittelwert gebildet wird.
      mittel = total / AnzahlMessungen;
      Akkuspannung = ( mittel * (5 / 1023.0) * Skalierung );          
      Zellenspannung = Akkuspannung/Typ;    // Anpassung an den Akku-Typ 
      currentMillis = millis();
      if ( Zellenspannung > minimum ) { zuschalten = true; Abschaltung = false; }    //
      if ( Zellenspannung < zuwenig ) { Abschaltung = true; zuschalten = false; }   //
    
       if (zuschalten) {
           digitalWrite(Last,HIGH);
        }
         if (Abschaltung) {
             digitalWrite(Last,LOW);
        }
   }
   
   if ((Zellenspannung < argwenig) && (Zellenspannung > zuwenig))       //Akku leer Warnung
   {
     // interval2 ist das nervige schnelle piepen, interval1 nervt nur etwas
     if ( Zellenspannung > ganzargwenig ) { interval = interval1; } else { interval = interval2; } 
    
     if (currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;
       digitalWrite(Piep, HIGH); // mach Piep
       delay(200);
       digitalWrite(Piep, LOW); // und Pause
     }
   }

     
 
  // Einstellmonitor:
  /*
  if (index >= AnzahlMessungen)
  {
      Serial.print("Akku = " );   
      Serial.print(Akkuspannung);
      Serial.print("   Zelle = " );  
      Serial.println(Zellenspannung);
      Serial.print("   zuschalten = " );  
      Serial.println(zuschalten);
      Serial.print("   Abschaltung = " );  
      Serial.println(Abschaltung);
      Serial.println(minimum);  
      Serial.println(zuwenig);
      Serial.println(index);
      delay(50);
  } 
  */
  
  if (index >= AnzahlMessungen) { index = 0; }
  
}


