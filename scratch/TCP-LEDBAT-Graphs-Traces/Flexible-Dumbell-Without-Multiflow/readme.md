# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (the throughput are taken with samplePeriod of 100 ms) [some files are bigger in size ex: pcap so they are not uploaded].

## Author : Awanit Ranjan (181CO161)

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


NOTE :


* The Corresponding code example `scratch/TCP-Ledbat-Evaluation/wifi-tcp-ledbat_flexible_dumbell` try to evaluates TCp Ledbat performance under wireless conditions . Here in this code we have make Two BSS attched via P2P links.
* All Nodes is enabled with TcpLedbat as its Socket Type in Layer 4 .
* we derive the graph after running the experiment 
* And graphs depict the flows (Throughput received by STA at each samplePerid (100 ms) ) of the STA at the right side of Dumbell i.e N_r_1 , ...N_r_n' 
* we have capped the plot generation for maximum 8 STA stations at the right side so that the generated plots does not  flood the base directoy ns3.3x/ in case when 
* number of STA at the right side of dumbell is high say 30-40. as a result if not capped then around 60-80 files of (.plt and .png) will be genearated 
* User can simply increse/decrease (modify) this capped value by changing the global variable value MAXOUTFILE to desired value.



* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error
* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:


*Troubleshooting:

    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v
   
*Running The Code / Experiment for say 28 seconds and for 5 STA at both sides of dumbell : 

    ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-tcp-ledbat_flexible_dumbell --simulationTime=28 --leftSTAs=8 --rightSTAs=8" 
   
   
*If user want to see the required command line arguments (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/wifi-tcp-ledbat_flexible_dumbell --help" 
    
    
 
