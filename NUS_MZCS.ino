// Definiowanie pinów połączonych z krańcówkami drukarki
// Limit Switches
//
#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19       

// Definicja zmiennych globalnych, w których zapisywane są stany krańcówek (0 lub 1)
int xMaxPinValue;
int yMaxPinValue;
int zMaxPinValue;

// Definicja pinów odpowiedzialnych za ruch silnikami krokowymi
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

// Zdefiniowana ilość kroków, które ma wykonać każdy z silników
int x_steps = 0;
int y_steps = 0;
int z_steps = 0;

// Zmienna pomocnicza wykorzystywana przy wpisywaniu ilości kroków do wykonania
String s;

// Flaga sygnalizujaca rozpoczecie ruchu
bool moving = false;

void setup() {
  // put your setup code here, to run once:
  // Inicjalizacja monitoru szeregowego
  Serial.begin(9600);

  // Ustawienie trybu pinów odpowiedzialnych za krańcówki jako wejście (do Arduino)
  pinMode(X_MAX_PIN, INPUT);
  // Ustawienie pull up
  digitalWrite(X_MAX_PIN, HIGH);

  pinMode(Y_MAX_PIN, INPUT);
  digitalWrite(Y_MAX_PIN, HIGH);

  pinMode(Z_MAX_PIN, INPUT);
  digitalWrite(Z_MAX_PIN, HIGH);

  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);

  pinMode(X_DIR_PIN, OUTPUT);
  digitalWrite(X_DIR_PIN, LOW);

  pinMode(X_STEP_PIN, OUTPUT);


  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);

  pinMode(Y_DIR_PIN, OUTPUT);
  digitalWrite(Y_DIR_PIN, LOW);

  pinMode(Y_STEP_PIN, OUTPUT);


  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);

  pinMode(Z_DIR_PIN, OUTPUT);
  digitalWrite(Z_DIR_PIN, LOW);

  pinMode(Z_STEP_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly

  int motorsReleased = 0;
  motorsReleased = digitalRead(X_MIN_PIN);
  if (motorsReleased == 1)
  {
     digitalWrite(X_ENABLE_PIN, LOW); 
     digitalWrite(Y_ENABLE_PIN, LOW); 
     digitalWrite(Z_ENABLE_PIN, LOW); 
  }
  else
  {
    digitalWrite(X_ENABLE_PIN, HIGH); 
    digitalWrite(Y_ENABLE_PIN, HIGH); 
    digitalWrite(Z_ENABLE_PIN, HIGH); 
    Serial.println("Tryb reczny");
    delay(100);
  }
  
  // Odczyt wartości krańcówki X (w kolejnych liniach Y i Z)
  xMaxPinValue = digitalRead(X_MAX_PIN);
  //  Serial.print("Wartosc krancowki X:");
  //  Serial.println(xMaxPinValue);
  //
  yMaxPinValue = digitalRead(Y_MAX_PIN);
  //  Serial.print("Wartosc krancowki Y:");
  //  Serial.println(yMaxPinValue);
  //
  zMaxPinValue = digitalRead(Z_MAX_PIN);
  //  Serial.print("Wartosc krancowki Z:");
  //  Serial.println(zMaxPinValue);
  //  Serial.println();
  //  delay(500);

  // delayMicroseconds(1000) to 1 ms narastania i 1 ms opadania zbocza, jeden ruch silnika to 2 ms, więc w 1 sekundę wykonamy 500 ruchów.

  // Warunki logiczne odpowiedzialne za ruch silnika jeżeli nie dojechał on do krańcówki, HIGH na tej zmiennej rozpoczyna zbocze narastające odpowiedzialne za wyzwolenie ruchu 
  // x_steps-- to odejmowanie kroków, przy zejściu do zera silnik się zatrzymuje
  if (xMaxPinValue == 0 && x_steps > 0)
  {
    digitalWrite(X_STEP_PIN, HIGH);
    x_steps--;
  }
 
  if (yMaxPinValue == 0 && y_steps > 0)
  {
    digitalWrite(Y_STEP_PIN, HIGH);
    y_steps--;
  }
  if (zMaxPinValue == 0 && z_steps > 0)
  {
    digitalWrite(Z_STEP_PIN, HIGH);
    z_steps--;
  }
  // Opóźnienie regulujące prędkość poruszania się wózka
  delayMicroseconds(1000);

  // Zakończenie zbocza narastającego
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  delayMicroseconds(1000);

  //Wybór w terminalu ilości kroków do wykonania z klawiatury, działa po wysłaniu komendy przez terminal
  while(Serial.available()>0)
  {
    //Wczytanie wartości do odpowiednich zmiennych
    s = Serial.readStringUntil('\n');
    sscanf(s.c_str(),"X:%d Y:%d Z:%d",&x_steps, &y_steps, &z_steps);
    moving = true;

    // Jezeli wartosc jest mniejsza od zera, zmienia kierunek ruchu i ustawia wartosc na dodatnia
    if (x_steps < 0)
    {
      digitalWrite(X_DIR_PIN, HIGH);
      x_steps = -x_steps;
    }
    else
    {
      digitalWrite(X_DIR_PIN, LOW);
    }

    if (y_steps < 0)
    {
      digitalWrite(Y_DIR_PIN, HIGH);
      y_steps = -y_steps;
    }
    else
    {
      digitalWrite(Y_DIR_PIN, LOW);
    }

    if (z_steps < 0)
    {
      digitalWrite(Z_DIR_PIN, HIGH);
      z_steps = -z_steps;
    }
    else
    {
      digitalWrite(Z_DIR_PIN, LOW);
    }
    
  }

  if (moving == true && x_steps == 0 && y_steps == 0 && z_steps == 0)
  {
    moving = false;
    Serial.println("OK");
  }
  
}
