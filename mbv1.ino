int unidades = 0; // varible de la unidades
int decenas = 0; // variable de las decenas

/***BOTONES***/
int pdwon = 31; 
int pup = 34;
/***********/

/***TECLADO***/
bool asterisco = false; // Para el control del asterisco
// Filas
int f0 = 42;
int f1 = 43;
int f2 = 44;
int f3 = 45;
// Columnas
int c0 = 49;
int c1 = 48;
int c3 = 47;
/*************/

int f = 200; // frecuencia inicial
int sc = 0 // selector de leds y columnas {0-->c0 y led1 || 1-->c1 y led2 || 2-->c2}

void contadorPositivo(){
  if(unidades == 9 && decenas == 9){
    // RESET
    unidades = 0; 
    decenas = 0; 
  } else {
    if(unidades == 9){
      unidades = 0; 
      decenas++; // se incrementa las decenas
    } else {
      unidades++;
    }
  }  
}

void contadorNegativo(){
  if(unidades !=0  || decenas != 0){
    if(unidades == 0){
      unidades = 9; 
      decenas--; // se decrementa las decenas
    } else {
      unidades--;
    }
  }  
}

void vaciar(int cn){
  //digitalWrite(cn, LOW);
  PORTA = 0; 
  digitalWrite(cn, HIGH); // Desactivamos
}

void mostrar(int n){
  switch(n){
    case 0:
      PORTA = 0x3f;
      break;
    case 1:
      PORTA = 0x06;
      break;
    case 2:
      PORTA = 0x5b;
      break;
    case 3:
      PORTA = 0x4f;
      break;
    case 4:
      PORTA = 0x66;
      break;
    case 5:
      PORTA = 0x6d;
      break;
    case 6:
      PORTA = 0x7d;
      break;
    case 7:
      PORTA = 0x07; 
      break;
    case 8:
      PORTA = 0x7f;
      break;
    case 9:
      PORTA = 0x6f;
      break;
  }
}

void Barrido(){
  if(sc == 0){
    if(asterisco){
        if(digitalRead(f0) == LOW)f = 200;
        else if(digitalRead(f1) == LOW)f = 800;
        else if(digitalRead(f2) == LOW)f = 1400;
        asterisco = false;
      } else {
        if(digitalRead(f3) == LOW){
          asterisco = true;
        }
      }
  } else if(sc == 1){
      if(asterisco){
        if(digitalRead(f0) == LOW)f = 400;
        else if(digitalRead(f1) == LOW)f = 1000;
        else if(digitalRead(f2) == LOW)f = 1600;
        else if(digitalRead(f3) == LOW)f = 0;
        asterisco = false;
      } 
  } else {
      if(asterisco){
        if(digitalRead(f0) == LOW)f = 600;
        else if(digitalRead(f1) == LOW)f = 1200;
        else if(digitalRead(f2) == LOW)f = 1800;
        asterisco = false;
      }
  }
}

void ISR_timer(){
  noInterrupts(); // Suspende las interrupciones
  if(sc == 0){
    digitalWrite(c0, LOW);
    mostrar(unidades);
    Barrido(); 
    sc = 1;
  } else if(sc == 1){
    vaciar(c0);
    digitalWrite(c1, LOW);
    mostrar(decenas);
    Barrido();
    sc = 2;
  } else {
    vaciar(c1);
    digitalWrite(c2, LOW);
    Barrido();
    sc = 0;
  }
  interrupts();                 // Autoriza las interrupciones

}
void setup() {
  DDRC = B00000001; // Definimos el PORTC de entrada salvo PC0 (salida) 
  PORTC= B11111000; // activamos el pull-up interno de las líneas entrada PC7-PC3  
  
  DDRL = B00001111; // Definimos el PORTL 
  PORTC= B11110000; // activamos el pull-up interno de las líneas entrada PL7-PL4
  
  DDRA = B10000000;  // PROBAR INICIARLOS DE ENTRA NO DE SALIDA (LEDS)
  
  // *****************************************************************
  // disable interrupts
  cli();
  // modo normal de funcionamiento
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; // cuenta inicial a cero
  // mode CTC
  TCCR1B |= (1<<WGM12);
  // prescaler N = 1024
  TCCR1B |= (1<<CS12)|(1<<CS10);
  // fintr = fclk/(N*(OCR1A+1)) --> OCR1A = [fclk/(N*fintr)] - 1
  // para fintr = 100Hz --> OCR1A = [16*10^6/(1024*100)] -1 = 155,25 --> 155

  OCR1A = 77; // para 200 Hz programar OCR1A = 77
  // enable timer1 compare interrupt
  TIMSK1 |=(1<<OCIE1A);
  // habilitamos interrupciones
  sei();
  // *****************************************************************
}
// Para la rutina de servicio del timer
ISR(TIMER1_COMPA_vect){
    ISR_timer();
}
void loop() {
  if(digitalRead(pup) == LOW && digitalRead(pdwon) == LOW){
    // RESET
    unidades = 0; 
    decenas = 0; 
    tone(37, f, 5);
    delay(100);
  } else {
    if(digitalRead(pup) == LOW && digitalRead(pdwon) == HIGH){
        //while(digitalRead(pup) == LOW){}
        contadorPositivo();
        tone(37, f, 5);
        delay(100);
     } else {
        if(digitalRead(pdwon) == LOW && digitalRead(pup) == HIGH){
          //while(digitalRead(pdwon) == LOW){}
          contadorNegativo();
          tone(37, f, 5);
          delay(100);
        }
    }
  }  
}
