#ifndef RECEIVER_TASK_H
#define RECEIVER_TASK_H

typedef struct {
    int channel[16];
    bool fail_safe;
    bool lost_frame;
    bool is_armed;
} TReceiverData;

extern TReceiverData receiverData;

void receiverTask(void *parameter);

#endif
