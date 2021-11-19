#include <cmath>
#include <iostream>
#include <sstream>
#include<bits/stdc++.h>
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/core-module.h"
#include "ns3/command-line.h"
#include "ns3/gnuplot.h"


using namespace ns3;



/*

Simple Wireless Dumbell topology 
N_l_1- - - - - -                                   - - - - - - - -N_r_1
                 \                                /
* - - - -  - - -  \          P2P                 /  - - - - - - - *
                 \ \   _____________________    / / 
* - - - - - -  - - AP                       R1  - - - - - - - - * 
                 / /                            \ \
* - - - -  - - -  /                              \  - - - - - - - * 
                 /                                \
N_l_n- - - - - -                                    - - - - - - - N_r_n'

*/

NS_LOG_COMPONENT_DEFINE ("Evaluation of TCP-LEDBAT For Wireless dumbell topology");


uint32_t maxBytes = 102400000;
uint64_t lastTotalRx0 = 0,lastTotalRx1 = 0,lastTotalRx2 = 0;
double aggregate=0,f0=0,f1=0,f2=0;
Ptr<PacketSink> sink0;
Ptr<PacketSink> sink1;
Ptr<PacketSink> sink2;
std::vector< std::ofstream* > gnuPlot;



void GeneratePLTForFlow(uint64_t flowNum, double simulationTime)
{
  std::string fileName = ("Flow"+std::to_string(flowNum) +".plt");
  std::ofstream* gnuPLotPtr = new std::ofstream("Flow-"+ std::to_string(flowNum+1) + ".plt");
  std::ostringstream tss;
  tss << fileName;
  *gnuPLotPtr<< "set terminal png" <<"\n";
  *gnuPLotPtr<< "set output \"" << "Flow"+std::to_string(flowNum+1) +".png" <<"\"\n"; 
  *gnuPLotPtr<< "set title \"" << "Flow-"+std::to_string(flowNum+1) << "\"\n";
  *gnuPLotPtr<< "set xlabel \"X Values\"\n";
  *gnuPLotPtr<< "set ylabel \"Y Values\"\n\n";
  *gnuPLotPtr<< "set xrange [0:"+ std::to_string( (uint64_t) (simulationTime+2.0))+"]\n";
  *gnuPLotPtr<< "set yrange [0:5]\n";
  *gnuPLotPtr<<"plot \"-\"  title \"Throughput\" with linespoints\n";
   gnuPlot.push_back(gnuPLotPtr);

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

    wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager");
    return ;
    
    } 
}


void
CalculateThroughput ()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  double cur0 = (sink0->GetTotalRx () - lastTotalRx0) * (double) 8 /1e5;
  double cur1 = (sink1->GetTotalRx () - lastTotalRx1) * (double) 8/1e5;
  double cur2 = (sink2->GetTotalRx () - lastTotalRx2) * (double) 8/1e5;

  *gnuPlot[0] << now.GetSeconds () << " " << cur0 << std::endl;
  *gnuPlot[1]  << now.GetSeconds () << " " << cur1 << std::endl;
  *gnuPlot[2]  << now.GetSeconds () << " " << cur2 << std::endl; 
  aggregate+=cur0+cur1+cur2; 
//   f0+=cur0; f1+=cur1; f2+=cur2;   
 std::cout << now.GetSeconds () << "s: \t" << cur0+cur1+cur2 << " Mbit/s" <<std::endl;
  lastTotalRx0 = sink0->GetTotalRx ();
  lastTotalRx1 = sink1->GetTotalRx ();  
  lastTotalRx2 = sink2->GetTotalRx ();
  Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
}

