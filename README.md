
<h1><i>#3</i> Exhaustive evaluation of TCP LEDBAT in WiFi environments.</h1>
<h3>Planning & Design Description Doc can be found here : <a href = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/scratch/TCP-Ledbat-Evaluation/planning_and%20_design_readme.md"> planning_and_design_readme.md </a> </h3>

<p>
  Our <b> Exmaple Code and Corresponding GNU Plots </b> are in: <br> 
 <table>
   <tbody>
  <tr> <td> Branch  : <b>`exhaustive_evaluation_ledbat`</b>  </td> </tr>
   <tr> <td> Directory : <b> scratch directory </b>  </td> </tr>
  <tr> <td> In the scratch directory we have two more directory </td> </tr>
   <tr> <td> <b> One of them is  TCP-Ledbat-Evaluation </b> which contains our example code  <a href="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/tree/exhaustive_evaluation_ledbat/scratch/TCP-Ledbat-Evaluation"> here </a> is the link</td> </tr> 
   <tr> <td>On the other hand the Second directory   <b> TCP-LEDBAT-Graphs-Traces </b> <a href="https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/tree/exhaustive_evaluation_ledbat/scratch/TCP-LEDBAT-Graphs-Traces">here</a> .This holds the corresponding GNU plots of individual throughput and Combined Throughput for flows of sink Nodes/STA(stations).</td> </tr> 

   <tr> <td>  
     Inside TCP-LEDBAT-Graphs-Traces each Toplogy has its own Folder conating plt, png files of GNU plots and some has xml file for animations which can be viewed      using NetAnim Tool. These folder also contains readme.md stating configuration done / command line arguments passed for geenrating such garphs/results.
     </td> </tr> 
   </tbody>
</table>
</p>


***
  
  According To the planned Task for this Assignment Stated in <a href = "https://github.com/Awanit512/3-TCP-LEDBAT_in_WiFi/blob/exhaustive_evaluation_ledbat/scratch/TCP-Ledbat-Evaluation/planning_and%20_design_readme.md"> `planning_and_design_readme.md` </a>

<p>
  We have Implemented Example Code for performing Exhaustive Evaluation of TCP LEDBAT in WiFi environments. Thus Corresponding to this the team has come up with 5-6 examples code measuring througput of sink Nodes for different types of flows and in different scenarios example for former includes Tcp Ledbat Flows, TcpNewRenoFlows / or other Tcp Varient and for latter we have simualted the experiment for dfifferent wifi-standard, different rate adaption algorithm, diffrent Error rate Model e.t.c as well tried to write a scalable code thus giving more flexibility to user to pass desired command line arguments for which user wants to experiment the simulation (those arguments can be shown by use of -help flag while running the example).
</p>

<br>

***

<p> Below shows a brief view Of Ledbat ALgorithm which is kept it for reference. </p>
<table>
<tr>
  <td><b>Brief:</b></td>
  <td>
   Low Extra Delay Background Transport (LEDBAT) is Less than Best Effort (LBE) type of
 transport. BitTorrent and Apple Inc use variants of LEDBAT as their default transport
 mechanisms for large-sized file transfers. This project aims to evaluate the performance of
 LEDBAT in WiFi environments, including Gigabit WiFi, with and without Rate Adaptation.
  </td>
</tr>
  <tr>
  <td><b>Contributors:</b></td>
  <td>
   <b>Mentor</b> <br/>
   Mohit P. Tahiliani [Assistant Prof in the Dept. CSE NITK Surathkal, Mangalore, India](<a href="https://gitlab.com/mohittahiliani"> Github Repo </a>)
     <br />
   <b>Mentee</b> <br />
  
   Awanit Ranjan      [181CO161] (<a href="https://github.com/Awanit512"> Github Repo </a>)     <br />
   Keerti Chaudhary   [181CO226] (<a href="https://github.com/keerti2001"> Github Repo </a>)    <br />
   Yerramaddu Jahnavi [181CO260] (<a href="https://github.com/janubangalore"> Github Repo </a>) <br />
   

  </td>
</tr>

