#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class TransportRx : public cSimpleModule
{
private:
   cQueue buffer;
   cMessage *endServiceEvent;
   simtime_t serviceTime;
   cOutVector bufferSize;

public:
   TransportRx();
   virtual ~TransportRx();
   virtual void sendPacket(cPacket *pkt);
   virtual void sendFeedback(char *type);
   virtual void dropPacket(cMessage *msg);
   virtual void receivePacket(cMessage *msg);

protected:
   virtual void initialize();
   virtual void finish();
   virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx()
{
   endServiceEvent = NULL;
}

TransportRx::~TransportRx()
{
   cancelAndDelete(endServiceEvent);
}

void TransportRx::sendPacket(cPacket *pkt)
{
   send(pkt, "toApp");
   serviceTime = pkt->getDuration();
   scheduleAt(simTime() + serviceTime, endServiceEvent);
}

void TransportRx::sendFeedback(char *type)
{
   cPacket *pkt = new cPacket(type, 2);
   send(pkt, "toOut$o");
}

void TransportRx::dropPacket(cMessage *msg)
{
   this->bubble("packet dropped");
   delete msg;
}

void TransportRx::receivePacket(cMessage *msg)
{
   buffer.insert(msg);
   bufferSize.record(buffer.getLength());
   if (!endServiceEvent->isScheduled())
   {
       scheduleAt(simTime(), endServiceEvent);
   }
}

void TransportRx::initialize()
{
   buffer.setName("buffer");
   bufferSize.setName("bufferSizeRx");
   endServiceEvent = new cMessage("endService");
}

void TransportRx::finish()
{
}

void TransportRx::handleMessage(cMessage *msg)
{
   if (msg == endServiceEvent)
   {
       if (!buffer.isEmpty())
       {
           cPacket *pkt = (cPacket *)buffer.pop();
           sendPacket(pkt);
       }
   }
   else if (msg->getKind())
   {
      if (!strcmp(msg->getName(), "REQUEST"))
      {
         sendFeedback("ACCEPT");
      }
      delete msg;
   }
   else if (buffer.getLength() >= par("bufferSize").intValue())
   {
      sendFeedback("NAK");
      dropPacket(msg);
   }
   else
   {
      sendFeedback("ACK");
      receivePacket(msg);
   }
}

#endif /* TRANSPORTRX */
