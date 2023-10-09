#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int btnInicio = 0;
int btnIzquierdo = 8; int btnCentro = 12; int btnDerecho = A1;
//              R  Verde    Azul
int tira1[3] = {9, 10, 11};
int tira2[3] = {2, 3, 4};
int tira3[3] = {5, 6, 7};
// int led1Rojo=9; int led1Verde=10; int led1Azul=11;
// int led2Rojo=2; int led2Verde=3; int led2Azul=4;
// int led3Rojo=5; int led3Verde=6; int led3Azul=7;
// configuacion para leds rgb anodo común
int colores[3][3] =
{
  {1, 1, 0},
  {1, 0, 1},
  {0, 1, 1}
};
int  generarLuces()
{
  int rnd1 = random(0, 2 + 1);
  digitalWrite(tira1[0], colores[rnd1][0]);
  digitalWrite(tira1[1], colores[rnd1][1]);
  digitalWrite(tira1[2], colores[rnd1][2]);
  int rnd2 = random(0, 2 + 1);
  while (rnd2 == rnd1)
  {
    rnd2 = random(0, 2 + 1);
  }
  digitalWrite(tira2[0], colores[rnd2][0]);
  digitalWrite(tira2[1], colores[rnd2][1]);
  digitalWrite(tira2[2], colores[rnd2][2]);
  int rnd3 = random(0, 2 + 1);
  while (rnd3 == rnd2 || rnd3 == rnd1)
  {
    rnd3 = random(0, 2 + 1);
  }
  digitalWrite(tira3[0], colores[rnd3][0]);
  digitalWrite(tira3[1], colores[rnd3][1]);
  digitalWrite(tira3[2], colores[rnd3][2]);
  if (rnd1 == 2) return 0;
  else if (rnd2 == 2) return 1;
  else if (rnd3 == 2) return 2;

}
bool lcdOk = true;
int contadorAcertado = 0;
int contadorFallas = 0;
void parteIncicioLCD()
{
  String mensaje = "TRABAJO FINAL PULSE EL BOTON PARA INICIAR";
  mensaje += +" "+mensaje.substring(0,15+1);
  //  trabajo final P";

  unsigned long tiempo1;
  bool mTiempo1 = true;
  int i = 0;
  while (true)
  {
    if (mTiempo1)
    {
      tiempo1 = millis();
      mTiempo1 = false;
    }
    if (millis() - tiempo1 > 200)
    {
      lcd.setCursor(0, 0);
      String otro = mensaje.substring(i, i + 16);
      lcd.print(otro);
      i++;
      if (i >= mensaje.length() - 15) i = 0;
      lcd.setCursor(0, 1);
      // lcd.clear();
      lcd.print("-------------------");
      mTiempo1 = true;
    }
    if (!digitalRead(btnInicio)) break;
    delay(5);
  }
  lcd.setCursor(0, 0);
  lcd.print("    INICIANDO      ");
  lcdOk = true;
  contadorAcertado = 0;
  contadorFallas = 0;
}
void apagarLeds()
{
  for (int i = 2 ; i <= 7; i++)
    digitalWrite(i, HIGH);
  for (int i = 9 ; i <= 11; i++)
    digitalWrite(i, HIGH);
}
void setup()
{
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  randomSeed(analogRead(0));
  for (int i = 2 ; i <= 7; i++)
    pinMode(i, OUTPUT);
  for (int i = 9 ; i <= 11; i++)
    pinMode(i, OUTPUT);
  pinMode(btnInicio, INPUT_PULLUP);
  pinMode(btnIzquierdo, INPUT_PULLUP);
  pinMode(btnCentro, INPUT_PULLUP);
  pinMode(btnDerecho, INPUT);
  Serial.begin(9600);
  parteIncicioLCD();
  // delay(500);
}

int queryRedLed;
bool estadoLed = true;
bool yesCorrecto = false;

