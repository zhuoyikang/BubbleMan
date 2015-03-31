#include "ServerHandler.hpp"
#include "BubbleApp.hpp"

void ServerHandler::Loop()
{
    gBubbleApp.Mq.Lock();
    while (!gBubbleApp.Mq.Empty()) {
        QueueMsg *msg = gBubbleApp.Mq.Front();
        gBubbleApp.Mq.Pop();

        ServerHandlerFptr h = FindHandler(msg->T());
        if (h!=NULL) {
            CallHandler(h, msg);
        }
    }
    gBubbleApp.Mq.Unlock();
}
