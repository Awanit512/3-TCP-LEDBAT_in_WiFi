/*
Author: Awanit Ranjan <awanitranjan.181me214@nitk.edu.in>

WIFI - LAN -WIFI Topology

* This Topology is inspired fro third.cc present in examples of ns3 .
* In this I have added a Wifi Hop at last to the last csma Node thus that node is acting as a AP too.
* Which we can see in below rough Topology Diagrame.
* In this example code I have configured the code to allow Competitive tcp Flows (provided allowCompetitiveTCPTraffic=true) 
* apart from TCp Ledbat Flow (ex: TcpReno flow) The socket type of 50% of nodes(STA) on both sides will 
* be having tcp varient other than TcpLedbat rest 50% STA of left and rigt BSS has TcpLedBat Socket. 

* As we also has few csma Nodes so This code does allow few flows 
* from these csma nodes (provided allowFlowFromCsmaNodes = true ) 
* ALso one worth point to share is that  We can notice that if STA do Active Probing then it takes 
* little time for a data transfer between source and sinks nodes 
* Lastly there are few other commnad Line Arguments such as simulationTime stating no: of seconds for which simulation is to be runned. 
* Thus this example code allows users to control by the parameter  ---> allowActiveProbing  
* Thus throug this example we can analyze the TcpLedbat Performance in presence of a competitive flow in wifi environmenst.
* We had derived the graph after running the experiment 
* And graphs depict the flows ( Throughput received by STA at each samplePerid (100 ms) some flows are for TcpLedbat and other are for Othr Tcp Varient used in default case it's TcpNewReno ) of the STA at the right side of Dumbell i.e N_r_1 , ...N_r_n' 
* We have capped the plot generation for maximum upto  8 STA stations at the right side so that the generated plots does not  flood the base directoy ns3.3x/ in case when 
* number of STA at the right side of dumbell is high say 30-40. as a result if not capped then around 60-80 files of (.plt and .png) will be genearated 
* User can simply increse/decrease (modify) this capped value by changing the global variable value MAXOUTFILE to desired value.


//   Wifi 10.1.1.0                                                           Wifi 10.1.4.0

//  ---- STA -----   AP                                               AP   ----- STA -----
//  *      *    *    *                                                *    *    *        *
//  |      |    |    |    10.1.2.0                                    |    |    |        |
// nr  ... n2   n1   AP0 -------------- n'1   n'2   n'3 ...  n'r'    AP1   N1   N2 ... N  r''
//                      point-to-point   |     |     |        |      |
//                                       =============================
//                                                 LAN 10.1.3.0
    At left side we have r STAS and a AP (AP0) at right side we have r'' STAs and a AP (AP1) in midle we have a P2P link
    Connecting AP0 and one of nodes of LAN after that we have A LAN network (bus topology).


 * QUICK INSTRUCTIONS:
 *
 * To optimize build:
 * ./waf -d optimized configure
 * ./waf
 *
 * To compile:
 * ./waf --run scratch/<file-name>
 *
 * To compile with command line(useful for varying parameters):
 * ./waf --run "<this file name> --simulationTime=0.3s --rateAdaptionAlgo=aarf"
 * 
 * To view pcap files:
 * tcpdump -nn -tt -r filename.pcap
 *
 * To monitor the files:
 * tail -f filename.pcap



Note : The experiment is performed thrice 

First one without flow from  csma node and without presence of any otehr TCP Varient (other than TcpLedbat)

  ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35"


Second one without flow from  csma node and with presence of any other TCP Varient (other than TcpLedbat)

  ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35 --allowFlowFromCsmaNodes=true --allowFlowFromCsmaNodes=false"


Third one with flow from  csma node and with presence of any otehr TCP Varient (other than TcpLedbat)

  ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35 --allowFlowFromCsmaNodes=true --allowFlowFromCsmaNodes=true"
*/


#include <cmath>
#include <iostream>
#include <sstream>
#include <bits/stdc++.h>
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/command-line.h"
#include "ns3/gnuplot.h"
#include "ns3/rectangle.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/athstats-helper.h"

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("Evaluation of TCP-LEDBAT For Wirless and Wired Environment.");



