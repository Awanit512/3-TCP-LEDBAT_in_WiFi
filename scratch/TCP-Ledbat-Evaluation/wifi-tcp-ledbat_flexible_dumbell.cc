/*
Author: Awanit Ranjan <awanitranjan.181me214@nitk.edu.in>

* This code example try to evaluates TCp Ledbat performance under wireless conditions . Here in this code we have make Two BSS attched via P2P links.
* All Nodes is enabled with TcpLedbat as its Socket Type in Layer 4 .
* we derive the graph after running the experiment 
* And graphs depict the flows (Throughput received by STA at each samplePerid (100 ms) ) of the STA at the right side of Dumbell i.e N_r_1 , ...N_r_n' 
* we have capped the plot generation for maximum 8 STA stations at the right side so that the generated plots does not  flood the base directoy ns3.3x/ in case when 
* number of STA at the right side of dumbell is high say 30-40. as a result if not capped then around 60-80 files of (.plt and .png) will be genearated 
* User can simply increse/decrease (modify) this capped value by changing the global variable value MAXOUTFILE to desired value.




* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error


* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:
  Troubleshooting:
    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v
   
  Running The Code / Experiment : 
   ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-tcp-ledbat_flexible_dumbell --simulationTime=28" 
   
   
 If user want to see the required command line argumenst (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-tcp-ledbat_flexible_dumbell --help" 
    
    
 



Flexible  Wireless Dumbell topology 

              
N_l_1- - - - - -                                   - - - - - - - -N_r_1
                 \                                /
* - - - -  - - -  \          10.1.2.0            /  - - - - - - - *
                 \ \   _____________________    / / 
* - - - - - -  - - AP0                       AP1  - - - - - - - - * 
                 / /                            \ \
* - - - -  - - -  /                              \  - - - - - - - * 
                 /                                \
N_l_n- - - - - -                                    - - - - - - - N_r_n'

    ---> BSS_0                                       ---> BSS_1
               10.1.1.0                                         10.1.3.0
        At left side we have n STAs.                      At right side we have n' STAs.


 * MORE QUICK INSTRUCTIONS:
 * To debug:
 * ./waf --shell
 * gdb ./build/debug/+ < path to this file from scratch folder>
 *
 * To view pcap files:
 * tcpdump -nn -tt -r filename.pcap
 *
 * To monitor the files:
 * tail -f filename.pcap
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


NS_LOG_COMPONENT_DEFINE ("Evaluation of TCP-LEDBAT For Flexible DUmbell Topology");



std::vector<uint64_t> lastTotalRx;              /* Vector of last total received bytes */
std::vector<uint64_t> flow;                     /*Vector of all flows to right Dumbell*/
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



// wifi-tcp


// #include "ns3/tcp-westwood.h"



//EXAMPES FROM THIRD.CC can also be used  and that from 
//LBE_Evualation.cc

// Fro wifi-adhoc we get info that how ot go for various rate adaption algorithm
//How to go for plotting GNU PLOT also setup of ADHOC/INFRASTRUCTURE less MDOE
//Setting of mobility model--> ConstantPositionMobilityModel 
//and position allocater --> ListPositionAllcoater



//  ************************  IMPORTANT  ************************


/*
 * This is a simple example to test TCP over 802.11n (with MPDU aggregation enabled).
 *
 * Network topology:
 *
 *   Ap    STA
 *   *      *
 *   |      |
 *   n1     n2
 *
 * In this example, an HT station sends TCP packets to the access point.
 * We report the total throughput received during a window of 100ms.
 * The user can specify the application data rate and choose the variant
 * of TCP i.e. congestion control algorithm to use.
 */

//////////////////////////   ****************        BUt this is lot similar to git repo of LBE_Evaluation.cc *******************




//Till Now we can see only one flow we want some competiting flows 

/////////////////////////////////////////////









//  ******************    TO BE DETERMINED ( TBD )    *****************

// Need to inspect about on - off and PacketSocket part present both in adhoc and ap 


