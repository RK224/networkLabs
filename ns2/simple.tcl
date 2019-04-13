#Create a simulator object
set ns [new Simulator]


#Open the output files
set f0 [open outSimple0.tr w]

#Open the nam trace file
set nf [open out.nam w]
$ns namtrace-all $nf

$ns trace-all $f0
#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
	#Close the trace file
        close $nf
	#Execute nam on the trace file
        exec nam out.nam &
        exit 0
}

#Define a procedure that attaches a UDP agent to a previously created node
#'node' and attaches an Expoo traffic generator to the agent with the
#characteristic values 'size' for packet size 'burst' for burst time,
#'idle' for idle time and 'rate' for burst peak rate. The procedure connects
#the source with the previously defined traffic sink 'sink' and returns the
#source object.
proc attach-expoo-traffic { node sink size burst idle rate } {
	#Get an instance of the simulator
	set ns [Simulator instance]

	#Create a UDP agent and attach it to the node
	set source [new Agent/UDP]
	$ns attach-agent $node $source

	#Create an Expoo traffic agent and set its configuration parameters
	set traffic [new Application/Traffic/Exponential]
	$traffic set packetSize_ $size
	$traffic set burst_time_ $burst
	$traffic set idle_time_ $idle
	$traffic set rate_ $rate
        
        # Attach traffic source to the traffic generator
        $traffic attach-agent $source
	#Connect the source and the sink
	$ns connect $source $sink
	return $traffic
}


proc record {} {
        global null0 f0

        set ns [Simulator instance]

        set time .1

        set bw0 [$null0 set bytes_]

        set now [$ns now]

        puts $f0 "$now [expr $bw0/$time*8/1000000]"

        $null set bytes_ 0

        $ns at [expr $now+$time] "record"
}

#Create two nodes
set n0 [$ns node]
set n1 [$ns node]

#Create a duplex link between the nodes
$ns duplex-link $n0 $n1 1Mb 10ms DropTail

#Create a UDP agent and attach it to node n0
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0

# Create a CBR traffic source and attach it to udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
#$cbr0 attach-agent $udp0



#Create a Null agent (a traffic sink) and attach it to node n1
set null0 [new Agent/Null]
$ns attach-agent $n1 $null0

set ebr0 [attach-expoo-traffic $n0 $null0 500 2s 1s 100k]
$ebr0 attach-agent $udp0
$cbr0 attach-agent $udp0

#Connect the traffic source with the traffic sink
$ns connect $udp0 $null0  

#Schedule events for the CBR agent
$ns at 0.5 "$ebr0 start"
$ns at 4.5 "$ebr0 stop"
$ns at 1.5 "$cbr0 start"
$ns at 5.5 "$cbr0 start"

#Call the finish procedure after 5 seconds of simulation time
$ns at 6.0 "finish"

#Run the simulation
$ns run
