////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              Author: Taha Emara
//
//   Youtube Cahnnel  :  http://www.youtube.com/user/omar0103637
//   Facebook Page    :  https://www.facebook.com/IcPublishes
//   Linkedin Profile :  http://eg.linkedin.com/pub/taha-emara/a4/1ab/524/
//   E-mail:          :  tahaemara.eng@gmail.com
//
//                              About This Project
//   Implementation of MP3 player which reads tracks from usb flash drive.
//   This program lets you store files on a USB flash drive, scan the contents of the drive for mp3 files, play individual files.
//
//   Board Type : Raspberry Pi B rev2
//   OS Type    : Raspbian
//
//   you must install :                
//    *sudo apt-get install mpg321
//    *sudo apt-get install libusb-1.0-0-dev
//    and also install wiringPi library http://wiringpi.com/download-and-install/
//   
// 
//  to compile this project with gcc compiler type in terminal:
//    "gcc -Wall mp3.c vector.c usbcheck.c getpath.c -lpthread -lusb-1.0 -lwiringPi -o mp3"
//
//  to make program run on startup :
//   type in terminal "sudo nano /etc/rc.local" 
//   and add this line to this file " sudo /path/to/folder/mp3 & "  in my case " sudo /home/pi/mp3player/mp3 &"
//   
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include"vector.h"
#include"getpath.h"
#include"usbcheck.h"
#include<pthread.h>
#include<wiringPi.h>
#include<signal.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   pthread_t t1,t2,t3;
   int play_flag=1;
   char* new_arr;
   char*newr;
   vector v;
   char *path_arr;
   volatile int list_iterator=1;
   int flag=1;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// this function responsible for scanning for mp3 files and save paths of thes file to vector ///////////  

void scanMp3(void){
  FILE *fp;
  char path[1035];
  vector_init(&v);
     /* Open the command for reading. */
  size_t len=strlen("find ")+strlen(usbPath())+strlen(" -type f -name \"*.mp3\"")+1;
  path_arr=malloc(len);
  strcpy(path_arr,"find ");
  strcat(path_arr,usbPath());
  strcat(path_arr," -type f -name \"*.mp3\"");
  //fp = popen("find /media/MYLINUXLIVE -type f -name \"*.mp3\"  ", "r");
  fp=popen(path_arr,"r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  int i=0;
  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("%s", path);
    vector_add(&v,path);
 }
 //printf("from mp3 li %d t-li %s\n",list_iterator,(char*)vector_get(&v,list_iterator-1) );

  /* close */
  pclose(fp);
}//end printMp3

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// this function responsible for turning led off before termination of program /////////////////// 

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);//reset handler
    digitalWrite(0,0);//make led off "GPIO 0" before closing program through ctr+c
    printf("exit \n");
    fflush(stdout) ;
    exit(0);
}//end sigintHandeler

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////// this thread responsible for checking existence of usb flash memory ////////////////////// 

void* checkThread(){
  while(1){

   if(usbExist()){
     digitalWrite(0,1);//make led "GPIO 0" on after pluging usb flash memory
    }else{
     digitalWrite(0,0);//make led "GPIO 0" ofter removing usb flash memory
     system("sudo pkill -f mpg321");//kill mp3 player "mpg321" after remove usb memory
    }
   usleep(10000);//delay 10 ms
  }//end while
}//end say

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// this thread responsible for playing mp3 files ///////////////////////////////// 

void* playMp3()
{

 while(1){usleep(10000);
  if((play_flag==1) && usbPath()!=NULL){
   scanMp3();
   play_flag=0;
   printf("total tracks : %d\n" ,vector_total(&v));
   int l = strlen((char *) vector_get(&v, list_iterator-1)) + strlen("mpg321 ") + 1;
   newr = malloc(l);
   strcpy(newr,"mpg321 ");
   strcat(newr,(char *) vector_get(&v ,list_iterator-1 ) );
  //int track=vector_total(&v)-list_iterator;
  //strcat(newr,"/media/MYLINUXLIVE/049.mp3");
   printf("li %d value t-li %s\n",list_iterator, (char*)vector_get(&v , list_iterator-1 ));
   system(newr);
  //printf("end play");
  }else{
   // flag=0;
  }
  usleep(10000);
 }//end while
}//end say1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// this thread responsible for checking states of buttons "play next track button" and "stop button" ///////

void * stopMp3(){
 while(1){
  if(usbPath()!=NULL &&digitalRead(2)==HIGH ){
   usleep(250000);
   system("sudo pkill -f mpg321");
   if(list_iterator<vector_total(&v)){
    list_iterator++;
    }else {
    list_iterator=1;
      }//end if
   play_flag=1;// set play flag to  start playing
   printf(" list i %d\n",list_iterator);
  }//end if
  if(digitalRead(3)==HIGH)
  {
  usleep(250000);
  system("sudo pkill -f mpg321"); //kill mpg321 if stop button is pressed
  }//end if
  if(usbPath()==NULL) play_flag=1;
  //printf("kill mpg321");
  usleep(10000);
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                           ////////////////////// Mani function ////////////////////// 
int main(void){

    wiringPiSetup();
    pinMode(0,OUTPUT);  //make "GPIO 0" output
    pinMode(2,INPUT);   //make "GPIO 2" input  play button
    pinMode(3,INPUT);   //make "GPIO 3" input  stop button

    signal(SIGINT, sigintHandler); //start signal handler
    // create threads //  
    pthread_create(&t1,NULL,checkThread,NULL);
    pthread_create(&t2,NULL,playMp3,NULL);
    pthread_create(&t3,NULL,stopMp3,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    printf("done\n");
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