std::string 
GetOutputFileName ()
{
    std::string outPutFileName = "wifi-ledbat-Flexible-dumbell";
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

static void
SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

static Vector
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

static void
AdvancePosition (Ptr<Node> node)
{
  Vector pos = GetPosition (node);
  pos.x += 3.0;
  if (pos.x >= 210.0)
    {
      return;
    }
  SetPosition (node, pos);

  Simulator::Schedule (Seconds (1.0), &AdvancePosition, node);
}

void
CourseChange (std::string context, Ptr<const MobilityModel> model)
  {
    Vector position = model->GetPosition ();
    NS_LOG_UNCOND (context << 
      " x = " << position.x << ", y = " << position.y);
  }

int main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (std::string ("ns3::") + "TcpLedbat")));

  uint32_t payloadSize = 1024;                                              /* Transport layer payload size in bytes. */
  uint32_t maxBytes = 102400000;                                            /* maximum Bytes size in bytes for Bulk Transfer */
  std::string dataRate = "100Mbps";                                         /* Application layer datarate on central P2P Link. */
  std::string delay  = "2ms";                                               /* delay on central P2P Link. */
//   std::string phyRate = "HtMcs7";                                        /* Physical layer bitrate. */
  std::string propagationDelayModel = "ConstantSpeedPropagationDelayModel"; /* PropagationDelayModel to be used in Phy layer*/
  std::string propagationLossModel = "FriisPropagationLossModel";           /* PropagationLossModel to be used in Phy layer*/
  double simulationTime = 10.0;                                             /* Simulation time in seconds by default = 10. */
  bool pcapTracing = false;                                                 /* PCAP Tracing is enabled or not. */
  bool enableFlowMon = true;                                                /* Flow monitor  Tracing is enabled or not. */
  bool enableTracing = true;                                                /* ASCII  Tracing is enabled or not. */
  std::string rateAdaptionAlgo = "aarf";                                    /* Rate adaption algorithm or wifi remote station manager to be used*/
  std::string mobilityModel = "RandomDirection2dMobilityModel";             /*  Mobility Model to be used for STA's MobilityModel*/
  std::string errorRateModel = "YansErrorRateModel";                        /* Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel*/
  uint32_t leftSTAs = 3;                                                    /* Number of STAs in BSS0 */
  uint32_t rightSTAs = 3;                                                   /* Number of STAs in BSS1 */
  double samplingPeriod = 100.0;                                            /*Sampling Period for sampling throughput (in milisec) */
  std::string wifiStandard = "b";                                           /* Wifi STandard example 802.11b , 802.11a, 802.11n, 802.11ac, 802.11g */
   

  /* Command line argument parser setup. */
  CommandLine cmd (__FILE__);
  cmd.AddValue ("payloadSize", "Transport Layer Payload size (in bytes)", payloadSize);
  cmd.AddValue ("maxBytes", "maximum Bytes size (in bytes) for Bulk Transfer", maxBytes);
  cmd.AddValue ("dataRate", "Application data Rate on Central P2P Link", dataRate);
  cmd.AddValue ("delay", "delay on central P2P Link", delay);
//cmd.AddValue ("phyRate", "Physical layer bitrate", phyRate);
  cmd.AddValue ("propagationDelayModel" , "PropagationDelayModel to be used in Phy layer" , propagationDelayModel);
  cmd.AddValue ("propagationLossModel" , "PropagationLossModel to be used in Phy layer example : FriisPropagationLossModel , LogDistancePropogationLossModel" , propagationLossModel);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("pcapTracing", "Enable/disable PCAP Tracing", pcapTracing);
  cmd.AddValue ("enableFlowMon", "Enable/disable FLOWMON Tracing", enableFlowMon);
  cmd.AddValue ("enableTracing", "Enable/disable ASCII Tracing", enableTracing);
  cmd.AddValue ("rateAdaptionAlgo", "Rate Adaption Algorithm or Wifi remote station manager to be use. [ Please write RAA's acronym and in small letters ex : aarf, arf, onoe, minstreal, ideal or rbar etc..]", rateAdaptionAlgo);
  cmd.AddValue ("mobilityModel", "Mobility Model to be used for STAs", mobilityModel);
  cmd.AddValue ("errorRateModel", "Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel", errorRateModel);
  cmd.AddValue ("leftSTAs", "Number of STAs in BSS0", leftSTAs);
  cmd.AddValue ("rightSTAs", "Number of STAs in BSS1", rightSTAs);
  cmd.AddValue("samplingPeriod","Sampling Period for sampling throughput (in milisec)",samplingPeriod);
  cmd.AddValue ("wifiStandard", "Wifi STandard enter just extension [ ex : b,a,g,n,ac]" , wifiStandard);
  cmd.Parse (argc, argv);

  uint64_t minimaBetweenLeftAndRightSide = std::min(leftSTAs ,rightSTAs ); /*minima value between left and right side of no of sta in flexible dumbell*/ 
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
        *outputGnuPlotPtr<< "set yrange [0:5]\n";
        *outputGnuPlotPtr<<"plot \"-\"  title \"Throughput\" with linespoints\n";
        outfile.push_back(outputGnuPlotPtr);
    }

  /* Configure TCP Options */
