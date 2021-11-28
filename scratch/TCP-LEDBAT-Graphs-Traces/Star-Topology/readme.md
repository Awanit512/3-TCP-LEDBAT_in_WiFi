# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (thses throughput are taken with samplePeriod of 100 ms).

## Author : Yerramaddu Jahnavi (181CO260)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/ledbat-star-topology --simulationTime=28 --pcapTracing=true"
  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 28 seconds.
  
  > We have made pcap tracing to be true  ( --pcapTracing=true ).
 
  > There are 2 nodes at both right and left side .

  > TcpNewReno is used as Tcp Varient against Tcp Ledbat.

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
***

* The Corresponding code example `scratch/TCP-Ledbat-Evaluation/ledbat-star-topology` try to evaluates TCP Ledbat performance under wireless conditions.

* In this example we will try to consider a star topology forming a BSS i.e at center we will be having AP and outside nodes as STA. 

* We will use a mobility model as required by simulator for calculating received signal. 

* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error


* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:

*Troubleshooting:

    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v
 
 
*Running The Code / Experiment for say 28 seconds: 

    ./waf --run "scratch/TCP-Ledbat-Evaluation/ledbat-star-topology  --simulationTime=28" 
   
   
*If user want to see the required command line arguments (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/ledbat-star-topology  --help" 
    
