/*
 * VideoApp.cc
 *
 *  Created on: 25/8/2015
 *      Author: Shivi, Yuchen
 */

#include "VideoApp.h"
#include "IPvXAddressResolver.h"

Define_Module(VideoApp);

VideoApp::VideoApp() { }

VideoApp::~VideoApp() { }

void VideoApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage==0)
    {
        numSent = 0;
        numReceived = 0;
        delayCounter = 0;
        sum_delay = 0;
        WATCH(numSent); //variable can be inspectable in Tkenv
        WATCH(numReceived);
        WATCH(delayCounter);
        WATCH(sum_delay);
        localPort = par("localPort");
        destPort = par("destPort");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");
        selfMsg = new cMessage("sendTimer");

    }

}

void VideoApp::sendPacket()
{
    char msgName[1388];
       sprintf(msgName, "VoIPApp-%d", numSent);
       cPacket *payload = new cPacket(msgName);
       payload->setByteLength(par("messageLength").longValue());

       //adding packet header size RTP has a minimum size of 12 Byte
       payload->addByteLength(12);
       //IPvXAddress destAddr = par("destAddresses") ;
       IPvXAddress destAddr = chooseDestAddr(); //Chooses random destination address
       EV<< "Destination address " << destAddr;
       socket.sendTo(payload,destAddr,destPort);
       numSent++;



}

void VideoApp::processPacket(cPacket *pk)
{
        emit(rcvdPkSignal, pk);
        //EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
        //delete pk;
        simtime_t delay = simTime()-pk->getCreationTime();
        delayCounter++;//problem
        sum_delay += delay;
        if (delay<=0.1) //Maximum delay 100ms
        {
            numReceived++;
        }
}


void VideoApp::processSend()
{
    sendPacket();
        simtime_t d = simTime() + par("sendInterval").doubleValue();
        if (stopTime < SIMTIME_ZERO || d < stopTime)
        {
            selfMsg->setKind(SEND);
            scheduleAt(d, selfMsg);
        }
        else
        {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
}

void VideoApp::finish()
{
    recordScalar("Packets received within the 100ms", numReceived);
    recordScalar("Packets Sent from the client", numSent);
    recordScalar("Lost Packets",((int)(delayCounter-numReceived)));
    recordScalar("Percentage of Loss",((double)(delayCounter-numReceived))/((double)delayCounter));
    recordScalar("Error Rate",(float)(numSent-numReceived)/numSent);
    recordScalar("Average Delay",sum_delay/numSent);
}