//   Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));

  NodeContainer          m_leftDumbellSTANode;            // < Left Dumbell STA node container > 
  NetDeviceContainer     m_leftDumbellDevices;            // < Left Dumbell STA net device container > 
  NodeContainer          m_rightDumbellSTANode;           // < Right Dumbell STA node container > 
  NetDeviceContainer     m_rightDumbellDevices;           // < Right Dumbell STA net device container > 
  NodeContainer          m_accessPoints;                    // < AP0 and AP1 using DS as p2p.
  NetDeviceContainer     m_accessPointsDevices;           // < AP NetDevices >
  Ipv4InterfaceContainer m_leftDumbellInterfaces;         // < Left Dumbell STA's interfaces (IPv4) >
  Ipv4InterfaceContainer m_leftAPInterfaces;              // < Left AP interfaces (IPv4) >
  Ipv4InterfaceContainer m_rightDumbellInterfaces;        // < Right Dumbell STA's interfaces (IPv4) >
  Ipv4InterfaceContainer m_rightAPInterfaces;             // < Right AP interfaces (IPv4) >
  Ipv4InterfaceContainer m_CentralLinkInterfaces;         // < Central Links AP0---AP1 interfaces (IPv4)
  WifiHelper wifi;
  MobilityHelper mobility;
  WifiMacHelper wifiMac;
  NodeContainer wifiApNode0;
  NetDeviceContainer wifiApDevice0;
  NodeContainer wifiApNode1;
  NetDeviceContainer wifiApDevice1;

  // Create the required two access points i.e AP0 and AP1 
  m_accessPoints.Create(2);  

  // Create the STA nodes
  m_leftDumbellSTANode.Create (leftSTAs);
  m_rightDumbellSTANode.Create (rightSTAs);

  // Create the distribution system between two access points by help of P2P Helper and configure it.
  PointToPointHelper distributionSystemHelper;

  distributionSystemHelper.SetDeviceAttribute ("DataRate", StringValue (dataRate));
  distributionSystemHelper.SetChannelAttribute ("Delay", StringValue (delay));

  // Add the link connecting accespoints AP0  and AP1
  m_accessPointsDevices  = distributionSystemHelper.Install (m_accessPoints);
  wifiApNode0 = m_accessPoints.Get (0);
  wifiApNode1 = m_accessPoints.Get (1);



  /* Set up Legacy Channel , Keeping it same for both side dumbell*/
  YansWifiChannelHelper wifiChannel0;
  if( propagationDelayModel == "ConstantSpeedPropagationDelayModel" )
        wifiChannel0.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  else
        wifiChannel0.SetPropagationDelay ("ns3::" + propagationDelayModel );

  if( propagationLossModel == "FriisPropagationLossModel" ) 
        wifiChannel0.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));
  else
        wifiChannel0.AddPropagationLoss ("ns3::" + propagationLossModel );


  /* Setup Physical Layer , Keeping it same for both side dumbell */
  YansWifiPhyHelper wifiPhy0 = YansWifiPhyHelper::Default ();
  wifiPhy0.SetChannel (wifiChannel0.Create ());
  if( errorRateModel == "YansErrorRateModel" )
        wifiPhy0.SetErrorRateModel ("ns3::YansErrorRateModel");
  else
        wifiPhy0.SetErrorRateModel ("ns3::" + errorRateModel);
