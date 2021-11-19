# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (these throughput are taken with samplePeriod of 100 ms) [some files are bigger in size ex: pcap so they are not uploaded].

## Author : Awanit Ranjan (181CO161)


<h2><i> WIFI - LAN -WIFI Topology</i> </h2>

Note : The experiment is performed thrice and all this experments are simulated for same rate Adaption alogithm = "AARF".

First (First Scenario) one without flow from csma node and without presence of any otehr TCP Varient (other than TcpLedbat)

    ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35"
  
Second (Second Scenario) one without flow from csma node and with presence of any other TCP Varient (other than TcpLedbat)

    ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35 --allowFlowFromCsmaNodes=true --allowFlowFromCsmaNodes=false"
  
Third (Third Scenario) one with flow from csma node and with presence of any other TCP Varient (other than TcpLedbat)

    ./waf --run "scratc/wifi-lan-wifi-ledbat --simulationTime=35 --allowFlowFromCsmaNodes=true --allowFlowFromCsmaNodes=true"

  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 35 seconds.
 
  > There are 4 nodes at both right and left BSS which is default value.

  > TcpNewReno is used as Tcp Varient against Tcp Ledbat for teh second and third time. 

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position , Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
  > 
***

At the left side i.e STAs onnecting to AP (i.e Left BSS) among 4 STAs 2 are using TcpNewReno as its Socket type in Layer 2 as well as 2 STA  present at right side (among 4 STA) which is operating on  TcpNewReno as its Socket type in Layer 4 and rest are using TcpLedbat.

Note in GNU Plots for 4 flows cooresponding to 4 STA at right side BSS:

For the First Scenario : 

        # Flow 1, Flow 2 are Flow 3, Flow 4, i.e all Flows are of TcpLedbat flows
        

For the Second Scenario : 

        # Flow 1, Flow 2 are TcpNewReno flows
        
        # Flow 3, Flow 4,  are TcpLedbat flows
        

For the Third Scenario : 

        # Flow 1, Flow 2 are TcpNewReno flows
        
        # Flow 3, Flow 4,  are TcpLedbat flows
        
        # But This time we does allow some of the flow from csma Nodes in LAN.    
        
        
        
NOTE :


<b> WIFI - LAN -WIFI Topology </b>

* This Topology is inspired fro third.cc present in examples of ns3 .
* In this I have added a Wifi Hop at last to the last csma Node thus that node is acting as a AP too.
* In this example code I have configured the code to allow Competitive tcp Flows (provided allowCompetitiveTCPTraffic=true) 
* Apart from TCp Ledbat Flow (ex: TcpReno flow) The socket type of 50% of nodes(STA) on both sides will 
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
* I have capped the plot generation for maximum upto  8 STA stations at the right side so that the generated plots does not  flood the base directoy ns3.3x/ in case when 
* Number of STA at the right side of dumbell is high say 30-40. as a result if not capped then around 60-80 files of (.plt and .png) will be genearated 
* User can simply increse/decrease (modify) this capped value by changing the global variable value MAXOUTFILE to desired value.
                                         
* At left side we have r STAS and a AP (AP0) at right side we have r'' STAs and a AP (AP1) in midle we have a P2P link
* Connecting AP0 and one of nodes of LAN after that we have A LAN network (bus topology).


* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error


* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:

*Troubleshooting:

    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v
 
 
*Running The Code / Experiment for say 35 seconds and for 8 STA at both BSS i.e at left and right side : 

    ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-lan-wifi-ledbat --simulationTime=35 --nleftSTAs=8 --nrightSTAs=8" 
   
   
*If user want to see the required command line arguments (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-lan-wifi-ledbat --help" 
    
