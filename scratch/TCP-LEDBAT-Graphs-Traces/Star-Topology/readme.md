# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (thses throughput are taken with samplePeriod of 100 ms) [some files are bigger in size ex: pcap so they are not uploaded].

## Author : Yerramaddu Jahnavi (181CO260)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/wifi-tcp-ledbat-multiflow --simulationTime=28 --leftSTAs=2 --rightSTAs=2 --pcapTracing=true"
  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 28 seconds.
  
  > We have made pcap tracing to be true  ( --pcapTracing=true ).
 
  > There are 2 nodes at both right and left side (--leftSTAs=2 --rightSTAs=2).

  > TcpNewReno is used as Tcp Varient against Tcp Ledbat.

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
***
