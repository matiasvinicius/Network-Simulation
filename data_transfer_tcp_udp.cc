#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("simulacao"); //Nome do log de saída

int main(int argc, char *argv[]){
  bool verbose = true;
  uint32_t nCsma = 3;
  
  CommandLine cmd;
  cmd.AddValue("nCsna", "Numeros de nos extras rede CSMA", nCsma);
  cmd.AddValue("verbose", "Habilita logs de aplicacoes", verbose);
  cmd.Parse(argc, argv);
  
  if(verbose){
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }
  
  nCsma = nCsma == 0 ? 1 : nCsma;
  
  //Nós = Hosts
  //Criando os nós
  NodeContainer p2pNodes;
  p2pNodes.Create(2); //2 nós na rede P2P
  
  NodeContainer csmaNodes;
  csmaNodes.Add(p2pNodes.Get(1)); //Nossa rede broadcast terá um nó da rede P2P
  csmaNodes.Create(nCsma);
  
  //Configuração da rede P2P
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
  
  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install(p2pNodes);
  
  //Configuração da rede CSMA (broadcast)
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
  
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);
  
  //Instala pilha de Internet
  InternetStackHelper stack;
  stack.Install(p2pNodes.Get(0));
  stack.Install(csmaNodes);
  
  //Estabelecer o endereçamento IPv4
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
  
  address.SetBase("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign(csmaDevices);
  
  //Estabelece as aplicações cliente / Servidor
  UdpEchoServerHelper echoServer(9);
  
  ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
  serverApps.Start (Seconds(1.0));
  serverApps.Stop(Seconds(10.0));
  
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress(nCsma), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue(1));
  echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize", UintegerValue(1024));
  
  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get(0));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10.0));
  
  //Roteamento
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  
  //Habilita logs e  gera .PCAPS
  pointToPoint.EnablePcapAll("second");
  csma.EnablePcap("second", csmaDevices.Get(1), true);
  
  //Gera xml para usar no NetAnim
  AnimationInterface anim ("simulacao.xml");

  //define posições dos nodes
  anim.SetConstantPosition (p2pNodes.Get(0), 10.0, 10.0);
  anim.SetConstantPosition (csmaNodes.Get(0), 20.0, 20.0);
  anim.SetConstantPosition (csmaNodes.Get(1), 20.0, 30.0);
  anim.SetConstantPosition (csmaNodes.Get(2), 30.0, 20.0);
  anim.SetConstantPosition (csmaNodes.Get(3), 30.0, 30.0);

  Simulator::Run();
  Simulator::Destroy();
  return 0;
 }
