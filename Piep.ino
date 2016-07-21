  /*

  Spannungsüberwachung mit automatischer Zellenzahlerkennung für 2-4s LiPo.
  Zwei Warnschwellen: "argwenig" und "ganzargwenig"
  Zugehörige Pausenzeiten: "interval1" und "interval2"
  
  Dirk Bauer 2016
  */


#define Akku A3                      // AnalogPin Messwerterfassung
#define Skalierung 3.73              // je nach Eingangsspannungsteiler errechnen. Bis 17V: + 27k Akku 10k -
#define Piep 9                       // DigitalPin Piepser

float argwenig = 3.5;                // nerviges Gepiepse Grenzwert
float ganzargwenig = 3.4;            // ganz arg nerviges Gepiepse Grenzwert
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

const int AnzahlMessungen = 50;      // wie oft 

int Messungen[AnzahlMessungen];      // Messungen
int index = 0;                       // Index
int total = 0;                       // gemessen
int mittel = 0;                      // Mittelwert



void setup() {
  
    
    //Serial.begin(9600);
    
    for (int thisReading = 0; thisReading < AnzahlMessungen ; thisReading++)
    Messungen[thisReading] = 0; 
    
    // Pieptest:
    pinMode(Piep, OUTPUT); 
    digitalWrite(Piep, HIGH); // mach Piep
    delay (300);
    digitalWrite(Piep, LOW); // aus Piep
    delay (500);
    digitalWrite(Piep, HIGH); // mach Piep
    delay (300);
    digitalWrite(Piep, LOW); // aus Piep
    
    // Akkutest:
    // Akku ganz zu Beginn mal testen, was das für einer ist
    // Grenzwerte: Typ 2 <8.45 , Typ 3 >8.45 , Typ 4 >12.65
    sensorValue = analogRead(Akku); 
    Akkuspannung = sensorValue * (5 / 1023.0) * Skalierung;
    if( Akkuspannung > 12.65) { 
      Typ = 4.0; 
    } else { 
      if( Akkuspannung > 8.45) {
        Typ = 3.0;
      } else {
        Typ = 2.0;
      }
    }
  }
  
void loop() {
  
    total= total - Messungen[index];
  
    Messungen[index] = analogRead(Akku); 
    total= total + Messungen[index];
    index = index + 1;
    
     
   if (index >= AnzahlMessungen) {              // Warten bis Mittelwert gebildet wird.
    mittel = total / AnzahlMessungen;
    Akkuspannung = ( mittel * (5 / 1023.0) * Skalierung );          
    Zellenspannung= Akkuspannung/Typ;    // Anpassung an den Akku-Typ 
    currentMillis = millis();
   }
   
  if (Zellenspannung < argwenig)       //Akku leer Warnung
  {
     // interval2 ist das nervige schnelle piepen, interval1 nervt nur etwas
    if( Zellenspannung > ganzargwenig ) { interval = interval1; } else { interval = interval2; } 
    
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
      delay(5);
  } */
  
  
  if (index >= AnzahlMessungen) { index = 0; }
  
}


