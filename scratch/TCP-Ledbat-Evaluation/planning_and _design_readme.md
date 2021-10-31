# Planning/Design Document for Exhaustive Evaluation of TCP Ledbat in WiFi Environment.


## Project Overview :

<p> 
  Low Extra Delay Background Transport </b>(LEDBAT) [1]</b>  is Less than Best Effort <b>(LBE)</b>  type of transport.  It is an experimental delay-based congestion control algorithm that seeks to utilise the available bandwidth on an end-to-end path while limiting the consequent increase in queueing delay on that path. LEDBAT uses changes in one-way delay <b>(OWD)</b> measurements to limit congestion that the flow itself induces in the network. LEDBAT is designed for use by background bulk-transfer applications to be no more aggressive than standard TCP congestion control (as specified in RFC 5681) and to yield in the presence of competing flows, thus limiting interference with the network performance of competing flows. LEDBAT is used by Apple for software updates and by BitTorrent for the majority of its transfers and by Microsoft SCCM software distribution points.LEDBAT was once estimated to carry 13-20% of Internet Traffic.
  
</p>

<br />

<p>
  This project aims to evaluate the performance of LEDBAT in WiFi environments, including Gigabit WiFi, with and without Rate Adaptation.
</p>



## Goals and Deliverables :

Clearly, the goal of this assignment is to come up with at least three to six exhaustive evaluation examples of programmes for analysing how Ledbat performs in wireless environments (WLAN/Wifi).

We have planned to come up with examples of programmes which are scalable in nature. Users can pass Command Line Arguments before running the programme to simulate (say)
<ul>
 <li> For the desired simulation time</li>
 <li> For the desired rate adaptation algorithm </li>
 <li> For the desired error rate model/mobility </li>
 <li> For the desired number of nodes in BSS (in some of the programmes), </li>
 <li> For linkrate and linkdelay in some cases, e.t.c. </li>
</ul>


The required arguments for each programe can be accesible via `--help` flag as proper argument message will be display for better understanding.

Example : 

    ./waf --run "<ledbat-evaluation-filename> --simulationTime=300s --rateAdaptationAlgo=aarf errorRateModel=yansErrorRate"
    
For Running with default arguments just type following: 

    ./waf --run  <ledbat-evaluation-filename> 
    
Finally, calculating Throughput and generating corresponding plots for evaluation purposes for the different command line arguments example for different rate adaptation algorithms.

The Link Delays and Link Rate we did not specify in beloe images as that will be dynamic in nature and will depend upon the value entered by user (as mentioned in above section).

The Evaluation Examples Experiments are planned to carried on 802.11 b/g and 802.11ac Gigabit Wifi Standards.
<b r/>

## Exhaustive Evaluation Examples ( Topology Design ) :
 
 <p>
The Images for the topology are created by team itself by use of online available tool  : https://app.diagrams.net/  

  
</p>

<br />

