int unidades = 0; // varible de la unidades
int decenas = 0;  // variable de las decenas
int centenas = 0; // variable de las centenas
int milesimas = 0;// variable de las milesimas
bool AUTO = false;

/***BOTONES***/
int pdwon = 31; 
int pup = 34;
int pright = 30;
int pleft = 32;
int penter = 33;
/***********/

/***TECLADO***/
bool asterisco = false; // Para el control del asterisco (*)
bool numeral = false; // Para el control del numeral (#)
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
int sc = 0 // selector de leds y columnas {0-->c0 y led1 || 1-->c1 y led2 || 2-->c2 y led3 || 3-->led4}
/*Variables mejoras*/
bool cuenta = true; // Activa modo auto+ o auto-
int tiempo = 1000; // El tiempo de retardo(Al incio debe estar a 5000)


void contadorPositivo(){
  if(unidades == 15 && decenas == 15 && centenas == 15 && milesimas == 15){
    // RESET
    unidades = 0; 
    decenas = 0; 
    centenas = 0;
    milesimas = 0;
  } else if(unidades == 15 && decenas == 15 && centenas == 15){
    	unidades = 0; 
    	decenas = 0; 
    	centenas = 0;
    	milesimas++; // se incrementa las milesimas
  } else if(unidades == 15 && decenas == 15){
      unidades = 0; 
    	decenas = 0; 
    	centenas++;// se incrementa las centenas
    }else if(unidades == 15){
    	unidades = 0; 
      decenas++; // se incrementa las decenas
    } else {
    	unidades++;// se incrementa las unidades
    } 
}

void contadorNegativo(){
  if(unidades != 0 || decenas != 0 || centenas != 0 || milesimas != 0){
    if(unidades == 0 && decenas == 0 && centenas == 0){
    	unidades = 15; 
    	decenas = 15; 
    	centenas = 15;
    	milesimas--; // se decrementa las milesimas
    } else if(unidades == 0 && decenas == 0){
      unidades = 15; 
    	decenas = 15; 
    	centenas--;// se decrementa las centenas
    }else if(unidades == 0){
    	unidades = 15; 
    	decenas--; // se decrementa las decenas
    } else {
    	unidades--;
    }  
  } 
}
//Para indicar el cambio NORMAL-AUTO
void cambio_Auto(){
  noInterrupts(); // Suspende las interrupciones
  // Vaciamos lo anterior en el display
  digitalWrite(c0, LOW);
  vaciar(c0);
  digitalWrite(c1, LOW);
  vaciar(c1);
  digitalWrite(c2, LOW);
  vaciar(c2);
  digitalWrite(c3, LOW);
  vaciar(c3);
  /******************/
  digitalWrite(c0, LOW);
  PORTA = 0x71;
  tone(37, 349, 500);
  vaciar(c0);
  digitalWrite(c1, LOW);
  PORTA = 0x71;
  tone(37, 330, 500);
  vaciar(c1);
  digitalWrite(c2, LOW);
  PORTA = 0x71;
  tone(37, 294, 500);
  vaciar(c2);
  digitalWrite(c3, LOW);
  PORTA = 0x71;
  tone(37, 262, 500);
  vaciar(c3);
  interrupts();                 // Autoriza las interrupciones
}

//Para indicar el cambio AUTO-NORMAL
void cambio_Normal(){
  noInterrupts(); // Suspende las interrupciones
  // Vaciamos lo anterior en el display
  digitalWrite(c0, LOW);
  vaciar(c0);
  digitalWrite(c1, LOW);
  vaciar(c1);
  digitalWrite(c2, LOW);
  vaciar(c2);
  digitalWrite(c3, LOW);
  vaciar(c3);
  /******************/
  digitalWrite(c3, LOW);
  PORTA = 0x3f;
  tone(37, 262, 500);
  vaciar(c3);
  digitalWrite(c2, LOW);
  PORTA = 0x3f;
  tone(37, 294, 500);
  vaciar(c2);
  digitalWrite(c1, LOW);
  PORTA = 0x3f;
  tone(37, 330, 500);
  vaciar(c1);
  digitalWrite(c0, LOW);
  PORTA = 0x3f;
  tone(37, 349, 500);
  vaciar(c0);
  interrupts();                 // Autoriza las interrupciones
  
}

