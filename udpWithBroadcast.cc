#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;



int main(int argc, char *argv[]){
  
  //Número de nós na rede Broadcast (não incluindo um nó misto da rede P2P)
  int numCsma = 3;

  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO); // Log que diz o tempo que levou para chegar no Cliente
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO); // Log que diz o tempo que levou para chegar no Servidor

  numCsma = numCsma == 0 ? 1 : numCsma;
  
  //Nós = Hosts
  //Criando os nós
  int numP2P = 4;
  NodeContainer p2pNodeGeral;
  p2pNodeGeral.Create(numP2P); //4 nós na rede P2P, 4 enlaces

  //Criando enlace dos nós da rede P2P
  NodeContainer p2pNode01 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(1));
  NodeContainer p2pNode02 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(2));
  NodeContainer p2pNode03 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(3));
  NodeContainer p2pNode12 = NodeContainer(p2pNodeGeral.Get(1), p2pNodeGeral.Get(2));


  //Criando enlace da rede Broadcast (incluindo um nó misto da rede P2P)
  NodeContainer csmaNodes;
  csmaNodes.Add(p2pNodeGeral.Get(numP2P-1)); //Nossa rede broadcast terá o último nó da rede P2P
  csmaNodes.Create(numCsma); //Nossa rede broadcast terá numCsma nós
  
  //Configuração da rede P2P
  //Point to Point é praticamente um link dedicado (dedica um canal de 5Mbps com delay de 2ms)
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
  
  //Interfaces de Rede
  NetDeviceContainer p2pDevice01 = pointToPoint.Install(p2pNode01);
  NetDeviceContainer p2pDevice02 = pointToPoint.Install(p2pNode02);
  NetDeviceContainer p2pDevice03 = pointToPoint.Install(p2pNode03);
  NetDeviceContainer p2pDevice12 = pointToPoint.Install(p2pNode12);
  
  //Configuração da rede CSMA (broadcast)
  //Configuração do canal único de comunicação da rede Broadcast
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));//NanoSeconds(6560)
  
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);
  
  //Instala pilha de Internet (permite o uso de protocolos TCP, UDP e IP)
  InternetStackHelper stack;
  for(int i = 0; i < numP2P - 1; i++){
    stack.Install(p2pNodeGeral.Get(i));
  }
  stack.Install(csmaNodes);
  
  //Acima foi criada toda a topologia da rede até a instalação da pilha de internet
  //Abaixo vem as coisas mais específicas de cada protocolo (TCP, UDP e IP)

  //Estabelecendo o endereçamento IPv4 para a rede P2P
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0"); //10.1. é o IP base dos nós da rede P2P
  address.Assign (p2pDevice01);
  address.SetBase("10.1.2.0", "255.255.255.0");
  address.Assign (p2pDevice02);
  address.SetBase("10.1.3.0", "255.255.255.0");
  address.Assign (p2pDevice03);
  address.SetBase("10.1.4.0", "255.255.255.0");
  address.Assign (p2pDevice12);
  
  //Estabelecendo o endereçamento IPv4 para a rede Broadcast
  address.SetBase("10.200.1.0", "255.255.255.0"); //10.200.1.0 é o IP base dos nós da rede broadcast
  Ipv4InterfaceContainer csmaInterfaces = address.Assign(csmaDevices);
  
  //SIMULAÇÃO 1 - UDP

  //Estabelece as aplicações cliente / Servidor
  UdpEchoServerHelper echoServer(9); //"escuta" a porta 9
  
  //Criando o servidor da rede Broadcast
  ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(numCsma)); //último nó da rede broadcast é o servidor
  serverApps.Start (Seconds(1.0)); //Depois de 1 segundo na rede o servidor começa a atuar
  serverApps.Stop(Seconds(10.0)); // Desligamos o servidor depois de 10s
  
  //Cria uma aplicação UDP que assinalamos o IP e porta do destinatario que enviaremos os pacotes
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress(numCsma), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue(1));
  echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize", UintegerValue(1024));
  
  //Instala a aplicação no nó 0 da rede P2P (primeiro "cliente")
  ApplicationContainer clientApps = echoClient.Install (p2pNodeGeral.Get(0)); //o primeiro nó da rede P2P é o remetente
  clientApps.Start(Seconds(1.0));
  clientApps.Stop(Seconds(10.0));
  
  //Tabela de Roteamento
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  
  //Habilita logs e  gera .PCAPS
  pointToPoint.EnablePcapAll("nodeP2P");
  csma.EnablePcap("nodeCsma", csmaDevices.Get(1), true);
  
  //Gera xml para usar no NetAnim
  AnimationInterface anim ("udpWithBroadcast.xml");

  //define posições do(s) node(s) P2P no NetAnim
  anim.SetConstantPosition (p2pNodeGeral.Get(0), 10.0, 10.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(1), 5.0, 20.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(2), 15.0, 20.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(3), 25.0, 10.0);
  anim.SetConstantPosition (csmaNodes.Get(1), 35.0, 10.0);
  anim.SetConstantPosition (csmaNodes.Get(2), 25.0, 20.0);
  anim.SetConstantPosition (csmaNodes.Get(3), 35.0, 20.0);
 
  Simulator::Run();
  Simulator::Destroy();
  return 0;
 }