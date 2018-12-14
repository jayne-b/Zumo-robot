#if 1
    
// maze assignment


void zmain(void)
{
    motor_start();
    struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    TickType_t start;
    TickType_t end;
    int time;
    
    int x = 0, y = -1, dir = 0, button, line = 0, i = 0;           // dir = direction, d = distance
     
    reflectance_start();
    reflectance_set_threshold(13000, 13000, 16000, 16000, 13000, 13000); // set center sensor threshold 
    
    
     for(;;)
    {
        // read raw sensor values
        reflectance_read(&ref);
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        
        vTaskDelay(1);
        button = SW1_Read(); // read SW1 on pSoC board
        if(button == 0){
       
        do {
        motor_forward(100,0);
        reflectance_read(&ref);
        reflectance_digital(&dig); 
              
        } while(!(dig.l3 == 1 && dig.r3 == 1));		// move to first line
        
        motor_forward(0,0);
        print_mqtt("Zumo025/ready", "maze");
        IR_flush();
        IR_wait();
        start = xTaskGetTickCount();
        print_mqtt("Zumo025/start", "%d", start);
        motor_forward(100,250);
        
        do {
            reflectance_read(&ref);
            reflectance_digital(&dig);
            int d = Ultra_GetDistance();
           
        if((dig.l1 == 1 && dig.r1 == 1))        //line follow loop
        {
            motor_forward(90,1);
        } else if(dig.l1 == 1 && dig.r1 == 0)
        {
            motor_turn(0,75,1);
        } else if(dig.l1 == 0 && dig.r1 == 1)
        {
            motor_turn(75,0,1);
        } 
         
        if(dig.l3 == 1 && dig.r3 == 1)      //if black line read
        {
            i = 1;
            d = Ultra_GetDistance();
            
        if(line == 2 && dir == 1)       //passed 2 black lines and facing right
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(0,240,300);
            line = 0;
            dir--;
        } else if(line == 2 && dir == -1)       //passed 2 black lines and facing left
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(250,0,340);
            line = 0;
            dir++;
        } else if(d<22 && dir == 1)        //sees object facing right
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(0,240,300);
            line = 0;
            dir--;
        } else if(d<22 && dir == -1)        //sees object facing left
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(250,0,340);
            line = 0;
            dir++;
        } else if (d<22 && dir == 0 && x >= 1)   //sees object facing forward and is on the right side of grid
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(0,240,300);
            line = 0;
            dir--;
        } else if(d<22 && dir == 0 && x <= 0)  //and on left side of grid or centre
        {
            motor_backward(50,100);
            motor_forward(100,1);
            motor_turn(250,0,340);
            line = 0;
            dir++;
        }
        
        } else if(dig.l3 == 0 && dig.r3 == 0 && i == 1)         //counts lines and x and y dir
        {
            line++;
            if(dir == 0)
            {
                y++;
                print_mqtt("Zumo025/position", "%d %d", x, y);
            } else if(dir == -1)
            {
                x--;
                print_mqtt("Zumo025/position", "%d %d", x, y);
            } else if(dir == 1)
            {
                x++;
                print_mqtt("Zumo025/position", "%d %d", x, y);
            }
            i = 0;
        }
        if(y > 10 && x >= 1 && dir == 0 && dig.l3 == 1 && dig.r3 == 0)       //robot is in end section, facing forward on right side of grid
            {
                motor_backward(50,100);
                motor_forward(100,1);
                motor_turn(0,240,300);
                line = 0;
                dir--;
            } else if(y > 10 && x <= -1 && dir == 0 && dig.l3 == 0 && dig.r3 == 1)       //facing forward on left side of grid
            {
                motor_backward(50,100);
                motor_forward(100,1);
                motor_turn(250,0,335);
                line = 0;
                dir++;
            } else if(y > 10 && x == 0 && dir == 1)     //facing right
            {
                motor_backward(50,100);
                motor_forward(100,1);
                motor_turn(0,255,325);
                line = 0;
                dir--;
            } else if(y > 10 && x == 0 && dir == -1)        //facing left
            {
                motor_backward(50,100);
                motor_forward(100,1);
                motor_turn(255,0,325);
                line = 0;
                dir++;
            }
            
               if(dig.l1 == 0 && dig.l2 == 0 && dig.l3 == 0 && dig.r1 == 0 && dig.r2 == 0 && dig.r3 == 0)		// if all sensors read white
            {
                motor_forward(0,0);
                end = xTaskGetTickCount();
                print_mqtt("Zumo025/stop", "%d", end);
                time = end - start;
                print_mqtt("Zumo025/time", "%d", time);
                motor_stop();
                break;
            }
        
                      
        } while(y<14);
        
   
    
}}}

               
#endif