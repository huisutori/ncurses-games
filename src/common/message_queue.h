/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "queue.h"

typedef struct MessageQueue MessageQueue;

MessageQueue *MessageQueue_new(const Queue *queue);
void MessageQueue_delete(MessageQueue *message_queue);

const void *MessageQueue_wait(MessageQueue *message_queue);
const void *MessageQueue_wait_timeout(MessageQueue *message_queue, uint32_t timeout_ms);

int MessageQueue_push(MessageQueue *message_queue, const void *message);