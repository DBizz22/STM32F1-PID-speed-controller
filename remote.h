#include <stdint.h>
#include <stm32f10x.h>


extern const uint32_t NEC_reference[17];
extern const uint8_t remote_key[17];

typedef struct REMOTE
{
    uint8_t pin;
    uint8_t key;
} remote_controller;

void remote_init(const remote_controller* remote);
uint8_t NEC(const uint8_t pin);
void update_remote_key(remote_controller* remote,uint8_t (*protocol)(const uint8_t pin));