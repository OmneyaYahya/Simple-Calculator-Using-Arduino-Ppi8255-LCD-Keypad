const int a0 = A4;// a0 of ic connected with A4 in arduino
const int a1 = A5;// a1 of ic connected with A5 in arduino
const int WR = A3;// WR of ic connected with A3 in arduino
const int RD = A2;// RD of ic connected with A2 in arduino

double result = 0.0;//result of operations
int symbol = '0';//character for symbols
int temp;//temp to save the value of pressed col
int counter = 0;
double num1 = 0;
double num2 = 0;


const byte row = 4;
const byte col = 4;
byte RowPins[row] = {0, 1, 2, 3}; //rows connected with pin 0 : 3 of 8255
byte ColPins[col] = {4, 5, 6, 7}; //columns connected with pin 4 : 7 of 8255


int keypad[row][col] = {'7', '8', '9', '/',
                        '4', '5', '6', '*',
                        '1', '2', '3', '-',
                        '.', '0', '=', '+'
                       };
void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
}

void fun_in() { //  pins  4 : 7 input from cols 
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);


}

void fun_out() {// pins  4 : 7 output on cols
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

}

////////////////////////////////////////////////////////////////////////////////////////////////
void ButtonPressed()
{
  // set all rows = 1
  for (int i = 0; i <= 3; i++)
  { 
    //select PortC
    digitalWrite(a0, 0); 
    digitalWrite(a1, 1);
    
    digitalWrite(RowPins[i], 1); //set a  row  = 1


    digitalWrite(WR, 0);// enable WR
    delay(1);
    digitalWrite(WR, 1);// disable WR

    digitalWrite(i, 0);// set selected row = 0 low
    
    digitalWrite(WR, 0);
    delay(1);
    digitalWrite(WR, 1);

    // to know the selected col
    for (int j = 4; j <= 7; j++)
    {

      fun_in(); 
      digitalWrite(RD, 0);// enable RD
      temp = digitalRead(j); //num of the pressed/selected col 
      // IF THERE IS AN ERROR WHEN YOU PRESS ANY KEY ON KEYPAD YOU CAN EDIT THE DELAY HERE TO BE COMPATABLE WITH YOUR PC
      delay(10);
      digitalWrite(RD, 1);//disable RD

      fun_out();

      if (temp == 0)
      {
        //if the pressed key is symbol ( * , + , - , / , = , . )
        if ((keypad[i][j - 4] == keypad[0][3]) | (keypad[i][j - 4] == keypad[1][3]) | (keypad[i][j - 4] == keypad[2][3]) | (keypad[i][j - 4] == keypad[3][3]) | (keypad[i][j - 4] == keypad[3][2]) | (keypad[i][j - 4] == keypad[3][0]))
        { 
          delay(1);
          if (keypad[i][j - 4] == '.') 
          {
            writeOnlcd(0x01,0);
            writeOnlcd(0x06,0);
            symbol = '0' ;
            num1 = 0 ;
            num2 = 0 ;
            st="";
            break;
          }  
          
       
          if (keypad[i][j - 4] == '=')
          {
            writeOnlcd('=',1);
            operation();
            symbol = '0' ;
            num1 = 0 ;
            num2 = 0 ;
            st="";
            break;
          }
          if(keypad[i][j - 4] == '-' && num1 ==0)
          {
            writeOnlcd('-',1);
            symbol='0';
            neg=true;
          }
          else
          {
            symbol = keypad[i][j - 4] ;
            st+=symbol;
            writeOnlcd(symbol,1);
          }
           
        }
        else
        {
          st+= (keypad[i][j - 4]);
          writeOnlcd(keypad[i][j - 4],1);
          delay(2);
          
          if (symbol == '0') 
          {
            num1 = ( num1 * 10 ) + ( keypad[i][j - 4]) - '0'  ; 
          }
          

         else
          {
            num2 = ( num2 * 10 ) +  (keypad[i][j - 4]) - '0'  ;
          }
          break;
        }
      }
    }
    digitalWrite(i, 1); //row=1 hy5ly al row be high 34an ylf fe al loop mn tany
    digitalWrite(WR, 0); //open write
    delay(1);
    digitalWrite(WR, 1); //close WR
  }

}

///////////////////////////////////////////////////////////////////////


void operation() //function to do operations
{
  
  if (symbol == '+')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    result = (num1 + num2);
    String str = String(result);
    displayOnLCD(str);
  }

  if (symbol == '-')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    result = (num1 - num2);
    String str = String(result);
    displayOnLCD(str);

  }
  if (symbol == '*')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    result = (num1 * num2);
    String str = String(result);
    displayOnLCD(str);
  }
  if (symbol == '/')
  {
    if (num2 == 0) {
      writeOnlcd('E',1);
      delay(1);
      num1 = 0;
      num2 = 0;
      symbol = '0';
    }
    else
    {
      if (neg == true){num1=num1*-1; neg=false;}
      result = (num1 / num2);
      String str = String(result);
      displayOnLCD(str);
    }

  }


}

void displayOnLCD(String msg) 
{
  for (int i = 0; i < 8; i++)
  {
    writeOnlcd(msg[i],1);
    delay(1);

  }
  delay(1);
}




//To send a command to LCD, RS=0  & To send Data to LCD, RS=1 
void writeOnlcd(int num , int rs) {
   //port B is selected
  digitalWrite(a0, 1);
  digitalWrite(a1, 0);
  for (int i = 0; i < 8; i++)//For loop to enter i to the pins of port B for ic
  {
    digitalWrite(i, (num >> i) & 0x01);// Here we make anding for i and 1 to take it to port b
  }
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
   //port A is selected
  digitalWrite(a0, 0); 
  digitalWrite(a1, 0);
  //to send command make E=1
  digitalWrite(6, rs);
  digitalWrite(7, 1);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
  //then make E=0
  digitalWrite(6, rs);
  digitalWrite(7, 0);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
}





void loop() {
  // put your main code here, to run repeatedly:
  //if condition to set control word
  if (counter == 0)
  {
    digitalWrite(RD, 1);   //READ DISABLED
    digitalWrite(WR, 1);  //WRITE DISABLED

    //Selecting Control Register of 8255
    digitalWrite(a0, 1); 
    digitalWrite(a1, 1);  

    digitalWrite(7, 1);  // active mode
    digitalWrite(6, 0);  //
    digitalWrite(5, 0);  //
    digitalWrite(4, 0);  // portA Output
    digitalWrite(3, 1);  // CU Input "cols"
    digitalWrite(2, 0);  //
    digitalWrite(1, 0);  // portB Output
    digitalWrite(0, 0);  // CL Output "rows"

    digitalWrite(WR, 0);//WRITE ENABLED
    delay(10);
    digitalWrite(WR, 1);//WRITE DISABLED

    writeOnlcd(0x38,0);//intialize lcd
    delay(30);
    writeOnlcd(0x0E,0);//display cursor
    delay(30);
    writeOnlcd(0x01,0);//clear lcd
    delay(30);
    writeOnlcd(0x06,0);//shift cursor right
    delay(30);
    counter = 1;
  }
  ButtonPressed();
}



