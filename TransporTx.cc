#ifndef TRANSPORTTX
#define TRANSPORTTX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class TransportTx : public cSimpleModule
{
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    cPacket *lastPkt;
    cOutVector sendVector;
    cOutVector bufferSize;
    short lastAck;
    bool established;

public:
    TransportTx();
    virtual ~TransportTx();
    virtual void sendPacket(cPacket *pkt);
    virtual void dropPacket(cMessage *msg);
    virtual void receivePacket(cMessage *msg);

protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx()
{
    endServiceEvent = NULL;
    lastPkt = NULL;
    established = false;
}

TransportTx::~TransportTx()
{
    cancelAndDelete(endServiceEvent);
}

void TransportTx::sendPacket(cPacket *pkt)
{
    lastPkt = pkt->dup();
    lastPkt->setTimestamp(simTime());
    lastAck = -1;
    send(pkt, "toOut$o");
    sendVector.record(1);
    serviceTime = pkt->getDuration();
    scheduleAt(simTime() + serviceTime, endServiceEvent);
}

void TransportTx::dropPacket(cMessage *msg)
{
    this->bubble("packet dropped");
    delete msg;
}

void TransportTx::receivePacket(cMessage *msg)
{
    buffer.insert(msg);
    bufferSize.record(buffer.getLength());
    if (!endServiceEvent->isScheduled())
    {
        scheduleAt(simTime(), endServiceEvent);
    }
}

void TransportTx::initialize()
{
    buffer.setName("buffer");
    sendVector.setName("sendTx");
    bufferSize.setName("bufferSizeTx");
    endServiceEvent = new cMessage("endService");

    cPacket *pkt = new cPacket("REQUEST", 2);
    sendPacket(pkt);
}

void TransportTx::finish()
{
}

void TransportTx::handleMessage(cMessage *msg)
{
    if (msg == endServiceEvent)
    {
        if (established && lastAck == 1)
        {
            if (!buffer.isEmpty())
            {
                cPacket *pkt = (cPacket *)buffer.pop();
                sendPacket(pkt);
            }
        }
        else if (lastAck == 0 || (simTime() - lastPkt->getTimestamp()) >= par("timeout"))
        {
            sendPacket(lastPkt);
        }
    }
    else if (msg->getKind())
    {
        if (!established && !strcmp(msg->getName(), "ACCEPT"))
        {
            lastAck = 1;
            established = true;
        }
        else
        {
            lastAck = !strcmp(msg->getName(), "ACK");
        }

        delete msg;
    }
    else if (established && buffer.getLength() >= par("bufferSize").intValue())
    {
        dropPacket(msg);
    }
    else
    {
        receivePacket(msg);
    }
}

#endif /* TRANSPORTTX */
