#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSIZE 100
#define KEYFILE_PATH  "keyfilepath"
#define ID 'M'
#define MSGQ_OK 0
#define MSGQ_NG -1
#define MSG_TYPE 1

int main(void)
{
  send_msg(1,"hello from send");
  /* code */
  return 0;
}

  /* send_msg: gui msg
     channel: kenh cua msg, co the coi nhu kenh tv,ben gui, ben nhan cung kenh thi thay msg
     text: noi dung muon gui
  */

int send_msg(int channel, char* text)
{
  // -------------- Doan khoi tao -------------------
  struct msgbuff{
    long mtype;
    char mtext[MSGSIZE];
  }message;

  int           msqid;
  key_t         keyx;
  struct msqid_ds msq;
  
  printf("processB(MessageSend)  : start\n");

  keyx = ftok(KEYFILE_PATH, (int)ID);

  msqid = msgget(keyx, 0666 | IPC_CREAT);

  if (msqid == -1) {
    fprintf(stderr, "msgget failed with error: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  printf("processB(MessageSend)   : msqID = %d\n", msqid);

  // -------------- Doan tao message, muon gui cai gi thi ghi vao mtext, mtype la loai -------------
  message.mtype = channel;

  strcpy(message.mtext,text);
  printf("processB(MessageSend)   : send message = %s\n", message.mtext);

  // ------------- Gui message ----------------------------
  // Send message
  if((msgsnd(msqid, (void*)&message, strlen(message.mtext) + 1, 0)) ==
     MSGQ_NG){
    perror("msgsnd:processB(MessageSend) ");
    exit(1);
  }

  printf("processB(MessageSend)   : end\n");
  return 0;
}