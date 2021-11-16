# This Folder Contains all the relevent pcap, flowmon ,gnu plt trace files along with correspoding graphs of throghput flows against time (thses throughput are taken with samplePeriod of 100 ms).

## Author : Awanit Ranjan (181CO161)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/wifi-tcp-ledbat-multiflow --simulationTime=33 --leftSTAs=8 --rightSTAs=8 --pcapTracing=true"
  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 33 seconds.
  
  > We have made pcap tracing to be true  ( --pcapTracing=true ).
 
  > There are 8 nodes at both right and left side of flexible dumbell (--leftSTAs=8 --rightSTAs=8).

  > TcpNewReno is used as Tcp Varient against Tcp Ledbat.

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
***

At the left side i.e STAs onnecting to AP0 among 8 STAs 4 are using TcpNewReno as its Socket type in Layer 4 as well as 4 STA are also present at right side which is operating on  TcpNewReno as its Socket type in Layer 4 and rest are using TcpLedbat.


        # Flow 1, Flow 2, Flow 3 , Flow 4 are TcpNewReno flows</b>
        
        # Flow 5, Flow 6, Flow 7 , Flow 8 are TcpLedbat flows</b>
