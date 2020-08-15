#include "Arduino.h"
#include "Math.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "Keypad.h"
#include "DHT.h"
#include "NewPing.h"

boolean h=0;                                                                         //parador                                                             
byte linhas[4]={13,12,11,10},colunas[3]={9,8,7};                                     //array pinos linhas e colunas display
byte k,j;                                                                            //contadores dos pontos de impressão do display
char teclas[4][3]={{'1','2','3'},{'4','5','6'},{'7','8','9'},{'-','0','#'}};         //array definições das teclas
int e=1,umi;                                                                         //"umi" variavel dht, "e" variavel de sinal
float a,b,c,d,x1=0,x2=0,temp;  
String l="",i="";
unsigned long timer=0;

#define i2c 0x3F
#define vertical 20 
#define horizontal 4
#define dhtp 4
#define ECHO 5
#define TRIG 6

Keypad teclado=Keypad(makeKeymap(teclas),linhas,colunas,4,3);
LiquidCrystal_I2C lcd(i2c,vertical,horizontal);
DHT dht(dhtp, DHT11);
NewPing sonar(TRIG,ECHO,999);

void setup(){
    dht.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
        menu:                                    //inicio menu e inicio programa
        l="";
        while(l!="1"||l!="2"||l!="3"){
            lcd.setCursor(8,0);
            lcd.print("Menu");
            lcd.setCursor(0,1);
            lcd.print("(1)Dia");
            lcd.setCursor(0,2);
            lcd.print("(2)Calculadora");
            lcd.setCursor(0,3);
            lcd.print("(3)Distancia");
            l=teclado.getKey();
            if(l=="1"){
                lcd.clear();
                goto dia;
            }
            else if(l=="2"){
                lcd.clear();
                goto calc;
            }
            else if(l=="3"){
                h=0;
                lcd.clear();
                goto sensor;
            }
        }                                       //fim menu
        dia:
        l="";temp=0;umi=0;                     //inicio relogio
        while(l!="#"){
            if ((millis() - timer) > 1000){
                temp = dht.readTemperature();
                umi = dht.readHumidity();
                timer=millis();
                lcd.setCursor(8,0);
                lcd.print("Dia");
                lcd.setCursor(0,1);
                lcd.print("Temp:     C  Umi:  %");
                lcd.setCursor(5,1);
                lcd.print(temp);
                lcd.setCursor(17,1);
                lcd.print(umi);
                lcd.setCursor(0,2);
                lcd.print("16:39:20  26/08/2020");
                lcd.setCursor(0,3);
                lcd.print("Segunda");
                lcd.setCursor(13,3);
                lcd.print("(#)Menu"); 
            }
            l=teclado.getKey();
            if(l=="#"){
                    lcd.clear();
                    goto menu;
            }
        }                                       // fim relogio
        calc:                                   //inicio menu calc
        l="";
        while(l!="1"||l!="2"||l!="3"||l!="4"||l!="#"){
            lcd.setCursor(8,0);
            lcd.print("Calc");
            lcd.setCursor(0,1);
            lcd.print("(1)% (2)2Grau (3)Tri");
            lcd.setCursor(0,2);
            lcd.print("(4)Soma & Subtracao");
            lcd.setCursor(13,3);
            lcd.print("(#)Menu");
            l=teclado.getKey();
            if(l=="1"){
                lcd.clear();
                goto porcentagem;
            }
            else if(l=="2"){
                lcd.clear();
                goto grau;
            }
            else if(l=="3"){
                lcd.clear();
                goto trigo;
            }
            else if(l=="4"){
                lcd.clear();
                //goto soma;
            }
            else if(l=="#"){
                lcd.clear();
                goto menu;
            }
        }                                       //fim menu calc
        porcentagem:                            //inicio porcentagem 
        l="";i="";k=3;j=1;a=0;b=0;c=0;h=0;
        while(h==0){                           
            lcd.setCursor(4,0);
            lcd.print("Porcentagem");
            lcd.setCursor(0,1);
            lcd.print("% = ");
            lcd.setCursor(0,5);
            l=teclado.getKey();
            if(l!=NO_KEY){
                if(l!="#"&&l!="-"){
                    k=k+1;
                    lcd.setCursor(k,j);
                    lcd.print(l);
                    i=i+l;
                }
                else if(l=="#"&&j==1){
                    lcd.setCursor(0,2);
                    lcd.print("de = ");
                    a=i.toFloat();
                    k=4;j=j+1;i="";
                }
                else if(l=="#"&&j==2){
                    b=i.toFloat();
                    c=(a*b)/100;                 //Contribuição da Bianca a braba do arduino
                    h=1;
                    lcd.clear();
                    lcd.setCursor(4,0);
                    lcd.print("Porcentagem");
                    lcd.setCursor(0,1);
                    lcd.print("Resultado:");
                    lcd.setCursor(0,2);
                    lcd.print(c);
                    lcd.setCursor(0,3);
                    lcd.print("(*)Calc   (#)Repetir");
                }
            }                                      
        }                                       
        while(h==1){
            l=teclado.getKey();
            if(l=="#"){
                lcd.clear();
                goto porcentagem;
            }
            if(l=="-"){
                lcd.clear();
                goto calc;
            }
        }                                       //fim porcentagem
        grau:                                   //inicio grau
        k=3;j=1;a=0;b=0;c=0;d=0;e=1;h=0;x1=0;x2=0;l="";i="";
        while(h==0){                            
            lcd.setCursor(3,0);
            lcd.print("Equacao 2 grau");
            lcd.setCursor(0,1);
            lcd.print("a = ");
            l=teclado.getKey();
            if(l!=NO_KEY){
                if(l!="#"&&l!="-"){
                    k=k+1;
                    lcd.setCursor(k,j);
                    lcd.print(l);
                    i=i+l;
                }
                else if(l=="-"){
                    k=k+1;
                    lcd.setCursor(k,j);
                    lcd.print(l);
                    e=-1;
                }
                else if(l=="#"&&j==1){
                    lcd.setCursor(0,2);
                    lcd.print("b = ");
                    a=i.toFloat();
                    a=a*e;
                    k=3;j=j+1;i="";e=1; 
                }
                else if(l=="#"&&j==2){
                    lcd.setCursor(0,3);
                    lcd.print("c = ");
                    b=i.toFloat();
                    b=b*e;
                    k=3;j=j+1;i="";e=1;
                }
                else if(l=="#"&&j==3){
                    c=i.toFloat();
                    c=c*e;
                    d=(pow(b, 2))-4*a*c;
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Delta = ");
                    lcd.setCursor(8,0);
                    lcd.print(d);
                    if(d<0){
                        h=1;
                        lcd.setCursor(0, 1);
                        lcd.print("Impossivel calcular");
                        lcd.setCursor(0, 3);
                        lcd.print("(*)Calc   (#)Repetir");
                    }
                    else{
                        h=1;
                        x1=(b*-1+sqrt(d))/(2*a);
                        x2=(b*-1-sqrt(d))/(2*a);
                        lcd.setCursor(0,1);
                        lcd.print("X1 = ");
                        lcd.setCursor(5,1);
                        lcd.print(x1);
                        lcd.setCursor(0,2);
                        lcd.print("X2 = ");
                        lcd.setCursor(5,2);
                        lcd.print(x2);
                        lcd.setCursor(0,3);
                        lcd.print("(*)Calc   (#)Repetir");
                    }
                }
            }
        }
        while(h==1){
        l=teclado.getKey();
            if(l=="#"){
                lcd.clear();
                goto grau;
            }
            if(l=="-"){
                lcd.clear();
                goto calc;
            }
        }                                       //fim grau
        trigo:                                  //inicio menu trigonometria
        while(l!="1"||l!="2"||l!="3"||l!="#"){
            lcd.setCursor(3,0);
            lcd.print("Trigonometria");
            lcd.setCursor(0,1);
            lcd.print("(1)Seno");
            lcd.setCursor(0,2);
            lcd.print("(2)Cosseno");
            lcd.setCursor(0,3);
            lcd.print("(3)Tangente (#)Menu");
            l=teclado.getKey();
                if(l=="1"){
                    h=1;l="";
                    lcd.clear();
                    goto seno;
                }
                else if(l=="2"){}
                else if(l=="3"){}
                else if(l=="#"){
                    lcd.clear();
                    goto menu;
                }
        }                                       //fim menu trigonometria
        seno:                                   //inicio seno
        l="";i="";k=7;a=0;h=0;
        while(h==0){
            lcd.setCursor(3,0);
            lcd.print("Trig-Seno");
            lcd.setCursor(0,1);
            lcd.print("Angulo:");
            l=teclado.getKey();
            if(l!=NO_KEY){
                if(l!="#"&&l!="-"){
                    k=k+1;
                    lcd.setCursor(k,1);
                    lcd.print(l);
                    i=i+l;
                }
                else if(l=="#"){
                    a=i.toFloat();
                    a=sin(a);
                    lcd.setCursor(0,2);
                    lcd.print("Seno=");
                    lcd.setCursor(6,2);
                    lcd.print(a);
                    lcd.setCursor(0, 3);
                    lcd.print("(*)Calc   (#)Repetir");
                    h=1;
                }
            }
        }
        while(h==1){
        l=teclado.getKey();
            if(l=="#"){
                lcd.clear();
                goto seno;
            }
            if(l=="-"){
                lcd.clear();
                goto calc;
            }
        }                                         //fim seno
    sensor:                                      //inicio sensor
    l="";a=0;
    while(l!="#"){                   
        if ((millis()-timer)>1000){
            a=(sonar.ping_median(10));
            a=(sonar.convert_cm(a));
            timer=millis();
            lcd.setCursor(8,0);
            lcd.print("Sensor");
            lcd.setCursor(0,1);
            lcd.print("Distancia = 0000");
            if(a<10){
                lcd.setCursor(15,1);
                lcd.print(a);  
            }
            else if(a>10&&a<99){
                lcd.setCursor(14,1);
                lcd.print(a); 
            }
            else{
                lcd.setCursor(13,1);
                lcd.print(a); 
            }
            lcd.setCursor(17,1);
            lcd.print("cm");
            lcd.setCursor(13,3);
            lcd.print("(#)Menu");
        }
        l=teclado.getKey();
        if(l=="#"){
            lcd.clear();
            goto menu;
        }
    }                                       //fim sensor
}                                        //fim programa
void loop(){}
