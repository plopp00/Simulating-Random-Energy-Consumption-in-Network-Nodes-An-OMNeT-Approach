package Project.datateknik_fordjupning;

simple Txc
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
}

network TictocNetwork
{
    @display("bgb=460,286");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        tic[6]: Txc {
            @display("p=230,95;is=l");
        }
    connections:
        tic[0].gate++ <--> Channel <--> tic[1].gate++;
        tic[1].gate++ <--> Channel <--> tic[2].gate++;
        tic[0].gate++ <--> Channel <--> tic[4].gate++;
        tic[0].gate++ <--> Channel <--> tic[5].gate++;
        tic[1].gate++ <--> Channel <--> tic[4].gate++;
        tic[1].gate++ <--> Channel <--> tic[5].gate++;
        tic[2].gate++ <--> Channel <--> tic[3].gate++;
        tic[2].gate++ <--> Channel <--> tic[4].gate++;
        tic[3].gate++ <--> Channel <--> tic[1].gate++;
        tic[3].gate++ <--> Channel <--> tic[4].gate++;
        tic[4].gate++ <--> Channel <--> tic[5].gate++;
}