// wifiPhy.SetErrorRateModel ("ns3::DsssErrorRateModel");


 Ssid ssid0 = Ssid ("WTLedbat-EE-FD0"); // wifi tcp ledbat exhaustive evaluation on flexible dumbell BSS0


  /* Set up Legacy Channel , Keeping it same for both side dumbell*/
  YansWifiChannelHelper wifiChannel1;
  if( propagationDelayModel == "ConstantSpeedPropagationDelayModel" )
        wifiChannel1.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  else
        wifiChannel1.SetPropagationDelay ("ns3::" + propagationDelayModel );

  if( propagationLossModel == "FriisPropagationLossModel" ) 
        wifiChannel1.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));
  else
        wifiChannel1.AddPropagationLoss ("ns3::" + propagationLossModel );


  /* Setup Physical Layer , Keeping it same for both side dumbell */
  YansWifiPhyHelper wifiPhy1 = YansWifiPhyHelper::Default ();
  wifiPhy1.SetChannel (wifiChannel1.Create ());
  if( errorRateModel == "YansErrorRateModel" )
        wifiPhy1.SetErrorRateModel ("ns3::YansErrorRateModel");
  else
        wifiPhy1.SetErrorRateModel ("ns3::" + errorRateModel);
// wifiPhy.SetErrorRateModel ("ns3::DsssErrorRateModel");


 Ssid ssid1 = Ssid ("WTLedbat-EE-FD1"); // wifi tcp ledbat exhaustive evaluation on flexible dumbell BSS1

 ConfigureWifiStandard ( wifi , wifiStandard );
 ConfigureRemoteStationManager ( wifi, rateAdaptionAlgo );


  // setup stas for Left Dumbell .
  wifiMac.SetType ("ns3::StaWifiMac",
                   "ActiveProbing", BooleanValue (true),
                   "Ssid", SsidValue (ssid0));


  m_leftDumbellDevices = wifi.Install (wifiPhy0, wifiMac, m_leftDumbellSTANode );

  // setup stas for Right Dumbell .
  wifiMac.SetType ("ns3::StaWifiMac",
                   "ActiveProbing", BooleanValue (true),
                   "Ssid", SsidValue (ssid1));
  m_rightDumbellDevices = wifi.Install (wifiPhy1, wifiMac, m_rightDumbellSTANode );


  // setup Left ap --> AP0.
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid0));
  wifiApDevice0  = wifi.Install (wifiPhy0, wifiMac, wifiApNode0);

  // setup Left ap --> AP1.
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid1));
  wifiApDevice1 = wifi.Install (wifiPhy1, wifiMac, wifiApNode1);
  
  
  // mobility configuration for left dumbell STAs.
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  
  //Mobility Model
  if (mobilityModel == "RandomWalk2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",  
                                "Bounds", RectangleValue (Rectangle (-40, 40, -40, 40)));
  else if( mobilityModel == "RandomDirection2dMobilityModel" )
  {
    mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                    "Bounds", RectangleValue (Rectangle (-70, 50, -70, 50)));
  }
  else
    mobility.SetMobilityModel ("ns3::" + mobilityModel );
  
  mobility.Install (m_leftDumbellSTANode);
 
  //AP0 Mobility
  Ptr<ListPositionAllocator> positionAllocForAP0 = CreateObject<ListPositionAllocator> ();
  positionAllocForAP0->Add (Vector ( 10.0, 40.0, 0.0 ));
  mobility.SetPositionAllocator( positionAllocForAP0 );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode0);


  // mobility configuration for right dumbell STAs.
