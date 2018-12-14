#if 1
   //sumo ass 
 
void zmain(void)
{
    motor_start();
    struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    struct accData_ data;
    LSM303D_Start();
    TickType_t start;
    TickType_t end;
    TickType_t now;
    int time;
    
    int button;          
     
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
              
        } while(!(dig.l3 == 1 && dig.r3 == 1));		// move to line
        
        motor_forward(0,0);
        print_mqtt("Zumo023/ready", "zumo");
        IR_flush();
        IR_wait();
        start = xTaskGetTickCount();
        print_mqtt("Zumo023/start", "%d", start);
        motor_forward(150,250);
        
        for(;;)
        {
        reflectance_read(&ref);
        reflectance_digital(&dig); 
        LSM303D_Read_Acc(&data);


        if((dig.l3 == 1 || dig.l2 == 1 || dig.l1 == 1 || dig.r3 == 1 || dig.r2 == 1 || dig.r1 == 1))		// if any sensor reads black
        {
            motor_backward(150,1);
            motor_turn(200,0,300);
        }
        else 
        {
            motor_forward(200,0);
        }
        
        if (data.accY < -6000)		// if zumo is hit from front
        {
            motor_forward(250,150);
            now = xTaskGetTickCount();
            print_mqtt("Zumo023/hit", "%d", now);
        } else if (data.accX < -6000)		// if zumo is hit from side
        {
            motor_backward(150,1);
            motor_turn(200,0,300);
            now = xTaskGetTickCount();
            print_mqtt("Zumo023/hit", "%d", now);
        }
        
        button = SW1_Read(); // read SW1 on pSoC board
        if(button == 0)
        {
            motor_forward(0,0);
            end = xTaskGetTickCount();
            print_mqtt("Zumo023/stop", "%d", end);
            time = end - start;
            print_mqtt("Zumo023/time", "%d", time);
            break;
        }
            
}}}}
#endif