#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "tictoc_m.h"

using namespace omnetpp;

class Txc : public cSimpleModule
{
  private:
    long numSent;  
    long numReceived;  
    double remainingEnergy;  
    double energyConsumptionRate;  

  protected:
    virtual TicTocMsg *generateMessage();
    virtual void forwardMessage(TicTocMsg *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void updateEnergy();  
    cOutVector energyVector;  
    cOutVector sentVector;  
    cOutVector receivedVector;
};

Define_Module(Txc);

void Txc::initialize()
{
    numSent = 0;
    numReceived = 0;
    remainingEnergy = 1000.0;  

    sentVector.setName("Packets Sent");
    receivedVector.setName("Packets Received");
    energyVector.setName("Energy");

    WATCH(numSent);
    WATCH(numReceived);
    WATCH(remainingEnergy);

    if (getIndex() == 0) {
        TicTocMsg *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc::handleMessage(cMessage *msg)
{
    TicTocMsg *ttmsg = check_and_cast<TicTocMsg *>(msg);

    if (ttmsg->getDestination() == getIndex()) {
        
        numReceived++;
        EV << "Message received. Total packets received: " << numReceived << endl;

        
        TicTocMsg *newmsg = generateMessage();
        forwardMessage(newmsg);
        numSent++;
    } else {
        forwardMessage(ttmsg);
    }
}

TicTocMsg *Txc::generateMessage()
{
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    TicTocMsg *msg = new TicTocMsg(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc::forwardMessage(TicTocMsg *msg)
{
    
    updateEnergy();

    if (remainingEnergy <= 0) {
        EV << "Node out of energy, message discarded.\n";
        delete msg;
        return;
    }

    int n = gateSize("gate");
    int k = intuniform(0, n-2);

    EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}

void Txc::updateEnergy()
{
    
    remainingEnergy -= energyConsumptionRate;
    
    energyConsumptionRate = uniform(0.5, 1.5);

    if (remainingEnergy < 0) {
        remainingEnergy = 0;
    }

    energyVector.record(remainingEnergy);
}

void Txc::finish()
{
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    EV << "Remaining Energy: " << remainingEnergy << endl;

    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);
    recordScalar("Remaining Energy", remainingEnergy);

    sentVector.record(numSent);
    receivedVector.record(numReceived);
    energyVector.record(remainingEnergy);
}