std::vector<uint64_t> lastTotalRx;              /* Vector of last total received bytes */
std::vector<uint64_t> flow;                     /*Vector of all flows to right BSS*/
double aggregate=0.0;                           /*Total aggreagted value of all flows at right side of dumbell*/
std::vector<Ptr<PacketSink> > sink;             /* Vector of Pointer to the packet sink application */
std::vector< std::ofstream* > outfile;          /*Vector of outfile for GNU plotting for corresponding flows*/
std::ofstream  allThroughputPlt;                /* A GNU PLOT plt  containing all throughput graphs */
std::ofstream  allThroughputData;               /* A GNU PLOT data containing all throughput.*/
uint64_t  MAXOUTFILE = 8;                  
 /* Maximum Number of gnu plot will be 8 so if at right side of dumbell we have more 
 * than 8 STA then 8 sta's flow will be selected at random for generating plot
 * This is done in order to avoid flooding of gnu plot in base folder 
 * in case where there are many sta nodes at the right side of dumbell. 
*/


void
ThroughputCalculation (double samplingPeriod, uint64_t minimaBetweenLeftAndRightSide)
{
   
  Time now = Simulator::Now ();  //Current( Virtual simulation ) Time  
  allThroughputData <<  now.GetSeconds ();
  for (uint64_t nodeNumber=0; nodeNumber < minimaBetweenLeftAndRightSide; nodeNumber++)
  {  
    /* 
    * Convert Application RX Packets to MBits. and finally taking throughput sampling 
    * at each given sample period  
    */      
    double newsamplingPeriod = samplingPeriod/1000;   // Converting sampling period from milisec to sec.         
    double currentFlow = ( (sink[nodeNumber]->GetTotalRx () - lastTotalRx[nodeNumber]) * (double) 8 /1e6 ) / (double) newsamplingPeriod;  
    aggregate += currentFlow ;
    if(nodeNumber < MAXOUTFILE + 1)
        {
            *(outfile[nodeNumber]) << now.GetSeconds () << " " << currentFlow  << std::endl;
            allThroughputData << " " << currentFlow;
        }
    
    
    flow[nodeNumber] += currentFlow ;
    std::cout <<"Time :"<< now.GetSeconds () << "s,\tcurrentFlow :" << currentFlow << " Mbit/s,\tGetTotalRx: " << sink[nodeNumber]->GetTotalRx ()  << ",\tlastTotalRx: " << lastTotalRx[nodeNumber] << std::endl;
    lastTotalRx[nodeNumber] = sink[nodeNumber]->GetTotalRx ();
  }
  allThroughputData<< "\n";
  Simulator::Schedule (MilliSeconds (samplingPeriod), &ThroughputCalculation, samplingPeriod, minimaBetweenLeftAndRightSide);

}

std::string 
GetOutputFileName ()
{
    std::string outPutFileName = "wifi-lan-ledbat-multiflow";
    return outPutFileName;
}

void 
ConfigureWifiStandard ( WifiHelper &wifiHelper , std::string wifiStandard )
{
    if( wifiStandard == "b")
    {NS_LOG_DEBUG ("WIFI-STANDARD 802.11b");
        wifiHelper.SetStandard (WIFI_STANDARD_80211b);
        return ;
    }
    else if ( wifiStandard == "g")
    {NS_LOG_DEBUG ("WIFI-STANDARD 802.11g");
        wifiHelper.SetStandard (WIFI_STANDARD_80211g);
        return ;
    }
    else if ( wifiStandard == "p")
    {NS_LOG_DEBUG ("WIFI-STANDARD 802.11p");
        wifiHelper.SetStandard (WIFI_STANDARD_80211p);
        return ;
    }
    else if ( wifiStandard == "ac")
    {NS_LOG_DEBUG ("WIFI-STANDARD 802.11ac");
        wifiHelper.SetStandard (WIFI_STANDARD_80211ac);
        return ;
    }
    else 
    {NS_LOG_DEBUG ("WIFI-STANDARD 802.11a");
        wifiHelper.SetStandard (WIFI_STANDARD_80211a);
        return ;
    }
}


void 
CountOfcsmaNodesChecker (uint64_t &ncsmaNodes)
{
  // As last CSMA Nodes is configured to be a AP so number of csma Nodes had to be atleast 1
  if (ncsmaNodes == 0)
  {

    std::cout<< "No of csma Nodes hs to atlest 1 the entered value" << ncsmaNodes << " is not valid so setting it to 1" << std::endl;
    ncsmaNodes = 1;
  }
}

