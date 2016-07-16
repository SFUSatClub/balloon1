#include <stdint.h>

typedef struct{
  uint16_t Interval;
  uint32_t LastTick;
  void (*Func)(void);
}TaskType;


TaskType *task_getConfig(void);
uint8_t task_getNumTasks(void);
