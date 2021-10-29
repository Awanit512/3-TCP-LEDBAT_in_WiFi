# Planning/Design Document for Exhaustive Evaluation of TCP Ledbat in WIFI Environment.


## Project Overview :

<p> 
  Low Extra Delay Background Transport </b>(LEDBAT)</b>  is Less than Best Effort <b>(LBE)</b>  type of transport.  It is an experimental delay-based congestion control algorithm that seeks to utilize the available bandwidth on an end-to-end path while limiting the consequent increase in queueing delay on that path. LEDBAT uses changes in one-way delay <b>(OWD)</b> measurements to limit congestion that the flow itself induces in the network. LEDBAT is designed for use by background bulk-transfer applications to be no more aggressive than standard TCP congestion control (as specified in RFC 5681) and to yield in the presence of competing flows, thus limiting interference with the network performance of competing flows. 
  
</p>

<br />

<p>
  This project aims to evaluate the performance of LEDBAT in WiFi environments, including Gigabit WiFi, with and without Rate Adaptation.
</p>



## Goals and Deliverables :

Cleraly, The Goal of this Assignment is to come up with say altlest three to six Exhaustive Evaulation Exmaples programs for analyzing how Ledbat performs in Wireless Environments ( WLAN / Wifi ). 

We have planned to come up with examples programs which are scalable in nature.
The users can pass Command LIne Arguments before running the programe to simulate (say) 
<ul>
 <li> For Desired Simulation Time </li>
 <li> For Desired Rate Adaption Algorithm </li>
 <li> For desired Error Rate Model/ Mobility </li>
 <li> For Desired Number of Nodes in BSS( in some of the programs ) </li>
 <li> For linkrate and linkdelay in some cases e.t.c..
</ul>


The required arguments for each programe can be accesible via `--help` flag as proper argument message will be display for better understanding.

Example : 

    ./waf --run "<ledbat-evulation-filename> --simulationTime=300s --rateAdaptionAlgo=aarf errorRateModel=yansErrorRate"
    
For Running on default argmumnets just type following: 
    ./waf --run  <ledbat-evaluation-filename> 
    
Finally, calculating Throughput and generating corresponding plots for Evaluation purposed for ifferent command line argumnets example for different rate adaption algoithms..

## Exhaustive Evaluation Examples ( Topology Design ) 

<ol>
  <li>  Simple p2p wireless network Tolpology <br />
   <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/simple-p2p.drawio.png"> <br />
  </li>
  
  <li> Flexible Dumbell Topology <br />
 <img src="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/flexible-dumpbell.png"> <br />
  </li>
    
  <li> Simple  Dumbell Topology <br /> <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/simple-dumpbell.png"><br />
  </li>

   <li>  Triangular Mesh Topology <br /> <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/traingular.png">  <br />
  </li>
    
 </ol>


## Timelines 



## References

