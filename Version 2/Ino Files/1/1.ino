//designed for an arduino mega
//pair numbers are for a step signal and an idd number are for a sens signal

#define first_pin 22
#define number_of_motor 4

//Those values are in stepps, not in degrees
int order[4] = {1600, 1600, 1600, 1600};
int actual_position [4]= {1600, 1600, 1600, 1600};
//the speed is the equivalent of 
long motor_period[4]={500, 1000, 1000, 1000};
unsigned long previous_motor_timer[4] = {0, 0, 0, 0};

//Full str message from the pc
String data ;
long data_number ;
unsigned int data_indice ;



int sign(float value)
{
    if(value > 0)return 1;
    else return 0;
}

bool read_serial()
{
    char msg = Serial.read();
    if(msg == '\n')
    {
        data_number = data.toInt();
        data_indice = data_number & 15 ;
        data_number = (data_number-data_indice)>>4 ;
        Serial.println(data_number);
        data = "";
        return true;
    }
    else 
    {
        data += msg ;
        return false ;
    }
}

void setup()
{
    //Initialize our output pin for our motors
    for(int i = 0; i<2*number_of_motor; i++)pinMode(first_pin+i, OUTPUT);

    digitalWrite(25, HIGH);
    Serial.begin(9600);
    
}

void loop()
{

    //Check for some data to read 
    if(Serial.available())
    {
        if(read_serial())
        {
            //Data are available to be processed
            if(data_indice<4)order[data_indice]=data_number;        
            else if(data_indice<8)motor_period[data_indice-4]=data_number;
        }
    }
    
    
    //Here is where we ask the motor to moove    
    for(int m = 0; m < number_of_motor; m++)
    {
        float error = order[m] - actual_position[m];
        if(error)
        {
            if(micros()>previous_motor_timer[m]+motor_period[m])
            //Here we moove
            {
                digitalWrite(first_pin+1+2*m, sign(error));
                digitalWrite(first_pin+2*m, HIGH);
                digitalWrite(first_pin+2*m, LOW);
                previous_motor_timer[m] = micros();
                if(sign(error))actual_position[m] +=1 ;
                else actual_position[m]-=1;

            }
        }
    }
}