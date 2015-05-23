#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MSGSIZE 100
#define KEYFILE_PATH  "keyfilepath2"
#define ID 'N'
#define MSGQ_OK 0
#define MSGQ_NG -1
#define MSG_TYPE 1
#define CHANNEL1 1

typedef struct msgbuff{
    long mtype;
    char mtext[MSGSIZE];
}message;

int send_msg(int channel, char* text);
int receive_msg(int channel, message* msg);






    //-----------------------------------------------------------------------------------------------------------
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

  /* receive: nhan msg
     channel: kenh cua msg, co the coi nhu kenh tv,ben gui, ben nhan cung kenh thi thay msg
     *msg: con tro den noi dung muon nhan
  */

int receive_msg(int channel, message* msg)
{
	// -------------- Doan khoi tao -------------------

  int           msqid;
  key_t         keyx;
  struct msqid_ds msq;
  
  printf("processA(MessageReceive)  : start\n");

  keyx = ftok(KEYFILE_PATH, (int)ID);

  msqid = msgget(keyx, 0666 | IPC_CREAT);

  if (msqid == -1) {
    fprintf(stderr, "msgget failed with error: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  printf("processB(MessageSend)   : msqID = %d\n", msqid);

  //Receive message
  if((msgrcv(msqid, msg, sizeof(msg->mtext), channel, 0)) ==
     MSGQ_NG){
    perror("msgrcv:processA(MessageReceive)");
    exit(1);
  }
  //printf("processA(MessageReceive)  : received message = %s\n", message.mtext);

  //Control of message queue (message queue ID deletion)
  if(msgctl(msqid, IPC_RMID, NULL) == MSGQ_NG){
    perror("msgctl:processA(MessageReceive)");
  }

  printf("processA(MessageReceive)  : end\n");
  return 0;
}