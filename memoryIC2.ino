const int LEE_SCL = 2; // puerto de entrada para leer el estado de la línea SCL
const int LEE_SDA = 3; // puerto de entrada para leer el estado de la línea SDA
const int ESC_SCL = 4; // puerto de salida para escribir el valor de la línea SCL
const int ESC_SDA = 5; // puerto de salida para escribir el valor de la línea SDA
int u2[7] = {1,0,1,0,1,0,0}; // Memoria para las mejoras
int t;  // Tamaño usado de la memoria

bool comenzar(){
  bool start = false;
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
  if(digitalRead(LEE_SDA) == HIGH && digitalRead(LEE_SCL) == HIGH)start = true; 
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, HIGH);
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, LOW);
  return start;
}

void parar(){
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, LOW);
  
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, HIGH);
  
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
  
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
}

void eBit1(){
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, LOW);
  
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
  
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
   
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, LOW);
  
}
void eBit0(){
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, LOW);

  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, HIGH);
  
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, HIGH);
  
  digitalWrite(ESC_SDA, LOW);
  digitalWrite(ESC_SCL, LOW);
}
bool leerAck(){
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, LOW);

  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);
  if(digitalRead(LEE_SDA) == LOW && digitalRead(LEE_SCL) == HIGH)return true; 
  

  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, LOW);
  return false; 
}
int Rbit(){
  while(true){
    digitalWrite(ESC_SDA, HIGH);
    digitalWrite(ESC_SCL, LOW);

    digitalWrite(ESC_SDA, HIGH);
    digitalWrite(ESC_SCL, HIGH);
    if(digitalRead(LEE_SCL) == HIGH)return digitalRead(LEE_SDA); 

    digitalWrite(ESC_SDA, HIGH);
    digitalWrite(ESC_SCL, LOW);
  }
}
void pasarVector(int vec[], int t){ 
  if(t == 1 || t == 0){
    vec[7] = t; 
  } else {
    int div = t; 
    int res = 0; 
    int i = 7; 
    while(div != 1){
      res = div%2; 
      div = div/2; 
      vec[i] = res; 
      i--; 
    }
    vec[i] = div;
  } 
}
int pasarEntero(int vec[], int t){
  int res = 0; 
  int exp = 0; 
  int pot = 0; 
  for(int i = t-1; i > -1; i--){
    exp = 0.5 + pow(2, pot); 
    res += vec[i] * exp;
    pot += 1; 
  }
  return res; 
}

void EscribirEnMemoria(int n, int dir){
  int vecNum[8] = {0,0,0,0,0,0,0,0};
  int vecDir[8] = {0,0,0,0,0,0,0,0};
  pasarVector(vecNum, n);
  pasarVector(vecDir, dir);
  espACK0:
  while(!comenzar()){}; 
  // Enviamos dirección del esclavo
  for(int i = 0; i < 7; i++){
    if(u2[i] == 0){
      eBit0(); 
    } else{
      eBit1();
    }
  }
  eBit0();  // RW == 0
  if(!leerAck())goto espACK0; 
  // Envaimos direción para el puntero
  espACK1:
  for(int i = 0; i < 8; i++){
    if(vecDir[i] == 0){
      eBit0(); 
    } else{
      eBit1();
    } 
  }
  if(!leerAck())goto espACK1;
  // Enviamos el dato
  espACK2:
  for(int i = 0; i < 8; i++){
    if(vecNum[i] == 0){
        eBit0(); 
     } else{
        eBit1();
     }
  }
  if(!leerAck())goto espACK2;
  parar(); 
  
}
int leerMemoria(int dir){
  int vecDir[8] = {0,0,0,0,0,0,0,0};
  int res[8] = {0,0,0,0,0,0,0,0};
  pasarVector(vecDir, dir);
  // Enviamos direción del esclavo
  espACK0:
  while(!comenzar()){}; 
 // Enviamos dirección del esclavo
  for(int i = 0; i < 7; i++){
    if(u2[i] == 0){
      eBit0(); 
    } else{
      eBit1();
    }
  }
  eBit0();  // RW == 0
  if(!leerAck())goto espACK0; 
  // Enviamos dirección de lectura
  espACK1:
  for(int i = 0; i < 8; i++){
    if(vecDir[i] == 0){
        eBit0(); 
     } else{
        eBit1();
     }
  }
  if(!leerAck())goto espACK1;
  while(!comenzar()){}
  // Enviamos direción esclavo
  espACK2:
  for(int i = 0; i < 7; i++){
    if(u2[i] == 0){
        eBit0(); 
     } else{
        eBit1();
     }
  }
  eBit1(); // RW == 1
  if(!leerAck())goto espACK2;
  // Recogemos el dato
  for(int i = 0; i < 8; i++){
    if(Rbit() == HIGH){
        res[i] = 1; 
    } else {
        res[i] = 0; 
    }
  }
  eBit1();  // Enviamos el No AKC
  parar(); 
  int valor = pasarEntero(res, 8); 
  return valor; 
}

void setup() {
 t = 0; 
 // Inicialización del canal serie para comunicarse con el usuario
 Serial.begin(9600);
 // Inicialización de los terminales de entrada
 pinMode(LEE_SDA, INPUT);
 pinMode(LEE_SCL, INPUT);
// Inicialización de los terminales de salida
 pinMode(ESC_SDA, OUTPUT);
 pinMode(ESC_SCL, OUTPUT);
// Asegurarse de no intervenir el bus poniendo SDA y SCL a "1"....
 digitalWrite(ESC_SDA, HIGH);
 digitalWrite(ESC_SCL, HIGH);

}
void loop() {
  end: 
  int puntero = 0; 
  if (Serial.available() > 0) {
  // lee el byte de entrada:
  puntero = Serial.read();
  if(puntero == 42){  // Si puntero=*, se vacia la memoria
      t = 0; 
      Serial.println("\n[Memoria limpia]\n");
      goto end; 
    }
    if(puntero == 199){ // Si puntero=Ç, se imprime la memoria
      for(int k = 0; k < t; k++){
        char ch = (char)leerMemoria(k);  
        Serial.print(ch);
      }
      Serial.print("\n");
    } else if(t < 128){
      EscribirEnMemoria(puntero, t);
      t++; 
    } else {
      Serial.println("La memoria esta llena");
    }
  }
}