 /* 
  *  Uso del I2C bus con la EEPROM 24LC64 
  * Ejemplo:    eeprom.pde
  * Este código pertenece a:
  * Autor: kans
  * Fecha: 05/03/2008
  * 
  *   http://pastebin.com/f3a36644b
  *   http://hispavila.com/3ds/atmega/i2c-eeprom.html
  */
       
  #include <Wire.h> //librería I²C
       
  // Las siguientes funciones para lectura y escritura en una EEPROM 
  // se encuentran en el wiki de Arduino: http://www.arduino.cc/playground/Code/I²CEEPROM
  
  void I2C_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) 
  {
   int rdata = data;
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddress >> 8));    // MSB
   Wire.write((int)(eeaddress & 0xFF));  // LSB
   Wire.write(rdata);
   Wire.endTransmission();
  }
       
  
  //ADVERTENCIA: address es una dirección de página, de 6 bits termina envolverá alrededor además, los
  // datos pueden ser como máximo alrededor 30 byte, porque la librería Wire tiene un búfer de 32 byte
  void I2C_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, 
  byte* data,  byte length ) 
  {
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddresspage >> 8));   // MSB
   Wire.write((int)(eeaddresspage & 0xFF)); // LSB
   byte c;
   for ( c = 0; c < length; c++)
   Wire.write(data[c]);
   Wire.endTransmission();
  }
       
  byte I2C_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) 
  {
   byte rdata = 0xFF;
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddress >> 8));   // MSB
   Wire.write((int)(eeaddress & 0xFF)); // LSB
   Wire.endTransmission();
   Wire.requestFrom(deviceaddress,1);
   if (Wire.available()) rdata = Wire.read();
   return rdata;
  }
       
 // tal vez no hay que leer más de 30 o 32 bytes de una vez!
  void I2C_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, 
  byte *buffer, int length ) 
  {
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddress >> 8));   // MSB
   Wire.write((int)(eeaddress & 0xFF)); // LSB
   Wire.endTransmission();
   Wire.requestFrom(deviceaddress,length);
   int c = 0;
   for ( c = 0; c < length; c++ )
   if (Wire.available()) buffer[c] = Wire.read();
  }       
       
   void setup() 
  {
   char cadena[] = "hola mundo desde una eeprom"; // cadena a escribir
   Wire.begin(); // es obligatorio inicializar la conexion
   Serial.begin(9600);
   //I2C_eeprom_write_page(0x50, 0, (byte *)cadena, sizeof(cadena)); // escribir la cadena al 
   // principio de la EEPROM; comentar esta línea para probar que la memoria es no volátil
   delay(10); //pequeña pausa despues de escribir en la memoria
  }
       
  void loop() 
  {
  int addr=0; //dirección a leer
   byte b = I2C_eeprom_read_byte(0x50, 0); // acceso a la primera posición de memoria
       
   while (b!=0) 
  {
   Serial.print((char)b); //enviar al ordenador
   addr++; //siguiente direccion
   b = I2C_eeprom_read_byte(0x50, addr); //acceso a posición de memoria
  }
   Serial.println();
   delay(2000);
  }
