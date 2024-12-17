#ifndef BLINK_TASK_H
#define BLINK_TASK_H

void blinkTask(void *parameter);
extern void updateMaskBlinkPattern(uint8_t newMask, uint8_t ledIndex);

#endif
