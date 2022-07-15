// sensor de temperatura:  tmp > 24°C <--> analogRead(A0) > 151
//					       tmp < 18°C <--> analogRead(A0) < 141
//
// LDR: noite <--> analogRead(A1) < 1000

bool presenca = 0, morador = 0;
float seno;
int frequencia;
 

bool alarme(){
	digitalWrite(10, HIGH);
	for(int x=0;x<180;x++){
  		seno=(sin(x*3.1416/180));
  		frequencia = 2000+(int(seno*2000));
  		tone(9,frequencia);
  		if(x==90){
  			digitalWrite(10, LOW);
    		digitalWrite(11, HIGH);
  		}
     if(digitalRead(8) == LOW){
      	return !digitalRead(8);
     }
  		delay(1);
 	}
  	digitalWrite(11,LOW);
  return 0;
}

void setup() {
	
  Serial.begin(9600);
	pinMode(2, INPUT);	// sensor de movimento
	pinMode(3, OUTPUT);	// relé 1: rádio
	pinMode(4, OUTPUT);	// relé 2: aquecedor
  pinMode(5, OUTPUT);	// relé 3: ar condicionado
  pinMode(6, OUTPUT);	// relé 4: lâmpada
  pinMode(7, INPUT_PULLUP);	// botão de ativação da segurança
  pinMode(8, INPUT_PULLUP);	// botão de desativação da segurança
 	pinMode(9,OUTPUT);		// buzzer
 	pinMode(10, OUTPUT);	// led do alarme
 	pinMode(11, OUTPUT);	// led do alarme
}
 

void loop() {
  
  	Serial.println(analogRead(A0));
  
  	if(digitalRead(8) == LOW){		// botão para o morador
      digitalWrite(6, LOW);       // desliga lâmpada	se estiver acesa
    	morador = 1;				        // (segurança desativada)
    }  
  
  	if(digitalRead(2)){	          // detecção de movimento
    	presenca = 1;
  	}  
  
  	if(presenca && morador){
    	digitalWrite(3, HIGH);  	  // liga rádio
   		
    	if(analogRead(A0) >= 151){	// se tiver quente
    		  digitalWrite(5, HIGH);	// liga ar condicionado
          digitalWrite(4, LOW);   // desliga aquecedor
  		}
    	if(analogRead(A0) <= 141){	// se tiver frio
    		  digitalWrite(4, HIGH);	// liga aquecedor
          digitalWrite(5, LOW);   // desliga ar condicionado
        }
      if(analogRead(A0) > 141 && analogRead(A0) < 151){	// se o clima estiver ameno
         	digitalWrite(4, LOW);	  // desliga aquecedor
         	digitalWrite(5, LOW);   // desliga ar condicionado
    	}
    	if(analogRead(A1) < 1000){	// se tiver escuro
    		digitalWrite(6, HIGH);	// liga lâmpada
    	}
      	if(analogRead(A1) >= 1000){	// se tiver claro
    		digitalWrite(6, LOW);	// desliga lâmpada
    	}
  	}
  
  	if(presenca && !morador){
      if(analogRead(A1) < 1000){// se tiver escuro
    	digitalWrite(6, HIGH);	// liga lâmpada
   		}
    	while(!morador){
          	morador = alarme();	// aciona o alarme
          
          	if(morador){			
    			noTone(9);			// 	alarme desativado
              	digitalWrite(10,LOW);
                digitalWrite(11,LOW);//
  			}
    	}  
  	}

    while(presenca){
      if(!digitalRead(2)){            // detecção de movimento
      presenca = 0;
      delay(5);                       // 5 seg para desligar as coisas
    } 
    }
  	if(digitalRead(7) == LOW){		// morador saindo
    	presenca = 0;				// (segurança ativada)
      	morador = 0;
    	digitalWrite(3, LOW);
  		digitalWrite(4, LOW);
  		digitalWrite(5, LOW);
  		digitalWrite(6, LOW);
  	}
}
