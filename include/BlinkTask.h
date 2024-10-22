#ifndef BLINK_TASK_H
#define BLINK_TASK_H

#define LED1 2              // blau (RUNNING, Armed, Disarmed)
#define LED2 15             // rot (Fehler)

class BlinkController {
    public:
        BlinkController() {};

        void blink(uint8_t pattern);
    private:

};

void blinkTask(void *parameter);

#endif
