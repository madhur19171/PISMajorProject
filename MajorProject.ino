// 100: Fault, 200: Tank empty, 10: OK, 50: ResetRequest
int x, t, t2, t3, y;
bool fault = false;
bool empty = false;
bool humandetect = false;
bool off = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(4, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(7, INPUT);//Human Detection
  pinMode(6, INPUT_PULLUP);//Reset Button
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  Serial.begin(9600);
  x = y = 0;
  t = 0;
  t2 = 0;
  t3 = 0;
  Serial.print("10");
}

void loop() {
  // put your main code here, to run repeatedly:
  x = analogRead(A0);
  y = analogRead(A1);
  if (!fault && !empty)
  {
    if (x > 900)
      t++;

    if (t > 50)
    {
      t2 = 5; //Emptying
    }

    if (t2 == 5)
    {
      if (x < 500)
        t3++;
    }

    if (t3 > 50)
    {
      t = t2 = t3 = 0;
      Serial.print(" 10");
    }

    if (t > 300)
    {
      if (delayCheckA1(2000, 10))
        empty = true;
      else
        fault = true;
      t = t2 = t3 = 0;
    }
    Serial.print(" 10");
  }
  if (fault && !empty)
  {
    if (digitalRead(7) == 1)
      humandetect = true;
    if (humandetect == true)
      Serial.print(" 300");
    else
      Serial.print(" 100");
    if (digitalRead(6) == 0)
    {
      for (int i = 0; i < 600; i++)
      {
        Serial.print(" 50");
        delay(10);
      }
      delay(4000);
      for (int i = 0; i < 100; i++)
      {
        Serial.print(" 40");
        delay(10);
      }
      t = t2 = t3 = 0;
      empty = humandetect = false;
    }
  }
  if (!fault && empty)
  {
    if (delayCheckA1(2000, 10))
      Serial.print(" 200");
    else
    {
      t = t2 = t3 = 0;
      fault = empty = humandetect = false;
      digitalWrite(3, LOW);
    }
  }
  if(fault || empty)
    digitalWrite(3, HIGH);
  delay(100);
}
bool delayCheckA1(int t, int d)
{
  int c = 0;
  for (int i = 0; i < (int)(t / d); i++)
  {
    int y = analogRead(A1);
    if (y > 900)
      c++;
    delay(d);
  }
  if (c > (t / d) / 3)
    return true;
  else
    return false;
}
