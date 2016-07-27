package finaltask;

import inet.examples.adhoc.hostautoconf.Host;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import inet.nodes.inet.WirelessHost;
import inet.nodes.wireless.AccessPoint;
import inet.world.radio.ChannelControl;
import ned.DatarateChannel;
import inet.util.ThruputMeteringChannel;

import inet.applications.udpapp.UDPBasicApp;
simple VideoApp extends UDPBasicApp
{
    @class(VideoApp);
}

@license(LGPL);
//
// TODO documentation
//


network finaltask
{   
    parameters:
            int n;
    @display("bgb=830,284");
    types:
        channel nonspec extends DatarateChannel
        {
            parameters:
                delay = 0us;
                datarate = 100Gbps;
                ber = 0;
                per = 0;
        }
        channel fastethernet extends DatarateChannel
        {
            parameters:
                datarate = 100Mbps;
        }
        channel SDSL extends DatarateChannel
        {
            parameters:
                delay = 15ms;
                datarate = 16Mbps;
        }
        channel internet extends DatarateChannel
        {
            parameters:
                delay = 30ms;
        }
        
     
        
    submodules:
        Router: Router {
            @display("p=546,110");
            routingFile = "./gen_route_router.irt";
        }
        ISPRouter: Router {
            @display("p=445,110");
            routingFile = "./gen_route_isprouter.irt";
        }
        Router1: Router {
            @display("p=163,104");
            routingFile = "./gen_route_router1.irt";
        }
        adminPC: StandardHost {
            @display("p=612,26");
        }
        VideoCallPartner: StandardHost {
            @display("p=55,110");
        }
        CloudStorage: StandardHost {
            @display("p=163,26");
        }
        UploadWebServer: StandardHost {
            @display("p=163,189");
        }
        accessPoint: AccessPoint {
            @display("p=643,109");
        }
        wirelessHost[n]: WirelessHost {
            @display("p=759,78");
        }
        StudentLaptop: WirelessHost {
            @display("p=733,177");
        }
        StudentSmartphone: WirelessHost {
            @display("p=643,225");
        }
        channelControl: ChannelControl {
            numChannels = 1;
            @display("p=299,235");
        }
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=460,246");
        }
    connections:
        VideoCallPartner.pppg++ <--> nonspec <--> Router1.pppg++;
        CloudStorage.pppg++ <--> nonspec <--> Router1.pppg++;
        UploadWebServer.pppg++ <--> nonspec <--> Router1.pppg++;
        Router1.pppg++ <--> internet <--> ISPRouter.pppg++;
        ISPRouter.pppg++ <--> SDSL <--> Router.pppg++;
        Router.pppg++ <--> nonspec <--> adminPC.pppg++;
        Router.ethg++ <--> fastethernet <--> accessPoint.ethg++;
}