<tr>
 
 <td><b>Recommended Reading</b>:</td>
 <td> 
  <table>
   <thead>
    <tr>
     <td><b>Description</b></td>
      <td><b>Link</b></td>
   </tr>
  </thead>
  <tbody>
    
   <tr>
     <td>LEDBAT: RFC 6817</td>
     <td> <a href="https://tools.ietf.org/html/rfc6817"> Link </a></td>
   </tr>

   <tr>
     <td>Less-than-Best-Effort Service for Community Wireless Networks: Challenges at Three Layers</td>
     <td> <a href="http://ieeexplore.ieee.org/document/6814737/"> Link </a></td> 
    </tr>


   <tr>
     <td>Exploration and evaluation of traditional TCP congestion control techniques</td>
     <td> <a href="https://www.researchgate.net/publication/257517254_Exploration_and_evaluation_of_traditional_TCP_congestion_control_techniques"> Link </a></td> 
    </tr>
    
    
   <tr>
     <td> Performance evaluation of a Python implementation of the new LEDBAT congestion control algorithm </td>
     <td> <a href="https://ieeexplore.ieee.org/document/5520827"> Link </a> </td> 
    </tr>
    
        
   <tr>
     <td>  Common TCP Evaluation Suite draft-irtf-iccrg-tcpeval-01 </td>
     <td> <a href="https://datatracker.ietf.org/doc/html/draft-irtf-iccrg-tcpeval"> Link </a> </td> 
    </tr>
    
  <tr>
     <td>  WiFi architecture in ns-3 </td>
     <td> <a href="https://www.nsnam.org/docs/models/html/wifi-design.html"> Link </a> </td> 
    </tr>
    
   
  </tbody>
  </table>
</td>
    </tr>
   
   
   <tr>
 
 <td><b>LEDBAT Operations/Algorithm</b>:</td>
 <td> 
  <table>
   <thead>
    <tr>
      <td><b>Sender side and Receiver side operations</b></td>
      <td><b>Pseudocode / Mechanisms</b></td>
   </tr>
  </thead>
  <tbody>
   <tr>
     <td>Receiver side </td>
     <td><pre lang="csharp">
   on data_packet:
       remote_timestamp = data_packet.timestamp
       acknowledgement.delay = local_timestamp() - remote_timestamp
       // fill in other fields of acknowledgement
       acknowledgement.send()
   </pre></td>
   </tr>

   <tr>
     <td>Sender side </td>
     <td><pre lang="csharp">
on initialization:
  set all NOISE_FILTER delays used by current_delay() to +infinity
  set all BASE_HISTORY delays used by base_delay() to +infinity
  last_rollover = -infinity # More than a minute in the past.
</pre>
<pre lang="csharp">
on acknowledgement:
  delay = acknowledgement.delay
  update_base_delay(delay)
  update_current_delay(delay)
  queuing_delay = current_delay() - base_delay()
  off_target = TARGET - queuing_delay + random_input()
  cwnd += GAIN * off_target / cwnd
  // flight_size() is the amount of currently not acked data.
  max_allowed_cwnd = ALLOWED_INCREASE + TETHER*flight_size()
  cwnd = min(cwnd, max_allowed_cwnd)
</pre>
<pre lang="csharp">
random_input()
  // random() is a PRNG between 0.0 and 1.0
  // NB: RANDOMNESS_AMOUNT is normally 0
  RANDOMNESS_AMOUNT * TARGET * ((random() - 0.5)*2)
</pre>
<pre lang="csharp">
update_current_delay(delay)
  // Maintain a list of NOISE_FILTER last delays observed.
  forget the earliest of NOISE_FILTER current_delays
  add delay to the end of current_delays
</pre>
<pre lang="csharp">
current_delay()
  min(the NOISE_FILTER delays stored by update_current_delay)
</pre>
<pre lang="csharp">
update_base_delay(delay)
  // Maintain BASE_HISTORY min delays. Each represents a minute.
  if round_to_minute(now) != round_to_minute(last_rollover)
    last_rollover = now
    forget the earliest of base delays
    add delay to the end of base_delays
  else
    last of base_delays = min(last of base_delays, delay)
</pre>
 <pre lang="csharp">
base_delay()
  min(the BASE_HISTORY min delays stored by update_base_delay)
   </pre></td>
    </tr>
  </tbody>
  </table>
</td>
    </tr>
   
  
</table>
