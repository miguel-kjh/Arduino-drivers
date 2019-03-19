int unidades = 0; // varible de la unidades
int decenas = 0; // variable de las decenas
int centenas = 0;
int milesimas = 0;

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
      	milesimas++; 
    } else if(unidades == 15 && decenas == 15){
        unidades = 0; 
    	decenas = 0; 
    	centenas++;
    }else if(unidades == 15){
    	unidades = 0; 
      	decenas++; // se incrementa las decenas
    } else {
    	unidades++;
    } 
}

void contadorNegativo(){
  if(unidades != 0 || decenas != 0 || centenas != 0 || milesimas != 0){
    if(unidades == 0 && decenas == 0 && centenas == 0){
    	unidades = 15; 
    	decenas = 15; 
    	centenas = 15;
      	milesimas--; 
    } else if(unidades == 0 && decenas == 0){
        unidades = 15; 
    	decenas = 15; 
    	centenas--;
    }else if(unidades == 0){
    	unidades = 15; 
      	decenas--; // se decrementa las decenas
    } else {
    	unidades--;
    }  
  } 
}

void setup() { 
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
// lee el byte de entrada:
	int puntero = Serial.read();
    if(puntero == 43){
      	contadorPositivo();
      	Serial.print(milesimas, HEX);
      	Serial.print(centenas, HEX);
      	Serial.print(decenas, HEX);
      	Serial.print(unidades, HEX);
      	Serial.print("\n");
    } else if(puntero == 45) {
    	contadorNegativo();
      	Serial.print(milesimas, HEX);
      	Serial.print(centenas, HEX);
      	Serial.print(decenas, HEX);
      	Serial.print(unidades, HEX);
      	Serial.print("\n");
    }
  
  
  }
}
 