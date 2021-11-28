# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (the throughput are taken with samplePeriod of 100 ms) [some files are bigger in size ex: pcap so they are not uploaded].

## Author : Yerramaddu Jahnavi (181CO260)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/ledbat-traingular-topology.cc --simulationTime=28"

  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 28 seconds.
  
  > There are 3 nodes on each BSS.

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
  ***
  NOTE :
  
  * This example consist of traigular(3 BSS ) and each having three nodes(STA) though the implementor will extend this example to x, y, z (3, 3, 3 for all three BSS by default) for BSS0, BSS1 and BSS2 respectively).
  *  A similar idea as that of flexible dumbell topology. 
  
  
* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error
* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:
  
Troubleshooting:

    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v
   
*Running The Code / Experiment for say 28 seconds: 

    ./waf --run "scratch/TCP-Ledbat-Evaluation/ledbat-triangular-topology --simulationTime=28" 
   
   
*If user want to see the required command line arguments (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/ledbat-triangular-topology --help" 
    
