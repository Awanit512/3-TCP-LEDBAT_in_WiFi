# This folder contains all the relevent gnu plt trace files along with correspoding graphs of throghput flows against time (the throughput are taken with samplePeriod of 100 ms) 

## Author : Keerti Chaudhary (181CO226)

The arguments feeded to the exampe while running it are following: 

    ./waf --run "scratch/ledbat-simple-dumbell-topology --simulationTime=20" 
  
  ***
  
Information Related to Experiments :

  > Experiment is ran for simulationTime = 20 seconds.
 
  > There are 8 nodes at both right and left side of simple dumbell (--leftSTAs=8 --rightSTAs=8).

  > 802.11b is used as the WIFI STANDARD for the experiment ( which is the default ).

  > Rate Adaption Algorithm used is  AARF ( which is the default ).

  > "RandomDirection2dMobilityModel" is used for STA's Mobility while AP are at fixed position, Also "YansErrorRateModel" is used for error Rate Model.
  
  > "ConstantSpeedPropagationDelayModel" is used as Propagation Delay Model & "FriisPropagationLossModel" is used for propagation Loss Model. 
***


Note in GNU Plots for 3 flows cooresponding to 8 STA at right side of dumbell:       
        
        
NOTE :



* The Corresponding code example `scratch/TCP-Ledbat-Evaluation/ledbat-simple-dumbell-topology` try to evaluates TCp Ledbat performance under wireless conditions . Here in this code we have make Two BSS attched via P2P links.
* We had derived the graph after running the experiment 
* We have capped the plot generation for maximum upto  8 STA stations at the right side so that the generated plots does not  flood the base directoy ns3.3x/ in case when 
* number of STA at the right side of dumbell is high say 30-40. as a result if not capped then around 60-80 files of (.plt and .png) will be genearated 

* In some version of ns3.3x example ns3.35 running the code might throw error which are actually warnings but compiler trated those as as error


* In that case better to run these experiments for ns3.32 (as these experimenst are performed in ns3.32 ) and before running we have to troublshoot this issue in following way:

*Troubleshooting:

    CXXFLAGS="-Wall" ./waf configure 
    ./waf -v

   
   
*If user want to see the required command line arguments (Assuming, the above troubleshooting has already been performed.):
 
    ./waf --run "scratch/TCP-Ledbat-Evaluation/ledbat-simple-dumbell-topology --help" 
    
