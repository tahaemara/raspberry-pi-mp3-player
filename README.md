

                         Author: Taha Emara



   Youtube Cahnnel  :  http://www.youtube.com/user/omar0103637

   Facebook Page    :  https://www.facebook.com/IcPublishes

   Linkedin Profile :  http://eg.linkedin.com/pub/taha-emara/a4/1ab/524/

   E-mail:          :  tahaemara.eng@gmail.com

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                      About This Project

                      Implementation of MP3 player using Raspberry Pi

   This program allows you play mp3 files on a USB flash drive.
   
   steps of operations:
   
     * check existence of usb flash drive.
     
     * scan the contents of the drive for mp3 files
     
     * play individual files according to your control.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   Board Type : Raspberry Pi B rev2

   OS Type    : Raspbian



   you must install :          

    *sudo apt-get install mpg321

    *sudo apt-get install libusb-1.0-0-dev

    and also install wiringPi library http://wiringpi.com/download-and-install/

   

 

  to compile this project with gcc compiler type in terminal:

    "gcc -Wall mp3.c vector.c usbcheck.c getpath.c -lpthread -lusb-1.0 -lwiringPi -o mp3"




  to make program run on startup :

   type in terminal "sudo nano /etc/rc.local" 

   and add this line to this file " sudo /path/to/folder/mp3 & "  in my case " sudo /home/pi/mp3player/mp3 &"


Free for non commercial use as long as this entire copyright notice is included or not in source code and any other documentation .
   
