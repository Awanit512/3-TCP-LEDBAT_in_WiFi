/* 
 * QUICK INSTRUCTIONS:
 *
 * To optimize build:
 * ./waf -d optimized configure
 * ./waf
 *
 * To compile:
 * ./waf --run wifi-multirate
 *
 * To compile with command line(useful for varying parameters):
 * ./waf --run "<this file name> --simulationTime=0.3s --rateAdaptionAlgo=ns3::MinstrelWifiManager"
 *
 * To turn on NS_LOG:
 * export NS_LOG=multirate=level_all
 * (can only view log if built with ./waf -d debug configure)
 *
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
#include "ns3/athstats-helper.h"

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("Evaluation of TCP-LEDBAT For Star Topology");

//
//
//               n0          n2
//                 \        /
//                   \     /
//                     AP0 
//                    /    \
//                  /       \
//                n1         n3
//
//                 Star Topology
//

std::vector<uint64_t> lastTotalRx;          /* Vector of last total received bytes */
std::vector<uint64_t> flow;                 /*Vector of all flows to right Dumbell*/
double aggregate=0.0;                         /*Total aggreagted value of all flows at right side of dumbell*/
std::vector<Ptr<PacketSink> > sink;         /* Vector of Pointer to the packet sink application */
std::vector< std::ofstream* > outfile;       /*Vector of outfile for GNU plotting for corresponding flows*/
uint64_t  MAXOUTFILE = 8;                  
 /* Maximum Number of gnu plot will be 8 so if at right side of dumbell we have more 
 * than 8 STA then 8 sta's flow will be selected at random for generating plot
 * This is done in order to avoid flooding of gnu plot in base folder 
 * in case where there are many sta nodes at the right side of dumbell. 
*/

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
    NS_LOG_DEBUG ("arf");
    std::string phyRate = "HtMcs7"; 
    wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                        "DataMode", StringValue (phyRate),
                                        "ControlMode", StringValue ("HtMcs0"));
    return ;
    
    } 
}

void
ThroughputCalculation (double samplingPeriod, uint64_t minimaBetweenLeftAndRightSide)
{
  Time now = Simulator::Now ();  //Current( Virtual simulation ) Time  
  for (uint64_t nodeNumber=0; nodeNumber < minimaBetweenLeftAndRightSide; nodeNumber++)
  {  
    /* 
    * Convert Application RX Packets to MBits. and finally taking throughput sampling 
    * at each given sample period  
    */                   
    double currentFlow = ( (sink[nodeNumber]->GetTotalRx () - lastTotalRx[nodeNumber]) * (double) 8 /1e6 ) / (double) samplingPeriod;  
    aggregate += currentFlow ;
    if(nodeNumber < MAXOUTFILE + 1)
        {
            *(outfile[nodeNumber]) << now.GetSeconds () << " " << currentFlow  << std::endl;
        }
    
    
    flow[nodeNumber] += currentFlow ;
    std::cout << now.GetSeconds () << "s: \t" << currentFlow << " Mbit/s" <<" -- "<< sink[nodeNumber]->GetTotalRx () << "GetTotalRx"<<std::endl;
    lastTotalRx[nodeNumber] = sink[nodeNumber]->GetTotalRx ();
  }

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

int main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (std::string ("ns3::") + "TcpLedbat")));

  uint32_t payloadSize = 1024;                                              /* Transport layer payload size in bytes. */
  uint32_t maxBytes = 102400000;                                            /* maximum Bytes size in bytes for Bulk Transfer */
  std::string dataRate = "100Mbps";                                         /* Application layer datarate on central P2P Link. */
  std::string delay  = "2ms";                                               /* delay on central P2P Link. */
