/*
 * VideoApp.h
 *
 *  Created on: 25/8/2015
 *      Author: Shivi, Yuchen
 */

#ifndef VIDEOAPP_H_
#define VIDEOAPP_H_

#include "UDPBasicApp.h"


class VideoApp : public UDPBasicApp
{
//Attributes
protected:
    int numSent; //number of packets sent by customer
    int numReceived; //number of packets received by other customer
    int delayCounter;//total of packets used to calculate how many delays - numReceived
    simtime_t sum_delay;//average delay
//Methods
public:
        VideoApp();
        ~VideoApp();
protected:

       virtual void initialize(int stage);
       virtual void sendPacket();
       virtual void processPacket(cPacket *msg);
       virtual void processSend();
       virtual void finish();


};




#endif /* VIDEOAPP_H_ */