//   mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
//                                  "MinX", DoubleValue (100.0),
//                                  "MinY", DoubleValue (100.0),
//                                  "DeltaX", DoubleValue (15.0),
//                                  "DeltaY", DoubleValue (15.0),
//                                  "GridWidth", UintegerValue (1),
//                                  "LayoutType", StringValue ("RowFirst"));

  // mobility configuration for right dumbell STAs.
  Ptr<ListPositionAllocator> positionAllocRightDumbell = CreateObject<ListPositionAllocator> ();
  for (uint32_t nodeNumber = 0; nodeNumber < m_rightDumbellSTANode.GetN (); ++nodeNumber )
   {      
        Ptr<UniformRandomVariable> randomNumber = CreateObject<UniformRandomVariable> ();
        randomNumber->SetAttribute ("Min", DoubleValue (10.0));
        randomNumber->SetAttribute ("Max", DoubleValue (25.0));
        positionAllocRightDumbell->Add (Vector (55.0 - 5.0*nodeNumber + (double) randomNumber->GetInteger (), 55.0 - 5.0*nodeNumber + (double) randomNumber->GetInteger (), 0.0));
   }
  mobility.SetPositionAllocator (positionAllocRightDumbell);
  
  //Setting Mobility Model for right Dumbell STAs
  if (mobilityModel == "RandomWalk2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",  
                                "Bounds", RectangleValue (Rectangle (-70, 50, -70, 50)));
  else if( mobilityModel == "RandomDirection2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                "Bounds", RectangleValue (Rectangle (-70, 50, -70, 50)));
  else
    mobility.SetMobilityModel ("ns3::" + mobilityModel );

  mobility.Install (m_rightDumbellSTANode);
  

  //AP1 Mobility
  Ptr<ListPositionAllocator> positionAllocForAP1 = CreateObject<ListPositionAllocator> ();
  positionAllocForAP1->Add (Vector ( 5.0, 70.0, 0.0 ));
  mobility.SetPositionAllocator( positionAllocForAP1 );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode1);

//    for (uint32_t nodeNumber = 0; nodeNumber < m_leftDumbellSTANode.GetN (); ++nodeNumber )
//         Simulator::Schedule (Seconds (1.0), &AdvancePosition, m_leftDumbellSTANode.Get (nodeNumber));
//    for (uint32_t nodeNumber = 0; nodeNumber < m_leftDumbellSTANode.GetN (); ++nodeNumber )
//         Simulator::Schedule (Seconds (1.0), &AdvancePosition, m_rightDumbellSTANode.Get (nodeNumber) );

  InternetStackHelper stack;
  stack.Install (m_accessPoints);
  // stack.Install (wifiApNode0);
  // stack.Install (wifiApNode1);
  Ipv4StaticRoutingHelper staticRoutingHelper;
  stack.SetRoutingHelper (staticRoutingHelper);
  stack.Install (m_leftDumbellSTANode);
  stack.Install (m_rightDumbellSTANode);



  //Assigns IPAddress to P2P Central DS Backbone
  Ipv4AddressHelper centralLinkIP;
  Ipv4AddressHelper rightIp;
  Ipv4AddressHelper leftIp;

  centralLinkIP.SetBase ("10.1.1.0", "255.255.255.0");
  m_CentralLinkInterfaces = centralLinkIP.Assign (m_accessPointsDevices);
 
  // Assign IP address to left side 
  leftIp.SetBase("10.1.2.0","255.255.255.0");
  NetDeviceContainer tempNdc0;
  tempNdc0.Add(wifiApDevice0);
  //tempNdc0.Add(wifiApDevice1);
  tempNdc0.Add (m_leftDumbellDevices);
  //tempNdc0.Add (m_rightDumbellDevices);
  Ipv4InterfaceContainer iifc0 = leftIp.Assign (tempNdc0);

  for (uint32_t nodeNumber = 0; nodeNumber < m_leftDumbellSTANode.GetN (); ++nodeNumber )
   {
      m_leftDumbellInterfaces.Add (iifc0.Get (1+nodeNumber));
      // std::cout<<"Node Number :"<< nodeNumber + 1 <<" -> "<<iifc0.Get (1+nodeNumber)<<std::endl;
    }