//   std::string phyRate = "HtMcs7";                                        /* Physical layer bitrate.
  std::string propagationDelayModel = "ConstantSpeedPropagationDelayModel"; /* PropagationDelayModel to be used in Phy layer*/
  std::string propagationLossModel = "FriisPropagationLossModel";           /* PropagationLossModel to be used in Phy layer*/
  double simulationTime = 10.0;                                             /* Simulation time in seconds by default = 10. */
  bool pcapTracing = false;                                                 /* PCAP Tracing is enabled or not. */
  std::string rateAdaptionAlgo = "aarf";                                    /* Rate adaption algorithm or wifi remote station manager to be used*/
  std::string mobilityModel = "RandomDirection2dMobilityModel";                  /*  Mobility Model to be used for STA's MobilityModel*/
  std::string errorRateModel = "YansErrorRateModel";                        /* Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel*/
  uint32_t leftSTAs = 2;                                                    /* Number of STAs on the left side*/
  uint32_t rightSTAs = 2;                                                   /* Number of STAs on the right side*/
  double samplingPeriod = 100.0;                                              /*Sampling Period for sampling throughput (in milisec) */
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
  cmd.AddValue ("rateAdaptionAlgo", "Rate Adaption Algorithm or Wifi remote station manager to be use. [ Please write RAA's acronym and in small letters ex : aarf, arf, onoe, minstreal, ideal or rbar etc..]", rateAdaptionAlgo);
  cmd.AddValue ("mobilityModel", "Mobility Model to be used for STAs", mobilityModel);
  cmd.AddValue ("errorRateModel", "Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel", errorRateModel);
  cmd.AddValue ("leftSTAs", "Number of STAs on the left side", leftSTAs);
  cmd.AddValue ("rightSTAs", "Number of STAs on the right side", rightSTAs);
  cmd.AddValue("samplingPeriod","Sampling Period for sampling throughput (in milisec)",samplingPeriod);
  cmd.AddValue ("wifiStandard", "Wifi STandard enter just extension [ ex : b,a,g,n,ac]" , wifiStandard);
  cmd.Parse (argc, argv);

  uint64_t minimaBetweenLeftAndRightSide = std::min(leftSTAs ,rightSTAs ); /*minima value between left and right side of no of sta in flexible dumbell*/ 
  for (uint64_t nodeNumber=0 ; nodeNumber < std::min(MAXOUTFILE, minimaBetweenLeftAndRightSide) ; nodeNumber++)
    { 
        lastTotalRx.push_back(0.0);
        flow.push_back(0.0);

        std::ofstream* outputGnuPlotPtr = new std::ofstream("Flow-"+ std::to_string(nodeNumber) + ".plt");
        std::ostringstream tempTss;
        //tempTss << "Flow-"+ std::to_string(nodeNumber) + ".plt";
        //outputGnuPlot.open (tempTss.str().c_str(), std::ofstream::out);
        *outputGnuPlotPtr<< "set terminal png" <<"\n";
        *outputGnuPlotPtr<< "set output \"" << "Flow-"+std::to_string(nodeNumber)+".png" <<"\"\n"; 
        *outputGnuPlotPtr<< "set title \"" << "Flow-"+std::to_string(nodeNumber) << "\"\n";
        *outputGnuPlotPtr<< "set xlabel \"X Values\"\n";
        *outputGnuPlotPtr<< "set ylabel \"Y Values\"\n\n";
        *outputGnuPlotPtr<< "set xrange [0:20]\n";
        *outputGnuPlotPtr<< "set yrange [0:10]\n";
        *outputGnuPlotPtr<<"plot \"-\"  title \"Throughput\" with linespoints\n";
        outfile.push_back(outputGnuPlotPtr);
    }
    
  /* Configure TCP Options */
//   Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));

  NodeContainer          m_leftSTANode;            // < Left side STA node container > 
  NetDeviceContainer     m_leftDevices;            // < Left side STA net device container > 
  NodeContainer          m_rightSTANode;           // < Right side STA node container > 
  NetDeviceContainer     m_rightDevices;           // < Right side STA net device container > 
  NodeContainer          m_accessPoints;                   // One Access Point
  NetDeviceContainer     m_accessPointsDevices;           // < AP NetDevices >
  Ipv4InterfaceContainer m_leftInterfaces;         // < Left side  STA's interfaces (IPv4) >
  Ipv4InterfaceContainer m_leftAPInterfaces;              // < Left AP interfaces (IPv4) >
  Ipv4InterfaceContainer m_rightInterfaces;        // < Right side  STA's interfaces (IPv4) >
  Ipv4InterfaceContainer m_rightAPInterfaces;             // < Right AP interfaces (IPv4) >
 // Ipv4InterfaceContainer m_CentralLinkInterfaces;         // < Central Links AP0---AP1 interfaces (IPv4)
  WifiHelper wifi;
  MobilityHelper mobility;
