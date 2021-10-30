# Planning/Design Document for Exhaustive Evaluation of TCP Ledbat in WiFi Environment.


## Project Overview :

<p> 
  Low Extra Delay Background Transport </b>(LEDBAT)</b>  is Less than Best Effort <b>(LBE)</b>  type of transport.  It is an experimental delay-based congestion control algorithm that seeks to utilise the available bandwidth on an end-to-end path while limiting the consequent increase in queueing delay on that path. LEDBAT uses changes in one-way delay <b>(OWD)</b> measurements to limit congestion that the flow itself induces in the network. LEDBAT is designed for use by background bulk-transfer applications to be no more aggressive than standard TCP congestion control (as specified in RFC 5681) and to yield in the presence of competing flows, thus limiting interference with the network performance of competing flows. 
  
</p>

<br />

<p>
  This project aims to evaluate the performance of LEDBAT in WiFi environments, including Gigabit WiFi, with and without Rate Adaptation.
</p>



## Goals and Deliverables :

Clearly, the goal of this assignment is to come up with at least three to six exhaustive evaluation examples of programmes for analysing how Ledbat performs in wireless environments (WLAN/Wifi).

We have planned to come up with examples of programmes which are scalable in nature. Users can pass Command Line Arguments before running the programme to simulate (say)
<ul>
 <li> [LEDBAT: RFC 6817](https://tools.ietf.org/html/rfc6817)</li>
 <li> For the desired rate adaptation algorithm </li>
 <li> For the desired error rate model/mobility </li>
 <li> For the desired number of nodes in BSS (in some of the programmes), </li>
 <li> For linkrate and linkdelay in some cases, e.t.c. </li>
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


## Timelines ..



## References

<ol>
  <li>LEDBAT: RFC 6817</li>
  <li>Less-than-Best-Effort Service for Community Wireless Networks: Challenges at Three Layers </li>
  <li>Exploration and evaluation of traditional TCP congestion control techniques</li>
  <li>Performance evaluation of a Python implementation of the new LEDBAT congestion control algorithm</li>
  <li>Common TCP Evaluation Suite draft-irtf-iccrg-tcpeval-01</li>
</ol>