//   Ipv4InterfaceContainer iifc0 = leftIp.Assign (tempNdc0);
  //  for (uint32_t nodeNumber =  m_leftDumbellSTANode.GetN (); nodeNumber < m_leftDumbellSTANode.GetN () +m_rightDumbellSTANode.GetN (); ++nodeNumber )
  //  {
  //     m_rightDumbellInterfaces.Add (iifc0.Get (nodeNumber));
  //   }
  // m_leftAPInterfaces.Add ( iifc0.Get (-1 + m_leftDumbellSTANode.GetN () + m_rightDumbellSTANode.GetN () ));

  NodeContainer::Iterator iter;
  for ( iter = m_leftDumbellSTANode.Begin (); iter != m_leftDumbellSTANode.End (); iter++)
    {
      Ptr<Ipv4StaticRouting> staticRouting;
      staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> ((*iter)->GetObject<Ipv4> ()->GetRoutingProtocol ());
      staticRouting->SetDefaultRoute ("10.1.2.1", 1 );
    }

  m_leftAPInterfaces.Add ( iifc0.Get (0));
  // std::cout<<"Node Number :"<< 0 <<" -> "<<iifc0.Get (0)<<std::endl;

  leftIp.NewNetwork ();


 //==================================================================================================


  // Assign IP address to right side 
  rightIp.SetBase("10.3.1.0","255.255.255.0");
  NetDeviceContainer tempNdc1;
  tempNdc1.Add(wifiApDevice1);
  tempNdc1.Add (m_rightDumbellDevices);
  Ipv4InterfaceContainer iifc1 = rightIp.Assign (tempNdc1);

   for (uint32_t nodeNumber = 0; nodeNumber < m_rightDumbellSTANode.GetN (); ++nodeNumber )
   {
      m_rightDumbellInterfaces.Add ( iifc1.Get ( 1+nodeNumber) );
      // std::cout<<"Node Number :"<< nodeNumber + 1 <<" -> "<<iifc1.Get (1+nodeNumber)<<std::endl;

    }

  for ( iter = m_rightDumbellSTANode.Begin (); iter != m_rightDumbellSTANode.End (); iter++)
    {
      Ptr<Ipv4StaticRouting> staticRouting;
      staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> ((*iter)->GetObject<Ipv4> ()->GetRoutingProtocol ());
      staticRouting->SetDefaultRoute ("10.3.1.1", 1 );
    }
  m_rightAPInterfaces.Add ( iifc1.Get (0));
  // std::cout<<"Node Number :"<< 0 <<" -> "<<iifc1.Get (0)<<std::endl;

  rightIp.NewNetwork ();

