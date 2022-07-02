#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    cOutVector sendVector;
public:
    Queue();
    virtual ~Queue();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    virtual void sendMsg();
    virtual void dropMsg(cMessage *msg);
    virtual void reciveMsg(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize() {
    buffer.setName("buffer");
    bufferSizeVector.setName("SizeVector");
    packetDropVector.setName("QueueDroppedpacket");
    sendVector.setName("sendVector");
    endServiceEvent = new cMessage("endService");
}

void Queue::finish() {
}

void Queue::sendMsg() {
    // dequeue packet
    cPacket *pkt = (cPacket*) buffer.pop();
    // send packet
    send(pkt, "out");
    sendVector.record(1);
    // start new service
    //serviceTime = par("serviceTime");
    serviceTime = pkt->getDuration();
    scheduleAt(simTime() + serviceTime, endServiceEvent);
}

void Queue::dropMsg(cMessage *msg) {
    delete msg;
    this->bubble("packet dropped");
    packetDropVector.record(1);
}

void Queue::reciveMsg(cMessage *msg) {
    // enqueue the packet
    buffer.insert(msg);
    bufferSizeVector.record(buffer.getLength());
    // if the server is idle
    if (!endServiceEvent->isScheduled()) {
        // start the service
        scheduleAt(simTime() + 0, endServiceEvent);
    }
}

void Queue::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            sendMsg();
        }
    } else if(buffer.getLength() >= par("bufferSize").longValue()){
        dropMsg(msg);
    } else { // if msg is a data packet
        reciveMsg(msg);
    }
}

#endif /* QUEUE */
