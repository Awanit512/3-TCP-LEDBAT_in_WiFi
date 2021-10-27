
<h1><i>#3</i> Exhaustive evaluation of TCP LEDBAT in WiFi environments.</h1>
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
   
   Awanit Ranjan ([181CO161](https://github.com/Awanit512)) <br />
   Keerti Chaudhary ([181CO226](https://github.com/keerti2001)) <br />
   Yerramaddu Jahnavi ([181CO260](https://github.com/janubangalore))

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
  <t/head>
  <tbody>
   <tr>
     <td>EDBAT: RFC 6817</td>
     <td>https://tools.ietf.org/html/rfc6817</td>
   </tr>

   <tr>
     <td>Less-than-Best-Effort Service for Community Wireless Networks: Challenges at Three Layers</td>
     <td>http://ieeexplore.ieee.org/document/6814737/</td>
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
  <t/head>
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

   
  