//   // Assign IP address to right side 
//   rightIp.SetBase("10.1.3.0","255.255.255.0");
//   NetDeviceContainer tempNdc1;
//   tempNdc1.Add (m_rightDumbellDevices);
//   tempNdc1.Add(wifiApDevice1);
//   Ipv4InterfaceContainer iifc1 = rightIp.Assign (tempNdc1);
//    for (uint32_t nodeNumber = 0; nodeNumber < m_rightDumbellSTANode.GetN (); ++nodeNumber )
//    {
//       m_rightDumbellInterfaces.Add (iifc1.Get (nodeNumber));
//     }
//   m_rightAPInterfaces.Add (iifc1.Get (m_rightDumbellSTANode.GetN ()));
//   rightIp.NewNetwork ();

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  //BulksendApplication on Left Dumbell STAS/nodes
  BulkSendHelper source ("ns3::TcpSocketFactory", Address ());
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));

  ApplicationContainer sourceAppsNodes;

  for (uint64_t nodeNumber = 0; nodeNumber  < leftSTAs; ++nodeNumber)
    {
      AddressValue sourceaddress (InetSocketAddress (m_rightDumbellInterfaces.GetAddress (nodeNumber), 8080));
      source.SetAttribute ("Remote", sourceaddress);
      sourceAppsNodes.Add (source.Install (m_leftDumbellSTANode.Get (nodeNumber)));
    }
  sourceAppsNodes.Start (Seconds (0.0));
  sourceAppsNodes.Stop (Seconds (simulationTime));



  //PacketsinkHelper on right Dumbell STA nodes
  PacketSinkHelper sinkhelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
  ApplicationContainer sinkAppsNodes;
  
  for (uint64_t nodeNumber = 0; nodeNumber < rightSTAs; ++nodeNumber)
    {
        sinkAppsNodes.Add (sinkhelper.Install (m_rightDumbellSTANode.Get (nodeNumber)));
    }
    
  for (uint64_t nodeNumber=0; nodeNumber < minimaBetweenLeftAndRightSide ;nodeNumber++)
    {
        sink.push_back( StaticCast<PacketSink> (sinkAppsNodes.Get (nodeNumber)) );
    }

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
   allThroughputPlt<< "set xrange [0:"+  std::to_string( (int) simulationTime+10.0 ) + "]\n";
   allThroughputPlt<< "set yrange [0:3]\n";
   allThroughputPlt<<"plot \"Combine-Throughput-Flow-data.txt\" using 1:2 title \"Flow 1\" with lines lw 2";
  for(uint64_t nodeNumber = 1 ; nodeNumber < std::min( MAXOUTFILE, minimaBetweenLeftAndRightSide ) ;nodeNumber++)
    {
        allThroughputPlt<<", \"Combine-Throughput-Flow-data.txt\" using 1:"+ std::to_string(nodeNumber+2) + " title \"Flow-" + std::to_string(1+nodeNumber)+"\" with lines lw 2";
    }
   allThroughputPlt<< "\n";

  
  Simulator::Schedule (Seconds (0), &ThroughputCalculation, samplingPeriod, minimaBetweenLeftAndRightSide);
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop(Seconds(simulationTime));


  /* Enable Traces */
  if (pcapTracing)
    {
      wifiPhy0.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
      wifiPhy0.EnablePcap ("Wl-Fd-AccessPoint", m_accessPointsDevices.Get (0)); //wiif flexible dumbell
      wifiPhy0.EnablePcap ("Wl-Fd-Station", m_leftDumbellDevices); //wiif flexible dumbell
      wifiPhy1.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
      wifiPhy1.EnablePcap ("Wl-Fd-AccessPoint", m_accessPointsDevices.Get (1)); //wiif flexible dumbell
      wifiPhy1.EnablePcap ("Wl-Fd-Station", m_rightDumbellDevices); //wiif flexible dumbell
    }

 if (enableTracing)
    {
      AsciiTraceHelper ascii;
      wifiPhy0.EnableAsciiAll (ascii.CreateFileStream ( (GetOutputFileName () + "_Phy0.tr").c_str()) );
      wifiPhy1.EnableAsciiAll (ascii.CreateFileStream ( (GetOutputFileName () + "_Phy1.tr").c_str()) );
    }

  FlowMonitorHelper flowmonHelper;

  if (enableFlowMon)
    {
      flowmonHelper.InstallAll ();
    }


  AnimationInterface anim ("flexible-dumbell-animation.xml");
    // position =  // configure the position example anim.SetConstantPosition(nodes.Get(0), 1.0, 2.0);
    // anim.SetConstantPosition(nodes.Get(0), position);
    // anim.SetConstantPosition(nodes.Get(1), position)




// To track the position of STA 
    // std::ostringstream oss;
    // oss << "/NodeList/" << m_rightDumbellSTANode.Get (2)->GetId () << "/$ns3::MobilityModel/CourseChange";

    // Config::Connect (oss.str (), MakeCallback (&CourseChange));

      

    // std::cout << "The below stats is for AP1 " <<std::endl;
    // std::cout << "Ipv4 Interface Address P2P " << (m_accessPoints.Get(1)->GetObject<Ipv4> ())->GetAddress (1,0) << std::endl;
    // std::cout << "Ipv4 Interface Address WIFI " << (m_accessPoints.Get(1)->GetObject<Ipv4> ())->GetAddress (2,0) << std::endl;
    // std::cout << "Ipv4 Local Address P2P" << ((m_accessPoints.Get(1)->GetObject<Ipv4> ())->GetAddress (1,0)).GetLocal() << std::endl;
    // std::cout << "Ipv4 Local Address WIFI " << ((m_accessPoints.Get(1)->GetObject<Ipv4> ())->GetAddress (2,0)).GetLocal() << std::endl;



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