//   NodeContainer ap;
  WifiMacHelper wifiMac;
  NodeContainer wifiApNode;
  NetDeviceContainer wifiApDevice;

  // Create the required one access point i.e AP0 
  m_accessPoints.Create(1);  

  // Create the STA nodes
  m_leftSTANode.Create (leftSTAs);
  m_rightSTANode.Create (rightSTAs);
  wifiApNode = m_accessPoints.Get (0);

  /* Set up Legacy Channel*/
  YansWifiChannelHelper wifiChannel;
  if( propagationDelayModel == "ConstantSpeedPropagationDelayModel" )
        wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  else
        wifiChannel.SetPropagationDelay ("ns3::" + propagationDelayModel );

  if( propagationLossModel == "FriisPropagationLossModel" ) 
        wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel", "Frequency", DoubleValue (5e9));
  else
        wifiChannel.AddPropagationLoss ("ns3::" + propagationLossModel );


  /* Setup Physical Layer*/
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  if( errorRateModel == "YansErrorRateModel" )
        wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel");
  else
        wifiPhy.SetErrorRateModel ("ns3::" + errorRateModel);
//   //wifiPhy.SetErrorRateModel ("ns3::DsssErrorRateModel");


 Ssid ssid = Ssid ("WTLedbat-EE-FD0"); // wifi tcp ledbat exhaustive evaluation 
 ConfigureWifiStandard ( wifi , wifiStandard );
 ConfigureRemoteStationManager ( wifi, rateAdaptionAlgo );


  // setup stas for Left side
  wifiMac.SetType ("ns3::StaWifiMac",
                   "ActiveProbing", BooleanValue (true),
                   "Ssid", SsidValue (ssid));
  m_leftDevices = wifi.Install (wifiPhy, wifiMac, m_leftSTANode );

  // setup stas for Right side
  wifiMac.SetType ("ns3::StaWifiMac",
                   "ActiveProbing", BooleanValue (true),
                   "Ssid", SsidValue (ssid));
  m_rightDevices = wifi.Install (wifiPhy, wifiMac, m_rightSTANode );

  // setup for AP
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));
  wifiApDevice  = wifi.Install (wifiPhy, wifiMac, wifiApNode);

  // mobility configuration for left side STAs.
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
                                "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  else if( mobilityModel == "RandomDirection2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel");
  else
    mobility.SetMobilityModel ("ns3::" + mobilityModel );
  
  mobility.Install (m_leftSTANode);
 
  //AP Mobility
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);


  // mobility configuration for right side STAs.
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (100.0),
                                 "MinY", DoubleValue (100.0),
                                 "DeltaX", DoubleValue (15.0),
                                 "DeltaY", DoubleValue (15.0),
                                 "GridWidth", UintegerValue (1),
                                 "LayoutType", StringValue ("RowFirst"));
  
  //Mobility Model
  if (mobilityModel == "RandomWalk2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",  
                                "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  else if( mobilityModel == "RandomDirection2dMobilityModel" )
    mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  else
    mobility.SetMobilityModel ("ns3::" + mobilityModel );

  mobility.Install (m_rightSTANode);
  
  //AP1 Mobility
 // mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
 // mobility.Install (wifiApNode1);

//    for (uint32_t nodeNumber = 0; nodeNumber < m_leftDumbellSTANode.GetN (); ++nodeNumber )
//         Simulator::Schedule (Seconds (1.0), &AdvancePosition, m_leftDumbellSTANode.Get (nodeNumber));
//    for (uint32_t nodeNumber = 0; nodeNumber < m_leftDumbellSTANode.GetN (); ++nodeNumber )
//         Simulator::Schedule (Seconds (1.0), &AdvancePosition, m_rightDumbellSTANode.Get (nodeNumber) );

  InternetStackHelper stack;
  stack.Install (m_accessPoints);
  stack.Install (m_leftSTANode);
  stack.Install (m_rightSTANode);

  Ipv4AddressHelper Ip;
  // Assign IP address  
  Ip.SetBase("10.1.2.0","255.255.255.0");
  NetDeviceContainer tempNdc0;
  tempNdc0.Add (m_leftDevices);
  tempNdc0.Add (m_rightDevices);
  tempNdc0.Add(wifiApDevice);

  Ipv4InterfaceContainer iifc0 = Ip.Assign (tempNdc0);
   for (uint32_t nodeNumber = 0; nodeNumber < m_leftSTANode.GetN (); ++nodeNumber )
   {
      m_leftInterfaces.Add (iifc0.Get (nodeNumber));
   }
   for (uint32_t nodeNumber =  m_leftSTANode.GetN (); nodeNumber < m_leftSTANode.GetN () +m_rightSTANode.GetN (); ++nodeNumber )
   {
      m_rightInterfaces.Add (iifc0.Get (nodeNumber));
    }
  m_leftAPInterfaces.Add ( iifc0.Get (-1 + m_leftSTANode.GetN () + m_rightSTANode.GetN () ));
  m_rightAPInterfaces.Add ( iifc0.Get  ( m_leftSTANode.GetN () + m_rightSTANode.GetN () ));
  Ip.NewNetwork ();

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  //BulksendApplication on Left side STAS/nodes
  BulkSendHelper source ("ns3::TcpSocketFactory", Address ());
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));

  ApplicationContainer sourceAppsNodes;

  for (uint64_t nodeNumber = 0; nodeNumber  < leftSTAs; ++nodeNumber)
    {
      AddressValue sourceaddress (InetSocketAddress (m_rightInterfaces.GetAddress (nodeNumber), 8080));
      source.SetAttribute ("Remote", sourceaddress);
      sourceAppsNodes.Add (source.Install (m_leftSTANode.Get (nodeNumber)));
    }
  sourceAppsNodes.Start (Seconds (0.0));
  sourceAppsNodes.Stop (Seconds (simulationTime));



  //PacketsinkHelper on right side STA nodes
  PacketSinkHelper sinkhelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
  ApplicationContainer sinkAppsNodes;
  
  for (uint64_t nodeNumber = 0; nodeNumber < rightSTAs; ++nodeNumber)
    {
        sinkAppsNodes.Add (sinkhelper.Install (m_rightSTANode.Get (nodeNumber)));
    }
    
  for (uint64_t nodeNumber=0; nodeNumber < minimaBetweenLeftAndRightSide ;nodeNumber++)
    {
        sink.push_back( StaticCast<PacketSink> (sinkAppsNodes.Get (nodeNumber)) );
    }

  sinkAppsNodes.Start (Seconds (0.0));
  sinkAppsNodes.Stop (Seconds (simulationTime));
  
  Simulator::Schedule (Seconds (0), &ThroughputCalculation, samplingPeriod, minimaBetweenLeftAndRightSide);
  NS_LOG_INFO ("Run Simulation.");
  
  Simulator::Stop(Seconds(simulationTime));

  AnimationInterface anim ("star-topology-animation.xml");
    // position =  // configure the position example anim.SetConstantPosition(nodes.Get(0), 1.0, 2.0);
    // anim.SetConstantPosition(nodes.Get(0), position);
    // anim.SetConstantPosition(nodes.Get(1), position);
    Simulator::Run();
    Simulator::Destroy();

    // for (uint64_t nodeNumber=0; nodeNumber < std::min(MAXOUTFILE ,minimaBetweenLeftAndRightSide); nodeNumber++)
    // {
    //  *outfile[nodeNumber]<<"e\n";  
    //  //*outfile[nodeNumber].close();
    //  system(("gnuplot Flow-" + std::to_string(nodeNumber) + ".plt").c_str());
    // } 

    aggregate=aggregate/(double)200 ;
    std::cout<<"aggregate throughput:"<<aggregate<<std::endl;
    // f0=f0/(double)200;
    // f1=f1/(double)200;
    // f2=f2/(double)200;
    // std::cout<<"aggregate throughput:"<<aggregate<<"\nFlow0:"<<f0<<"\nFlow1:"<<f1<<"\nFlow2:"<<f2;
    return 0;
}