<ol>
  <li>  Simple pRp wireless network Tolpology <br />
   <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/simple-pRp.drawio.png"> <br />
   <p> This is an simple node(STA)-AP-node(STA) Topology. The STAs will be configured to have a mobility model helping simulator to calculate received signal.
     </p>
    <br />
  </li>
      
  <li> Simple  Dumbell Topology <br /> <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/simple-dumpbell.png"> <br />
    <p>This is a Simple Dumbell Topology having 3 STA connected to a AP0 and another 3 STAs connected to another AP1 producing a Two BSS interaction example. 
    </p>
    <br />
  </li>
  
  
  <li> Flexible Dumbell Topology <br />
 <img src="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/flexible-dumpbell.png"> <br />
     <p>
       This is a Flexible Dumbell Topology, the main difference between Simple dumbell and Flexible  is justified by its name only i.e in former the topology is fixed type while in latter we will take user's command line arguments (by default the topology hold its inheritence as that of simple dumbell i.e 3 STA connected to a AP0 and another 3 STAs connected to another AP1 ), thus users can simulate for the topology by passing the number of nodes for BSS0 (AP0) AND BSS1 (AP1) thus producing a two BSS interaction example. 
  </p>
    <br />
  </li>

   <li> Star Topology <br />
     <img src ="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/star topology.png"> <br />
       <p>
         In this example we will try to consider a star topology  forming a BSS i.e at center we will be having AP and outside nodes as STA. We will use a mobility model as required by simulator for calculating received signal. 
     </p>
     <br />
  </li>
 
  <li>
  WiFi and Ethernet Combination Topology <br />
    <img src ="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/Combination Topology.png">  <br />
    <p>
      In this example we will combine Ethernet network connected to a WiFi AP that in turn connected to WiFi nodes thus wifi network forming a BSS. The set of  Ethernet nodes will be on single CsmaChannel, thus in same broadcast domain. One of these (Ethernet) nodes has a PointToPointNetDevice for a connection to the WiFi AP of  WiFi network, so the AP also has a PointToPointNetDevice as well. The WiFi nodes (STA) will have WifiNetDevice attached to each of them. Again, all STA will share a single Channel object, YansWifiChannel. We will also consider  various mobility model for STA because as we know STA are mobile in nature and besides simulator needs coordinate/distance to calculate the received signal. This example is inspired from <a href="https://gitlab.com/nsnam/ns-3-dev/-/blob/master/examples/tutorial/third.cc"> here </a>. After playing with the script and configuring above topology for ledbat simulation , We have planned to extend this exampe with making the last hop as wifi(wireless) instead of ethernet so that simulation can be experimented in completely wifi environment .
    </p>
    <br />
  </li>
  
   <li>  Triangular Topology <br /> <img src = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/Topology-Images/traingular.png">  <br />
     <p>This example consist of traigular( 3 BSS ) and each having three nodes(STA) though the implementor will extend this example to x, y, z (3, 3, 3 for all three BSS by default) for BSS0, BSS1 and BSS2 respectively), and the number of nodes's value will be passed by user during run-time. A similar idea as that of flexible dumbell topology.
       </p>
     <br />
  </li>
 </ol>




## Timelines :
<br />

  <table>
    <thead>
      <tr>
        <th>Week</th>
        <th>Task</th>
      </tr>
    </thead>
    <tbody>
        <tr>
            <td>Week 1</td>
            <td>
              <ul>
                <li> Revising ns3 documentation.</li>
                <li> Reading helper classes. </li>
                <li> Understanding code-base for WiFi module a listed in References. </li>
                <li> Implementation will be Initiated.</li>
              </ul>
          </td>
        </tr>
        <tr>
            <td>Week 2</td>
           <td>
              <ul>
                <li> Starting to work with simple P-R-P, Simple dumbell and Star. </li>
                <li> Analyze performance measuring throughput and generating graphs. </li>
              </ul>
          </td>
        </tr>
      <tr>
            <td>Week 3</td>
           <td>
              <ul>
                <li> Implementing the rest mentioned topologies.</li>
                <li> Followed by analyzing the performance of TCP Ledbat for different Rate Adaptation Algorithms and generating plots. </li>
              </ul>
          </td>
        </tr>
      <tr>
            <td>Week 4</td>
           <td>
              <ul>
                <li> Working on Documentation of all those topologies stating the configuration done. </li>
              </ul>
          </td>
        </tr>
    </tbody>
  </table>

<br />

## References :

<ol>
  <li> <a href ="https://tools.ietf.org/html/rfc6817"> LEDBAT: RFC 6817 </a> </li>
  <li> <a href="http://ieeexplore.ieee.org/document/6814737/"> Less-than-Best-Effort Service for Community Wireless Networks: Challenges at Three Layers </a>  </li>
  <li> <a href="https://www.researchgate.net/publication/257517254_Exploration_and_evaluation_of_traditional_TCP_congestion_control_techniques" > Exploration and evaluation of traditional TCP congestion control techniques </a>  </li>
  <li> <a href="https://ieeexplore.ieee.org/document/5520827"> Performance evaluation of a Python implementation of the new LEDBAT congestion control algorithm </a>  </li>
  <li> <a href="https://datatracker.ietf.org/doc/html/draft-irtf-iccrg-tcpeval"> Common TCP Evaluation Suite draft-irtf-iccrg-tcpeval-01 </a> </li>
  <li> <a href="https://www.nsnam.org/docs/models/html/wifi-design.html"> Wi-Fi architecture in ns-3 </a> </li>
  <li> <a href="https://www.nsnam.org/docs/models/html/wifi-user.html"> User documentation for Wi-Fi simulations using ns-3 </a> </li>
</ol>