// Controlador de botones en Modo Normal
void modoNormal(){
   if(digitalRead(pup) == LOW && digitalRead(pdwon) == LOW){
    // RESET
    unidades = 0; 
    decenas = 0; 
    tone(37, f, 5);
    delay(100);
  } else {
    if(digitalRead(pup) == LOW && digitalRead(pdwon) == HIGH){
        //while(digitalRead(pup) != LOW){}
        contadorPositivo();
        tone(37, f, 5);
        delay(100);
     } else {
        if(digitalRead(pdwon) == LOW && digitalRead(pup) == HIGH){
          //while(digitalRead(pdwon) != LOW){}
          contadorNegativo();
          tone(37, f, 5);
          delay(100);
        } else {
          if(digitalRead(penter) == LOW){
            AUTO = true; 
            cambio_Auto();
          }
        }
    }
  }
}

// El controlador de los Botones del Modo AUTO
void controladorBotones(){
  if(digitalRead(penter) == LOW){
    AUTO = false; 
    cambio_Normal();
  }else if(digitalRead(pright) == LOW){
    tiempo += 1000;
    delay(100);
  } else if(digitalRead(pleft) == LOW){
    tiempo -= 1000;
    delay(100);
  }else if(digitalRead(pup) == LOW){
    cuenta = true;
    delay(100);
  } else if(digitalRead(pdwon) == LOW){
    cuenta = false;
    delay(100);
  }
}

// Rutina del modo Auto
void modoAuto(){
  delay(tiempo);
  if(cuenta){
    contadorPositivo();
    //tone(37, f, 5);
  }else{ 
    contadorNegativo();
    //tone(37, f, 5);
  }  
}

// Para vaciar los display
void vaciar(int cn){
  //digitalWrite(cn, LOW);
  PORTA = 0; 
  digitalWrite(cn, HIGH); // Desactivamos
}

// Muestra en los display
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
    case 10:
      PORTA = 0x37;
      break;
    case 11:
      PORTA = 0x7c;
      break;
    case 12:
      PORTA = 0x39;
      break;
    case 13:
      PORTA = 0x3f;
      break;
    case 14:
      PORTA = 0x79;
      break;
    case 15:
      PORTA = 0x71;
      break;
  }
}

// Algoritmo de Barrido para el teclado
void Barrido(){
  if(sc == 0){
    if(asterisco && !numeral){
        if(digitalRead(f0) == LOW)f = 200;
        else if(digitalRead(f1) == LOW)f = 800;
        else if(digitalRead(f2) == LOW)f = 1400;
        asterisco = false;
    } else if(!asterisco && numeral){
        if(digitalRead(f0) == LOW)f = 294;
        else if(digitalRead(f1) == LOW)f = 392;
        else if(digitalRead(f2) == LOW)f = 277;
        numeral = false;
    } else {
        if(digitalRead(f3) == LOW){
          asterisco = true;
        }
      }
  } else if(sc == 1){
      if(asterisco && !numeral){
        if(digitalRead(f0) == LOW)f = 400;
        else if(digitalRead(f1) == LOW)f = 1000;
        else if(digitalRead(f2) == LOW)f = 1600;
        else if(digitalRead(f3) == LOW)f = 0;
        asterisco = false;
      } else if(!asterisco && numeral){
        if(digitalRead(f0) == LOW)f = 330;
        else if(digitalRead(f1) == LOW)f = 440;
        else if(digitalRead(f2) == LOW)f = 311;
        else if(digitalRead(f3) == LOW)f = 262;
        numeral = false;
      }
  } else {
      if(asterisco && !numeral){
        if(digitalRead(f0) == LOW)f = 600;
        else if(digitalRead(f1) == LOW)f = 1200;
        else if(digitalRead(f2) == LOW)f = 1800;
        asterisco = false;
      } else if(!asterisco && numeral){
        if(digitalRead(f0) == LOW)f = 349;
        else if(digitalRead(f1) == LOW)f = 494;
        else if(digitalRead(f2) == LOW)f = 370;
        numeral = false;
      } else {
        if(digitalRead(f3) == LOW){
          numeral = true;
        }
      }
  }
}

//La rutina que implementa el automata de control mejorado
void ISR_timer(){
  noInterrupts(); // Suspende las interrupciones
  if(sc == 0){
    vaciar(c3);
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
  } else if(sc == 2){
    vaciar(c1);
    digitalWrite(c2, LOW);
    mostrar(centenas);
    Barrido();
    sc = 3;
  } else {
    vaciar(c2);
    digitalWrite(c3, LOW);
    mostrar(centenas);
    sc = 0;
  }
  if(AUTO){
    controladorBotones();
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

// Selector dual
void loop() {
  if(!AUTO){
    modoNormal();
  } else{
    modoAuto();
  }
}