int 
main (int argc, char *argv[])
{ 
  double simulationTime = 20;
  uint32_t payloadSize = 1472;                       /* Transport layer payload size in bytes. */
  std::string delay  = "2ms";                                               /* delay on central P2P Link. */
  std::string rateAdaptionAlgo = "aarf";                                    /* Rate adaption algorithm or wifi remote station manager to be used*/
//   std::string mobilityModel = "RandomDirection2dMobilityModel";                  /*  Mobility Model to be used for STA's MobilityModel*/
//   std::string errorRateModel = "YansErrorRateModel";                        /* Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel*/
  uint32_t nleft=3;
  uint32_t nright=3;
  std::string wifiStandard = "g";                                           /* Wifi STandard example 802.11b , 802.11a, 802.11n, 802.11ac, 802.11g */

  NodeContainer          m_leftDumbellSTANode;            // < Left Dumbell STA node container > 
  NetDeviceContainer     m_leftDumbellDevices;            // < Left Dumbell STA net device container > 
  NodeContainer          m_rightDumbellSTANode;           // < Right Dumbell STA node container > 
  NetDeviceContainer     m_rightDumbellDevices;           // < Right Dumbell STA net device container >
  NodeContainer          centrallink;             //!< Router and AP
  NetDeviceContainer     m_centralDevices;       //!< Routers NetDevices
  NetDeviceContainer     m_rightRouterDevices;
  Ipv4InterfaceContainer m_leftLeafInterfaces;    //!< Left Leaf interfaces (IPv4)
  Ipv4InterfaceContainer m_leftRouterInterfaces;  //!< Left router interfaces (IPv4)
  Ipv4InterfaceContainer m_rightLeafInterfaces;   //!< Right Leaf interfaces (IPv4)
  Ipv4InterfaceContainer m_rightRouterInterfaces; //!< Right router interfaces (IPv4)
  Ipv4InterfaceContainer m_routerInterfaces;      //!< Router interfaces (IPv4)
  WifiHelper wifi;
  MobilityHelper mobility;
  NodeContainer ap;
  WifiMacHelper wifiMac;
  NodeContainer wifiApNode;
  NetDeviceContainer apDevice;

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (std::string ("ns3::") + "TcpLedbat")));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));


  
  centrallink.Create(2);

  CommandLine cmd;
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("Size", "Payload size in bytes", payloadSize);
  cmd.AddValue ("rateAdaptionAlgo", "Rate Adaption Algorithm or Wifi remote station manager to be use. [ Please write RAA's acronym and in small letters ex : aarf, arf, onoe, minstreal, ideal or rbar etc..]", rateAdaptionAlgo);