void
CountOfWiifSTAChecker (uint64_t &nleftSTAs, bool isBelongToLeftBSS )
{
  // if STA belong to left BSS then isBelongToLeftBSS is set else unset.

  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.
  // Thus decided to put max 15 Nodes
  if (nleftSTAs > 15)
  {
    if (isBelongToLeftBSS)
    {
      // std::cout << "No of STA can at Left side BSS can not be greater than 15 so restricting it to 15" << std::endl;
      nleftSTAs = 15;
    }
    else
    {
      // std::cout << "No of STA can at Rightt side BSS can not be greater than 15 so restricting it to 15" << std::endl;
      nleftSTAs = 15;
    }
  }
}

void
ConfigureRemoteStationManager (WifiHelper &wifiHelper, std::string rateAdaptionAlgo)
{
  /*wifi remote station manager setup or Rate adaption algorithm setup*/
  if( rateAdaptionAlgo == "arf" )
    {NS_LOG_DEBUG ("arf");
    wifiHelper.SetRemoteStationManager ("ns3::ArfWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "aarf" )
    {NS_LOG_DEBUG ("aarf");
    wifiHelper.SetRemoteStationManager ("ns3::AarfWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "aarf-cd" )
    {NS_LOG_DEBUG ("aarf-cd");
    wifiHelper.SetRemoteStationManager ("ns3::AarfcdWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "cara" )
    {NS_LOG_DEBUG ("cara");
    wifiHelper.SetRemoteStationManager ("ns3::CaraWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "rraa" )
    {NS_LOG_DEBUG ("rraa");
    wifiHelper.SetRemoteStationManager ("ns3::RraaWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "ideal" || rateAdaptionAlgo == "rbar"  )
    {NS_LOG_DEBUG ("ideal/rbar");
    wifiHelper.SetRemoteStationManager ("ns3::IdealWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "onoe" )
    {NS_LOG_DEBUG ("onoe");
    wifiHelper.SetRemoteStationManager ("ns3::OnoeWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "minstrel" )
    {NS_LOG_DEBUG ("minstrel");
    wifiHelper.SetRemoteStationManager ("ns3::MinstrelWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "amrr" )
    {NS_LOG_DEBUG ("amrr");
    wifiHelper.SetRemoteStationManager ("ns3::AmrrWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "parf" )
    {NS_LOG_DEBUG ("parf");
    wifiHelper.SetRemoteStationManager ("ns3::ParfWifiManager");
    return ;
    }
  else if( rateAdaptionAlgo == "thompson" )
    {NS_LOG_DEBUG ("thompson");
    wifiHelper.SetRemoteStationManager ("ns3::ThompsonSamplingWifiManager");
    return ;
    }
 else
    {      
    NS_LOG_DEBUG ("constant-rate");
    //std::string phyRate = "HtMcs7"; 
    //wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
    //                                    "DataMode", StringValue (phyRate),
    //                                    "ControlMode", StringValue ("HtMcs0"));
    wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager");
    return ;
    
    } 
}



int 
main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (std::string ("ns3::") + "TcpLedbat")));

  std::string tcpCompetitiveVarient = "TcpNewReno";                /* Competitve Tcp example to be used against ledbat  for Throgput analysis. */                       
  uint32_t payloadSize = 1024;                                     /* Transport layer payload size in bytes. */
  uint32_t maxBytes = 102400000;                                   /* maximum Bytes size in bytes for Bulk Transfer */
  double simulationTime = 20.0;                                    /* Simulation time in seconds by default = 10. */
  bool pcapTracing = false;                                        /* PCAP Tracing is enabled or not. */
  bool enableFlowMon = true ;                                      /* Flow monitor  Tracing is enabled or not. */
  bool enableTracing = true;                                       /* ASCII  Tracing is enabled or not. */
  std::string rateAdaptionAlgo = "aarf" ;                          /* Rate adaption algorithm or wifi remote station manager to be used*/
  uint64_t nleftSTAs = 4 ;                                         /* Number of STAs in BSS0 */
  uint64_t nrightSTAs = 4;                                         /* Number of STAs in BSS1 */
  uint64_t ncsmaNodes = 5;                                         /* Number of csma Nodes in LAN */
  double samplingPeriod = 100.0;                                   /* Sampling Period for sampling throughput (in milisec) */
  std::string wifiStandard = "b";                                  /* Wifi STandard example 802.11b , 802.11a, 802.11n, 802.11ac, 802.11g */
  bool allowCompetitiveTCPTraffic = false;                         /* To analyze throghput of ledbat in competittive environment this flag should be set.*/ 
  bool allowFlowFromCsmaNodes = false;                             /*Allow flows from csma Nodes too to the sinks nodes (STAs in Right BSS).*/
  bool allowActiveProbing = false;                                 /* If true then STA do Active Probing else do Passive Probing in order to connect to bss*/

  CommandLine cmd (__FILE__);                                      /* Command line argument parser setup. */
  cmd.AddValue ("tcpCompetitiveVarient","Competitve Tcp example to be used against ledbat  for Throgput analysis. example: TcpWestwood, TcpLedbat ,TcpNewReno , TcpVegas, TcpVeno, TcpTahoe, ...",tcpCompetitiveVarient);
  cmd.AddValue ("payloadSize", "Transport Layer Payload size (in bytes)", payloadSize);
  cmd.AddValue ("maxBytes", "maximum Bytes size (in bytes) for Bulk Transfer", maxBytes);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("pcapTracing", "Enable/disable PCAP Tracing", pcapTracing);
  cmd.AddValue ("enableFlowMon", "Enable/disable FLOWMON Tracing", enableFlowMon);
  cmd.AddValue ("enableTracing", "Enable/disable ASCII Tracing", enableTracing);
  cmd.AddValue ("rateAdaptionAlgo", "Rate Adaption Algorithm or Wifi remote station manager to be use. [ Please write RAA's acronym and in small letters ex : aarf, arf, onoe, minstreal, ideal or rbar etc..]", rateAdaptionAlgo);
  cmd.AddValue ("leftSTAs", "Number of STAs in BSS0", nleftSTAs);
  cmd.AddValue ("rightSTAs", "Number of STAs in BSS1", nrightSTAs);
  cmd.AddValue ("csmaNodes", "Number of csma Nodes in LAN", ncsmaNodes);
  cmd.AddValue("samplingPeriod","Sampling Period for sampling throughput (in milisec)",samplingPeriod);
  cmd.AddValue ("wifiStandard", "Wifi STandard enter just extension [ ex : b,a,g,n,ac]" , wifiStandard);
  cmd.AddValue ("allowCompetitiveTCPTraffic"," To analyze throghput of ledbat in competittive environment this flag should be set.",allowCompetitiveTCPTraffic);
  cmd.AddValue ("allowFlowFromCsmaNodes","Allow flows from csma Nodes too to the sinks nodes (STAs in Right BSS).",allowFlowFromCsmaNodes);
  cmd.AddValue ("allowActiveProbing","If true then STA do Active Probing else do Passive Probing in order to connect to bss",allowActiveProbing);
  cmd.Parse (argc, argv);


  uint64_t minimaBetweenLeftAndRightSide = std::min(nleftSTAs ,nrightSTAs); 
  /*minima value between left and right side of no of sta in this topology.*/ 

  for (uint64_t nodeNumber=0 ; nodeNumber < std::min(MAXOUTFILE, minimaBetweenLeftAndRightSide) ; nodeNumber++)
    { 
        lastTotalRx.push_back(0.0);
        flow.push_back(0.0);

        std::ofstream* outputGnuPlotPtr = new std::ofstream("Flow-"+ std::to_string(nodeNumber+1) + ".plt");
        std::ostringstream tempTss;
        //tempTss << "Flow-"+ std::to_string(nodeNumber+1) + ".plt";
        //outputGnuPlot.open (tempTss.str().c_str(), std::ofstream::out);
        *outputGnuPlotPtr<< "set terminal png" <<"\n";
        *outputGnuPlotPtr<< "set output \"" << "Flow-"+std::to_string(nodeNumber+1)+".png" <<"\"\n"; 
        *outputGnuPlotPtr<< "set title \"" << "Flow-"+std::to_string(nodeNumber+1) << "\"\n";
        *outputGnuPlotPtr<< "set xlabel \"X Values\"\n";
        *outputGnuPlotPtr<< "set ylabel \"Y Values\"\n\n";
        *outputGnuPlotPtr<< "set xrange [0:" + std::to_string( (int) simulationTime+10.0 ) + "]\n";
        *outputGnuPlotPtr<< "set yrange [0:3]\n";
        *outputGnuPlotPtr<<"plot \"-\"  title \"Throughput\" with linespoints\n";
        outfile.push_back(outputGnuPlotPtr);
    }


  PointToPointHelper pointToPoint;
  NetDeviceContainer p2pDevices;  
  NodeContainer p2pNodes;

  NodeContainer csmaNodes;
  CsmaHelper csmaHelper;
  NetDeviceContainer csmaDevices;

  NodeContainer wifiStaNodesLeft;
  NetDeviceContainer wifiStaNodesLeftDevices;
  NodeContainer wifiApNodeLeft;
  YansWifiChannelHelper leftYansChannelHelper;
  YansWifiPhyHelper leftBSSphy;
  WifiHelper wifiLeft;
  Ssid ssidLeft;
  NetDeviceContainer leftApDevice;
  
  NodeContainer wifiStaNodesRight;
  NetDeviceContainer wifiStaNodesRightDevices;
  NodeContainer wifiApNodeRight;
  YansWifiChannelHelper rightYansChannelHelper;
  YansWifiPhyHelper rightBSSphy;
  WifiHelper wifiRight;
  Ssid ssidRight; 
  NetDeviceContainer rightApDevice;


  MobilityHelper mobility;
  WifiMacHelper mac;

  ApplicationContainer sourceAppsNodes;
  ApplicationContainer sinkAppsNodes;

  InternetStackHelper stack;
  Ipv4AddressHelper p2paddress;
  Ipv4AddressHelper csmaaddress;
  Ipv4AddressHelper wifiLeftaddress;
  Ipv4AddressHelper wifiRightaddress;
  Ipv4InterfaceContainer p2pInterfaces;
  Ipv4InterfaceContainer csmaInterfaces;
  Ipv4InterfaceContainer wifiInterfaceLeft;
  Ipv4InterfaceContainer wifiInterfaceRight;
  Ipv4InterfaceContainer rightAPInterface;
  Ipv4InterfaceContainer leftAPInterface;


  // checking count of csma Nodes and STA in BSS is right or not.
  CountOfcsmaNodesChecker(ncsmaNodes);
  CountOfWiifSTAChecker (nleftSTAs,true);
  CountOfWiifSTAChecker (nrightSTAs,false);

  // configuring p2p Nodes
  p2pNodes.Create (2);
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  p2pDevices = pointToPoint.Install (p2pNodes);  

 // configuring Csma Nodes
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create ( ncsmaNodes );
  csmaHelper.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csmaHelper.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  csmaDevices = csmaHelper.Install (csmaNodes);
  // std::cout<<"\n\n\nNo of csma devices : "<< csmaNodes.GetN() << "\n\n\n"<<std::endl;

  // configureing left BSS
  wifiStaNodesLeft.Create (nleftSTAs);
  wifiApNodeLeft = p2pNodes.Get (0);
  leftYansChannelHelper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  leftYansChannelHelper.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));
  leftBSSphy.SetErrorRateModel ("ns3::YansErrorRateModel");
  leftBSSphy.SetChannel (leftYansChannelHelper.Create ());

  ConfigureWifiStandard ( wifiLeft , wifiStandard );
  ConfigureRemoteStationManager ( wifiLeft, rateAdaptionAlgo);

  ssidLeft = Ssid ("ns3-Left-BSS-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidLeft),
               "ActiveProbing", BooleanValue (allowActiveProbing));
  wifiStaNodesLeftDevices = wifiLeft.Install (leftBSSphy, mac, wifiStaNodesLeft);
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidLeft));
  leftApDevice = wifiLeft.Install (leftBSSphy, mac, wifiApNodeLeft);

  // configureing right BSS
  wifiStaNodesRight.Create (nrightSTAs);
  wifiApNodeRight = csmaNodes.Get (csmaNodes.GetN() - 1);
  rightYansChannelHelper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  rightYansChannelHelper.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));
  rightBSSphy.SetErrorRateModel ("ns3::YansErrorRateModel");
  rightBSSphy.SetChannel (rightYansChannelHelper.Create ());

  ConfigureWifiStandard ( wifiRight , wifiStandard );
  ConfigureRemoteStationManager ( wifiRight, rateAdaptionAlgo);
  ssidRight = Ssid ("ns3-Right-BSS-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidRight),
               "ActiveProbing", BooleanValue (allowActiveProbing));
  wifiStaNodesRightDevices = wifiRight.Install (rightBSSphy, mac, wifiStaNodesRight);
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidRight));
  rightApDevice = wifiRight.Install (rightBSSphy, mac, wifiApNodeRight);


  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiStaNodesLeft);

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (30.0),
                                 "MinY", DoubleValue (30.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
                                 
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiStaNodesRight);

  Ptr<ListPositionAllocator> positionAllocForLeftAP = CreateObject<ListPositionAllocator> ();
  positionAllocForLeftAP->Add (Vector ( 7.0, 30.0, 0.0 ));
  mobility.SetPositionAllocator( positionAllocForLeftAP );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNodeLeft);

  Ptr<ListPositionAllocator> positionAllocForRightAP = CreateObject<ListPositionAllocator> ();
  for(uint64_t nodeNumber =0;nodeNumber < 1+ncsmaNodes;nodeNumber++)
    positionAllocForRightAP->Add (Vector ( 50.0+nodeNumber, 55.0-nodeNumber*2, 0.0 ));
  mobility.SetPositionAllocator( positionAllocForRightAP );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (csmaNodes);




  // Adding Internet Protocol Stacks
  stack.Install (csmaNodes);
  stack.Install (wifiApNodeLeft);
  stack.Install (wifiStaNodesLeft);
  stack.Install (wifiStaNodesRight);

//===================================
  /* 
   * Here we try to make some of the nodes in left BSS to have TcpSocket Type other than TcpLedbat 
   * in oder to analyze Tcpledbat in competiting flows in wifi environments. 
  */

  /*
   * Let the 50% of number of STAs at left side of The dumbell be having  TcpSocket Type other than TcpLedbat.
  */

 if(allowCompetitiveTCPTraffic)
 {
    uint64_t STAotherThanLedbatLeft = (uint64_t) ((nleftSTAs*50.0)/100.0); 
    // std::cout << "No of STAs other than TCP- Ledbat at left BSS : " << STAotherThanLedbatLeft << std::endl;
    for( uint64_t nodeNumber = 0 ;nodeNumber < STAotherThanLedbatLeft ; nodeNumber++ )
    {
        /*
        * TcpNewReno , TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno,
        * TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat 
        */
          TypeId tid = TypeId::LookupByName ("ns3::"+tcpCompetitiveVarient);
          std::stringstream nodeId;
          nodeId << wifiStaNodesLeft.Get (nodeNumber)->GetId ();
          std::string specificNode = "/NodeList/" + nodeId.str () + "/$ns3::TcpL4Protocol/SocketType";
          Config::Set (specificNode, TypeIdValue (tid));
    }


    /* 
    * Here we try to make some of the nodes in right BSS to have TcpSocket Type other than TcpLedbat 
    * in oder to analyze Tcpledbat in competiting flows in wifi environments. 
    */

    /*
    * Let the 50% (i.e half) of number of STAs at right side of The BSS be having  TcpSocket Type other than TcpLedbat.
    */
    uint64_t STAotherThanLedbatRight = (uint64_t) ((nrightSTAs*50.0)/100.0); 
    for( uint64_t nodeNumber = 0 ;nodeNumber < STAotherThanLedbatRight ; nodeNumber++ )
    {
        /*
        * TcpNewReno , TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno,
        * TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat 
        */
          TypeId tid = TypeId::LookupByName ("ns3::"+tcpCompetitiveVarient);
          std::stringstream nodeId;
          nodeId << wifiStaNodesRight.Get (nodeNumber)->GetId ();
          std::string specificNode = "/NodeList/" + nodeId.str () + "/$ns3::TcpL4Protocol/SocketType";
          Config::Set (specificNode, TypeIdValue (tid));
    }
 }

// =========================




 // Assigning Address to the nodes
  p2paddress.SetBase ("10.1.2.0", "255.255.255.0");
  p2pInterfaces = p2paddress.Assign (p2pDevices);

  csmaaddress.SetBase ("10.1.3.0", "255.255.255.0");
  csmaInterfaces = csmaaddress.Assign (csmaDevices);

  wifiLeftaddress.SetBase ("10.1.1.0", "255.255.255.0");
  NetDeviceContainer tempNdcForLeft;
  tempNdcForLeft.Add (leftApDevice); 
  tempNdcForLeft.Add (wifiStaNodesLeftDevices);
  Ipv4InterfaceContainer iifcLeft =  wifiLeftaddress.Assign (tempNdcForLeft);

  for (uint32_t nodeNumber = 0; nodeNumber < wifiStaNodesLeft.GetN (); ++nodeNumber )
   {
      wifiInterfaceLeft.Add (iifcLeft.Get (1+nodeNumber));
      // std::cout<<"Node Number :"<< nodeNumber + 1 <<" -> "<<iifc0.Get (1+nodeNumber)<<std::endl;
   }
  leftAPInterface.Add ( iifcLeft.Get (0));
  wifiLeftaddress.NewNetwork ();

  wifiRightaddress.SetBase ("10.1.4.0", "255.255.255.0");
  NetDeviceContainer tempNdcForRight;
  tempNdcForRight.Add (rightApDevice); 
  tempNdcForRight.Add (wifiStaNodesRightDevices);

  Ipv4InterfaceContainer iifcRight =  wifiRightaddress.Assign (tempNdcForRight);
  for (uint32_t nodeNumber = 0; nodeNumber < wifiStaNodesRight.GetN (); ++nodeNumber )
   {
      wifiInterfaceRight.Add (iifcRight.Get (1+nodeNumber));
      // std::cout<<"Node Number :"<< nodeNumber + 1 <<" -> "<<iifc0.Get (1+nodeNumber)<<std::endl;
    }
  rightAPInterface.Add ( iifcRight.Get (0));
   wifiRightaddress.NewNetwork ();

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  //BulksendApplication on Left Dumbell STAS/nodes
  BulkSendHelper source ("ns3::TcpSocketFactory", Address ());
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  uint64_t maxINdexofRightNodes = nrightSTAs; 
  for (uint64_t nodeNumber = 0; nodeNumber  < nleftSTAs; ++nodeNumber)
    {
      if (nodeNumber<maxINdexofRightNodes)
        {
          AddressValue sourceaddress (InetSocketAddress (wifiInterfaceRight.GetAddress (nodeNumber), 8080));
          source.SetAttribute ("Remote", sourceaddress);
        }
      else
        {
          AddressValue sourceaddress (InetSocketAddress (wifiInterfaceRight.GetAddress (maxINdexofRightNodes-1), 8080));
          source.SetAttribute ("Remote", sourceaddress);
        }

      sourceAppsNodes.Add (source.Install (wifiStaNodesLeft.Get (nodeNumber)));
      // if(nodeNumber>0)
      //   sourceAppsNodes.Add (source.Install (wifiStaNodesLeft.Get (nodeNumber-1)));

    }

//---------------

if(allowFlowFromCsmaNodes)
{
  for (uint64_t nodeNumber = 0; nodeNumber  < ncsmaNodes; ++nodeNumber)
    {
      if (nodeNumber<maxINdexofRightNodes)
        {
          AddressValue sourceaddress (InetSocketAddress (wifiInterfaceRight.GetAddress (nodeNumber), 8080));
          source.SetAttribute ("Remote", sourceaddress);
        }
      else
        {
          AddressValue sourceaddress (InetSocketAddress (wifiInterfaceRight.GetAddress (maxINdexofRightNodes-1), 8080));
          source.SetAttribute ("Remote", sourceaddress);
        }

      sourceAppsNodes.Add (source.Install (csmaNodes.Get (nodeNumber)));
      // if(nodeNumber>0)
      //   sourceAppsNodes.Add (source.Install (wifiStaNodesLeft.Get (nodeNumber-1)));

    }
}
//---------------   
  sourceAppsNodes.Start (Seconds (0.0));
  sourceAppsNodes.Stop (Seconds (simulationTime));

  //PacketsinkHelper on right Dumbell STA nodes
  PacketSinkHelper sinkhelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
  
  for (uint64_t nodeNumber = 0; nodeNumber < nrightSTAs; ++nodeNumber)
    {
        sinkAppsNodes.Add (sinkhelper.Install (wifiStaNodesRight.Get (nodeNumber)));
    }
  for (uint64_t nodeNumber=0; nodeNumber < minimaBetweenLeftAndRightSide ;nodeNumber++)
    {
        sink.push_back( StaticCast<PacketSink> (sinkAppsNodes.Get (nodeNumber)) );
    }
  // if ( ncsmaNodes >= 2 )
  //   {   
  //     for (uint64_t nodeNumber = (uint64_t) ncsmaNodes/2; nodeNumber  < ncsmaNodes; ++nodeNumber)
  //       {
  //        sinkAppsNodes.Add (sinkhelper.Install (csmaNodes.Get (nodeNumber)));
  //       }
  //   } 
   sinkAppsNodes.Start (Seconds (0.0));
   sinkAppsNodes.Stop (Seconds (simulationTime));


   std::ostringstream tempTssData;
   tempTssData << "Combine-Throughput-Flow-data.txt";
   allThroughputData.open (tempTssData.str().c_str(), std::ofstream::out);

   std::ostringstream tempTss;
   tempTss << "Combine-Throughput-Flow.plt";
   allThroughputPlt.open (tempTss.str().c_str(), std::ofstream::out);
   allThroughputPlt<< "set terminal png" <<"\n";
   allThroughputPlt<< "set output \"" << "Combine-Throughput-Flow.png" <<"\"\n"; 
   allThroughputPlt<< "set title \"" << "Throghput of all "+ std::to_string( std::min( MAXOUTFILE, minimaBetweenLeftAndRightSide ) )+ " Flows" << "\"\n";
   allThroughputPlt<< "set xlabel \"TimeStamps\"\n";
   allThroughputPlt<< "set ylabel \"Throughput\"\n\n";
   allThroughputPlt<< "set xrange [0:"+ std::to_string( (int) simulationTime+10.0 ) + "]\n";
   allThroughputPlt<< "set yrange [0:4]\n";
   allThroughputPlt<<"plot \"Combine-Throughput-Flow-data.txt\" using 1:2 title \"Flow 1\" with lines lw 2";
  for(uint64_t nodeNumber = 1 ; nodeNumber < std::min( MAXOUTFILE, minimaBetweenLeftAndRightSide ) ;nodeNumber++)
    {
        allThroughputPlt<<", \"Combine-Throughput-Flow-data.txt\" using 1:"+ std::to_string(nodeNumber+2) + " title \"Flow-" + std::to_string(1+nodeNumber)+"\" with lines lw 2";
    }
   allThroughputPlt<< "\n";



  Simulator::Schedule (Seconds (0), &ThroughputCalculation, samplingPeriod, minimaBetweenLeftAndRightSide);
  NS_LOG_INFO ("Run Simulation.");
  
  Simulator::Stop(Seconds(simulationTime));




  if (pcapTracing)
    {
      leftBSSphy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
      leftBSSphy.EnablePcap ("Wl-Lan-AccessPoint-Left", leftApDevice.Get (0)); //wiif Lan
      leftBSSphy.EnablePcap ("Wl-Lan-Station-Left", wifiStaNodesLeftDevices); //wiif Lan
      rightBSSphy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
      rightBSSphy.EnablePcap ("Wl-Lan-AccessPoint-Right", rightApDevice.Get (0)); //wiif Lan
      rightBSSphy.EnablePcap ("Wl-Lan-Station-Right",  wifiStaNodesRightDevices); //wiif Lan
    }
   

 if (enableTracing)
    {
      AsciiTraceHelper ascii;
      leftBSSphy.EnableAsciiAll (ascii.CreateFileStream ( (GetOutputFileName () + "_PhyLeft.tr").c_str()) );
      rightBSSphy.EnableAsciiAll (ascii.CreateFileStream ( (GetOutputFileName () + "_PhyRight.tr").c_str()) );
    }

  FlowMonitorHelper flowmonHelper;

  if (enableFlowMon)
    {
      flowmonHelper.InstallAll ();
    }
  
  AnimationInterface anim ("wifi-Lan-animation.xml");



    Simulator::Run();
    Simulator::Destroy();




    for (uint64_t nodeNumber=0; nodeNumber < std::min(MAXOUTFILE ,minimaBetweenLeftAndRightSide); nodeNumber++)
    {
     *outfile[nodeNumber]<<"e\n";  
     //*outfile[nodeNumber].close();
     system(("gnuplot Flow-" + std::to_string(nodeNumber+1) + ".plt").c_str());
    } 

    // allThroughput<<"e\n";  
    allThroughputData.close();
    allThroughputPlt.close();
    system( "gnuplot Combine-Throughput-Flow.plt" );

    aggregate=aggregate/(double)200 ;
    std::cout<<"aggregate throughput:"<<aggregate<<std::endl;
    // f0=f0/(double)200;
    // f1=f1/(double)200;
    // f2=f2/(double)200;
    // std::cout<<"aggregate throughput:"<<aggregate<<"\nFlow0:"<<f0<<"\nFlow1:"<<f1<<"\nFlow2:"<<f2;


  if (enableFlowMon)
    {
      flowmonHelper.SerializeToXmlFile ((GetOutputFileName () + ".flomon"), false, false);
      flowmonHelper.SerializeToXmlFile ((GetOutputFileName () + ".xml"), true, true);
    }

  return 0;
}
