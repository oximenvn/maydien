#include "contact.h"

int main(){
    message msg;
    
    receive_msg(CHANNEL1, &msg);
    printf("ok\n");
    send_msg(CHANNEL1,"ok \n");
    send_msg(CHANNEL1,"update \n");
    send_msg(CHANNEL1,"changing ...");
    send_msg(CHANNEL1,"writelog....")
}