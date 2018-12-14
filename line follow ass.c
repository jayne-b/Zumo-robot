#if 1
//line follow assignment
void zmain(void)
{
    motor_start();
    struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    TickType_t start;
    TickType_t end;
    int time;
    
       
    uint8 button;
    
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
              
        } while(!(dig.l3 == 1 && dig.r3 == 1));	// move forward until line is read
        {
        motor_forward(0,0);
    }
        print_mqtt("Zumo025/ready", "line");
        IR_flush();
        IR_wait();
        start = xTaskGetTickCount();
        print_mqtt("Zumo025/start", "%d", start);
        motor_forward(100,250);
        
        do
        {
            reflectance_read(&ref);
            reflectance_digital(&dig);
            if((dig.l1 == 1 && dig.r1 == 1))
        {
            motor_forward(150,1);
        } else if(dig.l1 == 1 && dig.r1 == 0)
        {
            motor_turn(10,190,1);
        } else if(dig.l1 == 0 && dig.r1 == 1)
        {
            motor_turn(190,10,1);
        }  
        }
       while(!(dig.l3 == 1 && dig.r3 ==1));		// first black line is read
    
    motor_forward(150,150);
    
        do{
         reflectance_read(&ref);
         reflectance_digital(&dig);
         motor_forward(150,0);
        
        }
    while(!(dig.l3 == 1 && dig.r3 == 1));		// second black line is read
    {     
        
        motor_forward(0,500);
        end = xTaskGetTickCount();
        print_mqtt("Zumo025/stop", "%d", end);
        time = end - start;
        print_mqtt("Zumo025/time", "%d", time);		// calculate time and print
        
    }
        
    
}}}
#endif