//   cmd.AddValue ("mobilityModel", "Mobility Model to be used for STAs", mobilityModel);
//   cmd.AddValue ("errorRateModel", "Error Rate Model To be used like YansErrorRateModel NISTErrorRateModel", errorRateModel);
  cmd.AddValue ("wifiStandard", "Wifi STandard enter just extension [ ex : b,a,g,n,ac]" , wifiStandard);
  cmd.AddValue ("delay", "delay on central P2P Link", delay);
  cmd.Parse (argc, argv);


  m_leftDumbellSTANode.Create (nleft);
  m_rightDumbellSTANode.Create (nright);

  PointToPointHelper rightHelper,centrallinkHelper,distributionSystemHelper;

  centrallinkHelper.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  centrallinkHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  distributionSystemHelper.SetChannelAttribute ("Delay", StringValue (delay));
  // Add the link connecting routers
  m_centralDevices = centrallinkHelper.Install (centrallink);
  wifiApNode = centrallink.Get (0);

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();

  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  
  Ssid ssid = Ssid ("WTLedbat-EE-FD0");


  ConfigureWifiStandard ( wifi , wifiStandard );
  ConfigureRemoteStationManager ( wifi, rateAdaptionAlgo );
  
  // setup stas.
  wifiMac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid));

  m_leftDumbellDevices = wifi.Install (wifiPhy, wifiMac, m_leftDumbellSTANode);
  
  // setup ap.
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));
  apDevice = wifi.Install (wifiPhy, wifiMac, wifiApNode);
  
  // mobility.
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  
  //Left leaf nodes Mobility
  mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel");
  mobility.Install (m_leftDumbellSTANode);
  
  //AP Mobility
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  
  // Add the right side links
  rightHelper.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  rightHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  for (uint32_t i = 0; i < nright; ++i)
    {
      NetDeviceContainer c = rightHelper.Install (centrallink.Get (1),
                                                  m_rightDumbellSTANode.Get (i));
      m_rightRouterDevices.Add (c.Get (0));
      m_rightDumbellDevices.Add (c.Get (1));
    }  

  InternetStackHelper stack;
  stack.Install (centrallink);
  stack.Install (m_leftDumbellSTANode);
  stack.Install (m_rightDumbellSTANode);


 
  //Assigns IPAddress
  Ipv4AddressHelper routerIp;
  Ipv4AddressHelper rightIp;
  Ipv4AddressHelper leftIp;
  routerIp.SetBase ("10.1.1.0", "255.255.255.0");
  m_routerInterfaces = routerIp.Assign (m_centralDevices);
 
  // Assign to left side 
  leftIp.SetBase("10.1.2.0","255.255.255.0");
  NetDeviceContainer ndc;
  ndc.Add (m_leftDumbellDevices);
  ndc.Add(apDevice);
  Ipv4InterfaceContainer ifc = leftIp.Assign (ndc);
   for (uint32_t i = 0; i < m_leftDumbellSTANode.GetN (); ++i)
   {
      m_leftLeafInterfaces.Add (ifc.Get (i));
    }
  m_leftRouterInterfaces.Add (ifc.Get (m_leftDumbellSTANode.GetN ()));
  leftIp.NewNetwork ();

  // Assign to right side 
  rightIp.SetBase("10.1.3.0","255.255.255.0");
  for (uint32_t i = 0; i < m_rightDumbellSTANode.GetN (); ++i)
    { 
      NetDeviceContainer ndc;
      ndc.Add (m_rightDumbellDevices.Get (i));
      ndc.Add (m_rightRouterDevices.Get (i));
      Ipv4InterfaceContainer ifc = rightIp.Assign (ndc);
      m_rightLeafInterfaces.Add (ifc.Get (0));
      m_rightRouterInterfaces.Add (ifc.Get (1));
      rightIp.NewNetwork ();
    }
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  //BulksendApplication on Left nodes
  BulkSendHelper source ("ns3::TcpSocketFactory", Address ());
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));

  ApplicationContainer sourceApps;

  for (uint32_t i = 0; i < nleft; ++i)
    {
      AddressValue sourceaddress (InetSocketAddress (m_rightLeafInterfaces.GetAddress (i), 8080));
      source.SetAttribute ("Remote", sourceaddress);
      sourceApps.Add (source.Install (m_leftDumbellSTANode.Get (i)));
    }
  sourceApps.Start (Seconds (0.0));
  sourceApps.Stop (Seconds (20.0));
  //PacketsinkHelper on right nodes
  PacketSinkHelper sinkhelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
  ApplicationContainer sinkApps;
  
  
  for (uint32_t i = 0; i < nright; ++i)
    {
      sinkApps.Add (sinkhelper.Install (m_rightDumbellSTANode.Get (i)));
    }
  
  sink0 = StaticCast<PacketSink> (sinkApps.Get (0));
  sink1 = StaticCast<PacketSink> (sinkApps.Get (1));
  sink2 = StaticCast<PacketSink> (sinkApps.Get (2));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (20.0));

  
 for(uint64_t i=0;i<nright;i++)
   GeneratePLTForFlow(i,simulationTime);

  Simulator::Schedule (Seconds (0), &CalculateThroughput);
  NS_LOG_INFO ("Run Simulation.");
  
  Simulator::Stop(Seconds(simulationTime));
  Simulator::Run ();
  Simulator::Destroy ();

   for (uint64_t nodeNumber=0; nodeNumber < nright; nodeNumber++)
    {
     *gnuPlot[nodeNumber]<<"e\n";  
     system(("gnuplot Flow-" + std::to_string(nodeNumber+1) + ".plt").c_str());
    } 

  std::cout<<"Aggregate throughput: for All " << nright << " Flows : "<< aggregate/(double)200 <<std::endl;
  return 0;
}
 