unsigned long tiempo2;
bool pulsa1 = true, pulsa2 = true, pulsa3 = true;
bool retrasado = false;
void loop() {
  // put your main code here, to run repeatedly:

  if (estadoLed)
  {
    tiempo2 = millis();
    estadoLed = false;
    // queryRedLed= generarLuces();
  }
  if (millis() - tiempo2 >= 3000 || yesCorrecto)
  {
    if (lcdOk)
    {
      lcd.setCursor(0, 0);
      lcd.print("      OK           ");
      lcdOk = false;
    }
    if (yesCorrecto)
    {
      lcd.setCursor(0, 0);
      lcd.print("acertados:" + String(contadorAcertado) + "       ");
      lcd.setCursor(0, 1);
      lcd.print("fallados: " + String(contadorFallas) + "         ");
      if ((contadorAcertado + contadorFallas) >= 10)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" PROMEDIO FINAL      ");
        lcd.setCursor(0, 1);
        lcd.print(String("Acertado:") +
                  ((contadorAcertado * 100.0) / (contadorAcertado + contadorFallas)) +
                  String("%            "));
        Serial.println(String("Acertado:") +
                       ((contadorAcertado * 100.0) / (contadorAcertado + contadorFallas)) +
                       String("%            "));
        apagarLeds();
        Serial.print("acertados:" + String(contadorAcertado) + "       ");
        Serial.println("fallados: " + String(contadorFallas) + "         ");
        delay(3000);

        parteIncicioLCD();
      }
    }
    else if (retrasado)
    {
      Serial.println("Tiempo limite superado, eres lento");
      contadorFallas++;
      lcd.setCursor(0, 0);
      lcd.print("acertados:" + String(contadorAcertado) + "       ");
      lcd.setCursor(0, 1);
      lcd.print("fallados: " + String(contadorFallas) + "         ");
      if ((contadorAcertado + contadorFallas) >= 10)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" PROMEDIO FINAL      ");
        lcd.setCursor(0, 1);
        lcd.print(String("Acertado:") +
                  ((contadorAcertado * 100.0) / (contadorAcertado + contadorFallas)) +
                  String("%            "));
        Serial.println(String("Acertado:") +
                       ((contadorAcertado * 100.0) / (contadorAcertado + contadorFallas)) +
                       String("%            "));
        apagarLeds();
        Serial.print("acertados:" + String(contadorAcertado) + "       ");
        Serial.println("fallados: " + String(contadorFallas) + "         ");
        delay(3000);

        parteIncicioLCD();
      }
    }
    if (!retrasado)
    {
      retrasado = true;
    }

    queryRedLed = generarLuces(); estadoLed = true;
    yesCorrecto = false;
    // Serial.println(queryRedLed);
  }
  if (!digitalRead(btnIzquierdo))
  {
    if (pulsa1)
    {
      pulsa1 = false;
      if (queryRedLed == 0)
      {

        Serial.print("Correcto acertado led1 en rojo: ");
        contadorAcertado++;
        Serial.println(contadorAcertado);
      }
      else
      {
        Serial.print("Fallaste con pulsador1: ");
        contadorFallas++;
        Serial.println(contadorFallas);
      }
      Serial.println("Tiempo de reación es: " + String(millis() - tiempo2) + " milisegundos");
      yesCorrecto = true;

    }
  }
  else
  {
    pulsa1 = true;
  }
  if (!digitalRead(btnCentro) )
  {
    if (pulsa2)
    {
      pulsa2 = false;
      if (queryRedLed == 1)
      {

        Serial.print("Correcto acertado led2 en rojo: ");
        contadorAcertado++;
        Serial.println(contadorAcertado);

      }
      else
      {
        Serial.print("Fallaste con pulsador 2: ");
        contadorFallas++;
        Serial.println(contadorFallas);
      }
      Serial.println("Tiempo de reación es: " + String(millis() - tiempo2) + " milisegundos");
      yesCorrecto = true;
    }
  }
  else
  {
    pulsa2 = true;
  }
  // Serial.println(digitalRead(btnDerecho));
  if (!digitalRead(btnDerecho))
  {
    if (pulsa3)
    {
      pulsa3 = false;
      if (queryRedLed == 2)
      {
        Serial.print("Correcto acertado led3 en rojo: ");
        contadorAcertado++;
        Serial.println(contadorAcertado);
      }
      else
      {
        Serial.print("Fallaste con pulsador 3: ");
        contadorFallas++;
        Serial.println(contadorFallas);
      }
      Serial.println("Tiempo de reación es: " + String(millis() - tiempo2) + " milisegundos");
      yesCorrecto = true;
    }
  }
  else
  {
    pulsa3 = true;
  }

  delay(10);
}
