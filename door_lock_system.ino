#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <String.h>

SoftwareSerial SMS(7, 8);

char admin_phone_number[]={'+','9','2','0','0','0','0','0','0','0','0','0','0','\0'};
char general_pin[]={'0','0','0','0','\0'};

byte KeypadRow[]={13,12,11,10};// Keypad Rows connected to Arduino pins
byte KaypadColumn[]={9,6,5,4};//Keypad Columns connected to Arduino pins

const byte row = 4;
const byte col = 4;

char keyMap[row][col] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad key = Keypad(makeKeymap(keyMap),KeypadRow,KaypadColumn,row,col);

int adminWrongCheck ;
int userWrongCheck ;
LiquidCrystal lcd(A5,A4,A3,A2,A1,A0); //tells with which pins LCD is Connected

int phone(void);
void pin(void);
void admin(void);
void user(void);
bool verify(void);
int random_number(void);
void Message(int num);

void setup()
{
  randomSeed(analogRead(0));
  adminWrongCheck = 0;
  userWrongCheck = 0;
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Welcome");
  
  SMS.begin(19200);   // the GPRS baud rate    
  Serial.begin(19200);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digital Security");
  lcd.setCursor(5,1);
  lcd.print("System");
  delay(3000);
  phone();
  pin();  
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. Admin");
  lcd.setCursor(0,1);
  lcd.leftToRight();
  lcd.print("2. Open Door");
  char digit_input='\0';
  digit_input = key.waitForKey();
  switch(digit_input)
  {
      case '1':
        admin();
        break;
      case '2':
        user();
        break;
  }
}

int phone(void)
{
  do
  {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Enter Mobile No#");
     lcd.setCursor(0,1);
     lcd.leftToRight();
     lcd.print('0');
     lcd.print('3');
     admin_phone_number[0] = '+';
     admin_phone_number[1] = '9';
     admin_phone_number[2] = '2';
     admin_phone_number[3] = '3';
     int string_cnt=4;
     admin_phone_number[string_cnt] = '\0';  
     do
     {
      char digit_input='\0';
      digit_input = key.waitForKey();
      if(digit_input=='C')
      {
         break;
      }
      else if(digit_input=='A' && string_cnt==13)
      {
         return 0;
      }
      else if(digit_input!='A' && digit_input!='B' && digit_input!='D' && digit_input!='*' && digit_input!='#' && string_cnt<13)
        {
          lcd.print(digit_input);
          admin_phone_number[string_cnt] = digit_input;
          string_cnt++;
          admin_phone_number[string_cnt] = '\0';
        }
      }while(true);
  }while(true); 
}

void pin(void)
{
  do
  {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Enter PIN Code#");
     lcd.setCursor(0,1);
     lcd.leftToRight();
     int string_cnt=0;
     general_pin[string_cnt] = '\0';  
     do
     {
      char digit_input='\0';
      digit_input = key.waitForKey();
      if(digit_input=='C')
         break;
      else if(digit_input=='A' && string_cnt==4)
         return 0;
      else if(digit_input!='A' && digit_input!='B' && digit_input!='D' && digit_input!='*' && digit_input!='#' && string_cnt<4)
        {
          lcd.print("*");
          general_pin[string_cnt] = digit_input;
          string_cnt++;
          general_pin[string_cnt] = '\0';
        }
      }while(true);
  }while(true);
}

void admin(void)
{
  if(verify()){
    char digit_input='\0';
    do{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1. Change Phone");
      lcd.setCursor(0,1);
      lcd.leftToRight();
      lcd.print("2. Change Pin");
      
      digit_input = key.waitForKey();
      switch(digit_input)
      {
          case '1':
            phone();
            break;
          case '2':
            pin();
            break;
      }
    }while(digit_input!='1'&& digit_input!='2' && digit_input !='C');
  }
  else
  {
    if(adminWrongCheck == 3)
    {
      MessageWrong(adminWrongCheck+1);
      adminWrongCheck = 0;
    }
    else
      adminWrongCheck ++ ; 
  }
}

int random_number(void)
{
  
 // random number from 1111 to 9999
  return random(1111,10000);
}

bool verify(void)
{
  int random_num = random_number();
  Message(random_num);
  char num[] = {'0','0','0','0','\0'};
  do
  {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Enter Code : ");
     lcd.setCursor(0,1);
     lcd.leftToRight();
     int string_cnt=0;
     num[string_cnt] = '\0';  
     do
     {
      char digit_input='\0';
      digit_input = key.waitForKey();
      if(digit_input=='C')
         break;
      else if(digit_input=='A' && string_cnt==4)
          return atoi(num)==random_num;
      else if(digit_input!='A' && digit_input!='B' && digit_input!='D' && digit_input!='*' && digit_input!='#' && string_cnt<4)
        {
          lcd.print(digit_input);
          num[string_cnt] = digit_input;
          string_cnt++;
          num[string_cnt] = '\0';
        }
      }while(true);
  }while(true);
}

void Message(int num)
{
    String first = "AT + CMGS = \"";
    String last = "\"";
    first+=admin_phone_number;
    first+=last;
    String msg = "Digital Security System Code : ";
    msg+=num;
    
    SMS.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
    delay(100);
    SMS.println(first);
    delay(100);
    SMS.println(msg);//the content of the message
    delay(100);
    SMS.println((char)26);//the ASCII code of the ctrl+z is 26
    delay(100);
    SMS.println();
}

void MessageWrong(int num)
{
    String first = "AT + CMGS = \"";
    String last = "\"";
    first+=admin_phone_number;
    first+=last;
    String msg = "Someone tried to enter into system illegally. Attempt tired : ";
    msg+=num;
    
    SMS.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
    delay(100);
    SMS.println(first);
    delay(100);
    SMS.println(msg);//the content of the message
    delay(100);
    SMS.println((char)26);//the ASCII code of the ctrl+z is 26
    delay(100);
    SMS.println();
}

bool verifyPin(void)
{
  char num[] = {'0','0','0','0','\0'};
  do
  {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Enter Code : ");
     lcd.setCursor(0,1);
     lcd.leftToRight();
     int string_cnt=0;
     num[string_cnt] = '\0';  
     do
     {
      char digit_input='\0';
      digit_input = key.waitForKey();
      if(digit_input=='C')
         break;
      else if(digit_input=='A' && string_cnt==4){
            if(strcmp(num,general_pin)==0)
              return true;
            else
              return false;
              
        }
      else if(digit_input!='A' && digit_input!='B' && digit_input!='D' && digit_input!='*' && digit_input!='#' && string_cnt<4)
        {
          lcd.print('*');
          num[string_cnt] = digit_input;
          string_cnt++;
          num[string_cnt] = '\0';
        }
      }while(true);
  }while(true);
}
void user(void)
{
  if(verifyPin()){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.leftToRight();
    lcd.print("Door Unlocked");
    char digit_input='\0';
    do
    {
      digit_input = key.waitForKey();
      switch(digit_input)
      {
          case 'A':
          for(int i=10;i>0;i--){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.leftToRight();
            lcd.print("Door Lock in: ");
            lcd.print(i);
            delay(1000);
          }
            break;
      }
    }while(digit_input!='A');
  }
  else
  {
    if(userWrongCheck == 3)
    {
      MessageWrong(userWrongCheck+1);
      userWrongCheck = 0;
    }
    else
      userWrongCheck ++ ; 
  }
}
