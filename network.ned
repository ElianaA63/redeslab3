simple Generator
{
    parameters:
        volatile double generationInterval; // sec
        volatile int packetByteSize;
        @display("i=block/source");
    gates:
        output out;
}

simple Queue
{
    parameters:
        volatile double serviceTime; // sec
        volatile int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        input in;
        output out;
}

simple Sink
{
    parameters:
        @display("i=block/sink");
    gates:
        input in;
}

simple TransportTx
{
    parameters:
        int bufferSize;
        volatile double serviceTime;
        volatile double timeout;
        @display("i=block/queue;queue=buffer");
    gates:
        input toApp;
        inout toOut;
}

simple TransportRx
{
    parameters:
        int bufferSize;
        volatile double serviceTime;
        @display("i=block/queue;queue=buffer");
    gates:
        output toApp;
        inout toOut;
}

module NodeTx
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
    submodules:
        gen: Generator {
            parameters:
                @display("p=75,50");
        }
        //queue: Queue {
        //    parameters:
        //        @display("p=75,190");
        //}
        traTx: TransportTx {
            parameters:
                @display("p=75,190");
        }
    connections:
        gen.out --> traTx.toApp;
        in --> traTx.toOut$i;
        traTx.toOut$o --> out;

        //gen.out --> queue.in;
        //queue.out --> out;
}

module NodeRx
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
    submodules:
        sink: Sink {
            parameters:
                @display("p=75,50");
        }
        //queue: Queue {
        //    parameters:
        //        @display("p=75,190");
        //}
        traRx: TransportRx {
            parameters:
                @display("p=75,190");
        }
    connections:
        in --> traRx.toOut$i;
        traRx.toOut$o --> out;
        //case study 1:
        //traRx.toApp --> {  datarate = 0.5Mbps; delay = 100us; } --> sink.in;
		//case study 2:
		traRx.toApp --> {  datarate = 1Mbps; delay = 100us; } --> sink.in;
        
        //in --> queue.in;
        //case study 1:
        //queue.out --> {  datarate = 0.5Mbps; } --> sink.in;
        //case study 2:
        //queue.out --> { datarate = 1Mbps; } --> sink.in;
}

network Network
{
    @display("bgl=2");
    submodules:
        nodeTx: NodeTx {
            @display("p=30,30");
        }
        //queue: Queue {
        //    @display("p=130,30");
        //}
        queue[2]: Queue {
            @display("p=130,30");
        }
        nodeRx: NodeRx {
            @display("p=230,30");
        }
    connections:
        nodeTx.out --> {  datarate = 1Mbps; delay = 100us; } --> queue[0].in; 
        //case study 1:
        //queue[0].out --> {  datarate = 1Mbps; delay = 100us; } --> nodeRx.in;
        //case study 2:
        queue[0].out --> {  datarate = 0.5Mbps; delay = 100us; } --> nodeRx.in;
        nodeRx.out --> {  datarate = 1Mbps; delay = 100us; } --> queue[1].in;
        queue[1].out --> {  datarate = 1Mbps; delay = 100us; } --> nodeTx.in;
		
		//nodeTx.out --> { datarate = 1Mbps; delay = 100us; } --> queue.in;
        //case study 1:
        //queue.out --> {  datarate = 1Mbps; delay = 100us; } --> nodeRx.in;
        //case study 2:
       //queue.out --> { datarate = 0.5Mbps; delay = 100us; } --> nodeRx.in;
}
