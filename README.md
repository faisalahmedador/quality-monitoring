# IOT-water-quality-monitoring
It is a IOT system that detects water quality using sensors like PH sensor,turbidity sensor,temperature sensor etc. 
The euipments needed for this are : 
                                    
                                    1.  Arduino Uno
                                    2.  Nodemcu (wifi module)[ESP-12F]
                                    3.  Waterproof temperature sensor (ds18b20)[SEN-00072]
                                    4.  Waterproof turbidity sensor [SEN-00205]
                                    5.  Waterproof pH sensor [TOL-00163]
                                    6.  Jumper Wire
                                    7.  12V or greater external power source
                                    8.  470 Ohm 1W Resistor  
                                    
First the sensors are connected to Arduino Uno. Here 2 sensors are analog and 1 is digital. The analog sensors are needed to serial controlling via the modules given with the sensors. The digital sensor i.e. temperature sensor can connect directly to arduino but a 470 Ohm resistor should be connected to the output wire of the arduino.                                    
Connection details are given: 



                              A) Arduino & pH module
                              pH module           Arduino
                              VCC                 5V    
                              Grd                 Grd
                              P0                  A5
                              
                              B)Arduino & turbidity module
                              turbidity module           Arduino
                              VCC                         5V    
                              Grd                         Grd
                              Output                      A0
                              
                              C)Arduino & temperature sensor
                              turbidity module                                    Arduino
                              RED wire                                              5V    
                              BLACK wire                                            Grd
                              YELLOW wire(via 470 Ohm resistor)                     PWN 2
                              
                              D)Arduino & nodemcu
                              nodemcu                Arduino
                              Rx                       Tx    
                              Tx                        Rx
                              Grd                     grd
                              
                              
                              
                              
