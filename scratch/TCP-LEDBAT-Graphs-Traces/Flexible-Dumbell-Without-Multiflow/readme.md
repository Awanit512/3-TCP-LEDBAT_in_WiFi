# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (the throughput are taken with samplePeriod of 100 ms) [some files are bigger in size ex: pcap so they are not uploaded].

## Author : Awanit Ranjan (18CO161)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/wifi-tcp-ledbat --simulationTime=37 --leftSTAs=5 --rightSTAs=5"

  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 37 seconds.
  
  > We havem't enabled pcap tracing   ( as pcapTracing=false by default ).
 
  > There are 5 nodes at both right and left side of flexible dumbell (--leftSTAs=5 --rightSTAs=5).

  > TcpLedbat is used as Tcp Varient for all Nodes.

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
***
Note in GNU Plots for 5 flows cooresponding to 5 STA at right side of dumbell:


        # Flow 1, Flow 2, Flow 3 , Flow 4, Flow 5 are TcpLedbat flows
        
***
