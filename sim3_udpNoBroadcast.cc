#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("simulacao");//permite a adoção de logs durante o código

int main(int argc, char *argv[]){
  
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO); // Log que diz o tempo que levou para chegar no Cliente
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO); // Log que diz o tempo que levou para chegar no Servidor
  
  //Nós = Hosts
  //Criando os nós
  int numP2P = 7;
  NodeContainer p2pNodeGeral;
  p2pNodeGeral.Create(numP2P); //7 nós na rede P2P, 10 enlaces

  //Criando os enlaces dos nós da rede P2P
  NodeContainer p2pNode01 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(1));
  NodeContainer p2pNode02 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(2));
  NodeContainer p2pNode03 = NodeContainer(p2pNodeGeral.Get(0), p2pNodeGeral.Get(3));
  NodeContainer p2pNode12 = NodeContainer(p2pNodeGeral.Get(1), p2pNodeGeral.Get(2));
  NodeContainer p2pNode34 = NodeContainer(p2pNodeGeral.Get(3), p2pNodeGeral.Get(4));
  NodeContainer p2pNode35 = NodeContainer(p2pNodeGeral.Get(3), p2pNodeGeral.Get(5));
  NodeContainer p2pNode36 = NodeContainer(p2pNodeGeral.Get(3), p2pNodeGeral.Get(6));
  NodeContainer p2pNode45 = NodeContainer(p2pNodeGeral.Get(4), p2pNodeGeral.Get(5));
  NodeContainer p2pNode46 = NodeContainer(p2pNodeGeral.Get(4), p2pNodeGeral.Get(6));
  NodeContainer p2pNode56 = NodeContainer(p2pNodeGeral.Get(5), p2pNodeGeral.Get(6));


  //Configuração da rede P2P
  //Point to Point é praticamente um link dedicado (dedica um canal de 5Mbps com delay de 2ms)
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("512Kbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
  
  //Interfaces de Rede
  NetDeviceContainer p2pDevice01 = pointToPoint.Install(p2pNode01);
  NetDeviceContainer p2pDevice02 = pointToPoint.Install(p2pNode02);
  NetDeviceContainer p2pDevice03 = pointToPoint.Install(p2pNode03);
  NetDeviceContainer p2pDevice12 = pointToPoint.Install(p2pNode12);
  NetDeviceContainer p2pDevice34 = pointToPoint.Install(p2pNode34);
  NetDeviceContainer p2pDevice35 = pointToPoint.Install(p2pNode35);
  NetDeviceContainer p2pDevice36 = pointToPoint.Install(p2pNode36);
  NetDeviceContainer p2pDevice45 = pointToPoint.Install(p2pNode45);
  NetDeviceContainer p2pDevice46 = pointToPoint.Install(p2pNode46);
  NetDeviceContainer p2pDevice56 = pointToPoint.Install(p2pNode56);


  //Instala pilha de Internet (permite o uso de protocolos TCP, UDP e IP)
  InternetStackHelper stack;
  for(int i = 0; i < numP2P; i++){
    stack.Install(p2pNodeGeral.Get(i));
  }

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
  address.SetBase("10.1.5.0", "255.255.255.0");
  address.Assign (p2pDevice34);
  address.SetBase("10.1.6.0", "255.255.255.0");
  address.Assign (p2pDevice35);
  address.SetBase("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer destinatario = address.Assign (p2pDevice36);
  address.SetBase("10.1.8.0", "255.255.255.0");
  address.Assign (p2pDevice45);
  address.SetBase("10.1.9.0", "255.255.255.0");
  address.Assign (p2pDevice46);
  address.SetBase("10.1.10.0", "255.255.255.0");
  address.Assign(p2pDevice56);

  //SIMULAÇÃO 1 - UDP
  

  //Estabelece as aplicações cliente / Servidor
  UdpEchoServerHelper echoServer(9); //"escuta" a porta 9

  ApplicationContainer serverApps = echoServer.Install(p2pNodeGeral.Get(numP2P-1)); //último nó da rede é o destinatário (servidor)
  serverApps.Start (Seconds(1.0)); //Depois de 1 segundo na rede o servidor começa a atuar
  serverApps.Stop(Seconds(10.0)); // Desligamos o servidor depois de 10s


  //Cria uma aplicação UDP na qual assinalamos o IP e porta do servidor que enviaremos os pacotes (n6)
  UdpEchoClientHelper echoClient (destinatario.GetAddress(1), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue(1));
  echoClient.SetAttribute("Interval", TimeValue(Seconds(0)));
  echoClient.SetAttribute("PacketSize", UintegerValue(1024));
  
  //Cria uma aplicação UDP na qual assinalamos o IP e porta do servidor que enviaremos os pacotes (n3)
  UdpEchoClientHelper echoClient2 (destinatario.GetAddress(1), 9);
  echoClient2.SetAttribute("MaxPackets", UintegerValue(1));
  echoClient2.SetAttribute("Interval", TimeValue(Seconds(0)));
  echoClient2.SetAttribute("PacketSize", UintegerValue(20480));

  //Instala a aplicação no nó 1 da rede P2P (primeiro "cliente")
  ApplicationContainer clientApps = echoClient.Install (p2pNodeGeral.Get(1)); //o 1º nó da rede P2P é "cliente"
  clientApps.Start(Seconds(1.0));
  clientApps.Stop(Seconds(10.0));

  ApplicationContainer clientApps2 = echoClient2.Install (p2pNodeGeral.Get(2)); //o 2º nó da rede P2P também é "cliente"
  clientApps2.Start(Seconds(1.0));
  clientApps2.Stop(Seconds(10.0));

  //Tabela de Roteamento
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //Habilita logs e  gera .PCAPS
  pointToPoint.EnablePcapAll("sim3_udp");

  //Gera xml para usar no NetAnim
  AnimationInterface anim ("sim3_udpNoBroadcast.xml");

  //define posições do(s) node(s) P2P no NetAnim
  anim.SetConstantPosition (p2pNodeGeral.Get(0), 10.0, 10.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(1), 5.0, 20.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(2), 15.0, 20.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(3), 25.0, 10.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(4), 35.0, 10.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(5), 25.0, 20.0);
  anim.SetConstantPosition (p2pNodeGeral.Get(6), 35.0, 20.0);

  
  Simulator::Run();
  Simulator::Destroy();
  return 0;
 }