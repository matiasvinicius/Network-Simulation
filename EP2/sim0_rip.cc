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
  std::string SplitHorizon ("PoisonReverse");
  Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));

  /*
  Criação dos 35 nós, armazenados em uma estrutura de dados "NodeContainer"
  Dos 35, 26 são hosts e 9 são roteadores
  Como a estrutura dos nós é independente, eles podem ser criados em conjunto e armazenados na 
  mesma estrutura (uma para hosts e uma para roteadores)
  A diferenciação no pertencimento à cada rede, portanto, se dará a partir da criação dos enlaces
  */
  int totalHosts = 26;
  int totalRouters = 9;
  NodeContainer hosts;
  NodeContainer routers;
  hosts.Create(totalHosts); 
  routers.Create(totalRouters);
  /*
  Criação dos 44 enlaces para os 35 nós
  Aqui separamos os disposivos criados em conjunto e definimos com que máquina cada um se comunicará
  Como eles foram criados em uma sequência para serem amazenados na lista, diferenciaremos cada nó pela nomenclaura:
  h = host
  r = roteador
  Exemplos: 
  (1) h0h1 é o enlace entre o host 0 e o host 1
  (2) r0h0 é o enlace entre o roteador 0 e o host 0
  */

  //------------Configuração dos enlaces--------------------
  //Enlaces entre hosts
  //Rede 1
  NodeContainer h0h1 = NodeContainer(hosts.Get(0), hosts.Get(1));
  NodeContainer h0h2 = NodeContainer(hosts.Get(0), hosts.Get(2));
  NodeContainer h0h3 = NodeContainer(hosts.Get(0), hosts.Get(3));
  NodeContainer h1h4 = NodeContainer(hosts.Get(1), hosts.Get(4));
  NodeContainer h2h3 = NodeContainer(hosts.Get(2), hosts.Get(3));
  NodeContainer h2h4 = NodeContainer(hosts.Get(2), hosts.Get(4));
  NodeContainer h3h4 = NodeContainer(hosts.Get(3), hosts.Get(4));

  //Rede 2
  NodeContainer h5h6 = NodeContainer(hosts.Get(5), hosts.Get(6));
  NodeContainer h6h7 = NodeContainer(hosts.Get(6), hosts.Get(7));

  //Rede 3
  NodeContainer h8h9 = NodeContainer(hosts.Get(8), hosts.Get(9));

  //Rede 4
  NodeContainer h10h11 = NodeContainer(hosts.Get(10), hosts.Get(11));
  NodeContainer h10h12 = NodeContainer(hosts.Get(10), hosts.Get(12));
  NodeContainer h10h13 = NodeContainer(hosts.Get(10), hosts.Get(13));
  NodeContainer h12h13 = NodeContainer(hosts.Get(12), hosts.Get(13));

  //Rede 5
  NodeContainer h14h15 = NodeContainer(hosts.Get(14), hosts.Get(15));
  
  //Rede 6
  NodeContainer h16h17 = NodeContainer(hosts.Get(16), hosts.Get(17));
  
  //Rede 7
  NodeContainer h18h19 = NodeContainer(hosts.Get(18), hosts.Get(19));
  NodeContainer h18h20 = NodeContainer(hosts.Get(18), hosts.Get(20));
  NodeContainer h19h20 = NodeContainer(hosts.Get(19), hosts.Get(20));

  //Rede 8
  NodeContainer h21h22 = NodeContainer(hosts.Get(21), hosts.Get(22));
  NodeContainer h21h23 = NodeContainer(hosts.Get(21), hosts.Get(23));
  NodeContainer h22h23 = NodeContainer(hosts.Get(22), hosts.Get(23));
  NodeContainer h21h24 = NodeContainer(hosts.Get(21), hosts.Get(24));
  NodeContainer h24h25 = NodeContainer(hosts.Get(24), hosts.Get(25));

  //Elaces entre os roteadores
  NodeContainer r0r1 = NodeContainer(routers.Get(0), routers.Get(1));
  NodeContainer r0r2 = NodeContainer(routers.Get(0), routers.Get(2));
  NodeContainer r0r3 = NodeContainer(routers.Get(0), routers.Get(3));
  NodeContainer r1r2 = NodeContainer(routers.Get(1), routers.Get(2));
  NodeContainer r1r3 = NodeContainer(routers.Get(1), routers.Get(3));
  NodeContainer r2r3 = NodeContainer(routers.Get(2), routers.Get(3));

  NodeContainer r4r5 = NodeContainer(routers.Get(4), routers.Get(5));

  NodeContainer r6r7 = NodeContainer(routers.Get(6), routers.Get(7));
  NodeContainer r6r8 = NodeContainer(routers.Get(6), routers.Get(8));
  NodeContainer r7r8 = NodeContainer(routers.Get(7), routers.Get(8));

  NodeContainer r3r4 = NodeContainer(routers.Get(3), routers.Get(4));

  NodeContainer r5r6 = NodeContainer(routers.Get(5), routers.Get(6));
  
  //Enlaces entre Roteador e Host
  NodeContainer r0h0 = NodeContainer(routers.Get(0), hosts.Get(0));
  NodeContainer r1h5 = NodeContainer(routers.Get(1), hosts.Get(5));
  NodeContainer r2h8 = NodeContainer(routers.Get(2), hosts.Get(8));

  NodeContainer r4h10 = NodeContainer(routers.Get(4), hosts.Get(10));
  NodeContainer r5h14 = NodeContainer(routers.Get(5), hosts.Get(14));

  NodeContainer r6h16 = NodeContainer(routers.Get(6), hosts.Get(16));
  NodeContainer r7h18 = NodeContainer(routers.Get(7), hosts.Get(18));
  NodeContainer r8h21 = NodeContainer(routers.Get(8), hosts.Get(21));
  
  //---------------Configuração dos canais------------- 
  //Existem 2 configurações de canais para os Enlaces
  //Os resultados das análsies do relatório utilzaram apenas o channelLevel2
  PointToPointHelper channelLevel1;
  channelLevel1.SetDeviceAttribute("DataRate", StringValue("128kbps"));
  channelLevel1.SetChannelAttribute("Delay", StringValue("10ms"));
  
  PointToPointHelper channelLevel2;
  channelLevel2.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
  channelLevel2.SetChannelAttribute("Delay", StringValue("3ms"));

  //--------Atribuição dos canais aos enlaces----------
  //Rede 1 
  NetDeviceContainer h0h1_link = channelLevel2.Install(h0h1);
  NetDeviceContainer h0h2_link = channelLevel2.Install(h0h2);
  NetDeviceContainer h0h3_link = channelLevel2.Install(h0h3);
  NetDeviceContainer h1h4_link = channelLevel2.Install(h1h4);
  NetDeviceContainer h2h3_link = channelLevel2.Install(h2h3);
  NetDeviceContainer h2h4_link = channelLevel2.Install(h2h4);
  NetDeviceContainer h3h4_link = channelLevel2.Install(h3h4);
  
  //Rede 2
  NetDeviceContainer h5h6_link = channelLevel2.Install(h5h6);
  NetDeviceContainer h6h7_link = channelLevel2.Install(h6h7);

  //Rede 3
  NetDeviceContainer h8h9_link = channelLevel2.Install(h8h9);

  //Rede 4
  NetDeviceContainer h10h11_link = channelLevel2.Install(h10h11);
  NetDeviceContainer h10h12_link = channelLevel2.Install(h10h12);
  NetDeviceContainer h10h13_link = channelLevel2.Install(h10h13);
  NetDeviceContainer h12h13_link = channelLevel2.Install(h12h13);

  //Rede 5
  NetDeviceContainer h14h15_link = channelLevel2.Install(h14h15);

  //Rede 6
  NetDeviceContainer h16h17_link = channelLevel2.Install(h16h17);

  //Rede 7
  NetDeviceContainer h18h19_link = channelLevel2.Install(h18h19);
  NetDeviceContainer h18h20_link = channelLevel2.Install(h18h20);
  NetDeviceContainer h19h20_link = channelLevel2.Install(h19h20);
  
  //Rede 8 -> AS3
  NetDeviceContainer h21h22_link = channelLevel2.Install(h21h22);
  NetDeviceContainer h21h23_link = channelLevel2.Install(h21h23);
  NetDeviceContainer h22h23_link = channelLevel2.Install(h22h23);
  NetDeviceContainer h21h24_link = channelLevel2.Install(h21h24);
  NetDeviceContainer h24h25_link = channelLevel2.Install(h24h25);
  
  //Elaces entre os roteadores
  NetDeviceContainer r0r1_link = channelLevel2.Install(r0r1);
  NetDeviceContainer r0r2_link = channelLevel2.Install(r0r2);
  NetDeviceContainer r0r3_link = channelLevel2.Install(r0r3);
  NetDeviceContainer r1r2_link = channelLevel2.Install(r1r2);
  NetDeviceContainer r1r3_link = channelLevel2.Install(r1r3);
  NetDeviceContainer r2r3_link = channelLevel2.Install(r2r3);

  NetDeviceContainer r4r5_link = channelLevel2.Install(r4r5);
  
  NetDeviceContainer r6r7_link = channelLevel2.Install(r6r7);
  NetDeviceContainer r6r8_link = channelLevel2.Install(r6r8);
  NetDeviceContainer r7r8_link = channelLevel2.Install(r7r8);

  NetDeviceContainer r3r4_link = channelLevel2.Install(r3r4);

  NetDeviceContainer r5r6_link = channelLevel2.Install(r5r6);
  
  //Enlaces entre Roteador e Host
  NetDeviceContainer r0h0_link = channelLevel2.Install(r0h0);
  NetDeviceContainer r1h5_link = channelLevel2.Install(r1h5);
  NetDeviceContainer r2h8_link = channelLevel2.Install(r2h8);
 
  NetDeviceContainer r4h10_link = channelLevel2.Install(r4h10);
  NetDeviceContainer r5h14_link = channelLevel2.Install(r5h14);

  NetDeviceContainer r6h16_link = channelLevel2.Install(r6h16);
  NetDeviceContainer r7h18_link = channelLevel2.Install(r7h18);
  NetDeviceContainer r8h21_link = channelLevel2.Install(r8h21); 
  
  //---------Instalação da Pilha de Internet----------
  //Adiciona por padrão o protocolo IPv4, UDP e TCP nos hosts
  InternetStackHelper internetHosts;
  internetHosts.SetIpv6StackInstall (false);
  for(int i = 0; i < totalHosts; i++){
    internetHosts.Install(hosts.Get(i));
  }

  //Instalação da pilha com adição do protocolo RIP nos roteadores
  RipHelper ripRouting;
  Ipv4ListRoutingHelper listRH;
  listRH.Add (ripRouting, 0);
  InternetStackHelper internetRouters;
  internetRouters.SetIpv6StackInstall (false);
  internetRouters.SetRoutingHelper (listRH);
  for(int i = 0; i < totalRouters; i++){
    internetRouters.Install(routers.Get(i));
  }

  //-------------Atribuição dos endereços IP----------------
  Ipv4AddressHelper address;

  //Elaces entre os roteadores
  address.SetBase("192.168.2.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r0r1_link);

  address.SetBase("192.168.3.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r0r2_link);  

  address.SetBase("192.168.5.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r0r3_link);  

  address.SetBase("192.168.4.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r1r2_link); 

  address.SetBase("192.168.0.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r2r3_link);  

  address.SetBase("192.168.1.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r1r3_link);  

  address.SetBase("192.169.0.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r4r5_link);  

  address.SetBase("192.170.0.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r6r7_link);  

  address.SetBase("192.170.1.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r7r8_link); 

  address.SetBase("192.170.2.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r6r8_link);   

  address.SetBase("192.1.0.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r3r4_link);   

  address.SetBase("192.2.0.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r5r6_link);   

  //Enlaces entre Roteador e Host
  address.SetBase("192.168.7.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r0h0_link);   

  address.SetBase("192.168.8.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r1h5_link);

  address.SetBase("192.168.6.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r2h8_link);

  address.SetBase("192.169.1.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r4h10_link);

  address.SetBase("192.169.2.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r5h14_link);

  address.SetBase("192.170.5.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r6h16_link);
  
  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r8h21_link); 

  address.SetBase("192.170.4.0", "255.255.255.0", "0.0.0.0");
  address.Assign (r7h18_link);

  //IPs dos enlaces entre hosts
  //Rede 1
  address.SetBase("192.168.7.0", "255.255.255.0","0.0.0.2");
  address.Assign (h0h3_link);

  address.SetBase("192.168.7.0", "255.255.255.0","0.0.0.4");
  address.Assign (h3h4_link);

  address.SetBase("192.168.7.0", "255.255.255.0","0.0.0.6");
  address.Assign (h1h4_link);

  address.SetBase("192.168.7.0", "255.255.255.0","0.0.0.8");
  address.Assign (h0h1_link);

  address.SetBase("192.168.7.0", "255.255.255.0","0.0.0.10");
  address.Assign (h0h2_link);

  address.SetBase("192.168.7.0", "255.255.255.0", "0.0.0.12");
  address.Assign (h2h3_link);

  address.SetBase("192.168.7.0", "255.255.255.0", "0.0.0.14");
  address.Assign (h2h4_link);

  //Rede 2
  address.SetBase("192.168.8.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h5h6_link); 

  address.SetBase("192.168.8.0", "255.255.255.0", "0.0.0.4");
  address.Assign (h6h7_link);

  //Rede 3
  address.SetBase("192.168.6.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h8h9_link); 

  //Rede 4
  address.SetBase("192.169.1.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h10h11_link);  

  address.SetBase("192.169.1.0", "255.255.255.0", "0.0.0.4");
  address.Assign (h10h12_link); 

  address.SetBase("192.169.1.0", "255.255.255.0", "0.0.0.6");
  address.Assign (h10h13_link); 

  address.SetBase("192.169.1.0", "255.255.255.0", "0.0.0.8");
  address.Assign (h12h13_link); 

  //Rede 5
  address.SetBase("192.169.2.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h14h15_link); 

  //Rede 6
  address.SetBase("192.170.5.0", "255.255.255.0","0.0.0.2");
  address.Assign (h16h17_link);

  //Rede 7
  address.SetBase("192.170.4.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h18h19_link); 

  address.SetBase("192.170.4.0", "255.255.255.0", "0.0.0.4");
  address.Assign (h19h20_link); 

  address.SetBase("192.170.4.0", "255.255.255.0", "0.0.0.6");
  address.Assign (h18h20_link); 

  //Rede 8
  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.2");
  address.Assign (h21h23_link); 

  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.4");
  address.Assign (h22h23_link);  

  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.6");
  address.Assign (h21h22_link);  

  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.8");
  address.Assign (h21h24_link);  

  address.SetBase("192.170.3.0", "255.255.255.0", "0.0.0.10");
  Ipv4InterfaceContainer destinatario = address.Assign (h24h25_link);

  //----CAPTURA DA TABELA DE ROTEAMENTO--------- 
  Ptr<Ipv4StaticRouting> staticRouting;
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (routers.Get(0)->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (routers.Get(7)->GetObject<Ipv4> ()->GetRoutingProtocol ());

  RipHelper routingHelper;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);

  //Log no terminal das tabelas de roteamento nos segundos 0,12,13 e 40 para os roteadores 0 e 7
  routingHelper.PrintRoutingTableAt (Seconds (0), routers.Get(0), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (12), routers.Get(0), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (13), routers.Get(0), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (40), routers.Get(0), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (0), routers.Get(7), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (12), routers.Get(7), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (13), routers.Get(7), routingStream);
  routingHelper.PrintRoutingTableAt (Seconds (40), routers.Get(7), routingStream);
  
  //Rota realizada no arquivo .tr
  AsciiTraceHelper ascii;
  channelLevel2.EnableAsciiAll (ascii.CreateFileStream ("sim0_rip.tr"));

  //Tabela de Roteamento
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //Gera xml para usar no NetAnim
  AnimationInterface anim ("sim0_rip.xml");

  //define posições do(s) node(s) P2P no NetAnim
  anim.SetConstantPosition (h0h3.Get(0), 25.0, 50.0);
  anim.SetConstantPosition (hosts.Get(1), 20.0, 55.0);
  anim.SetConstantPosition (hosts.Get(2), 25.0, 55.0);
  anim.SetConstantPosition (h0h3.Get(1), 30.0, 55.0);
  anim.SetConstantPosition (h3h4.Get(1), 25.0, 60.0);
  anim.SetConstantPosition (h5h6.Get(0), 15.0, 40.0);
  anim.SetConstantPosition (h6h7.Get(0), 10.0, 40.0);
  anim.SetConstantPosition (h6h7.Get(1), 5.0, 40.0);
  anim.SetConstantPosition (h8h9.Get(0), 35.0, 40.0);
  anim.SetConstantPosition (h8h9.Get(1), 40.0, 40.0);
  anim.SetConstantPosition (h10h11.Get(0), 35.0, 30.0);
  anim.SetConstantPosition (h10h11.Get(1), 40.0, 35.0);
  anim.SetConstantPosition (h10h12.Get(1), 40.0, 25.0);
  anim.SetConstantPosition (h10h13.Get(1), 45.0, 30.0);
  anim.SetConstantPosition (h14h15.Get(0), 35.0, 20.0);
  anim.SetConstantPosition (h14h15.Get(1), 40.0, 20.0);
  anim.SetConstantPosition (h16h17.Get(0), 35.0, 15.0);
  anim.SetConstantPosition (h16h17.Get(1), 40.0, 15.0);
  anim.SetConstantPosition (h18h19.Get(0), 15.0, 25.0);
  anim.SetConstantPosition (h18h19.Get(1), 10.0, 30.0);
  anim.SetConstantPosition (h19h20.Get(1), 5.0, 25.0);
  anim.SetConstantPosition (h21h22.Get(0), 20.0, 10.0);
  anim.SetConstantPosition (h21h22.Get(1), 10.0, 10.0);
  anim.SetConstantPosition (h22h23.Get(1), 15.0, 5.0);
  anim.SetConstantPosition (h24h25.Get(0), 25.0, 10.0);
  anim.SetConstantPosition (h24h25.Get(1), 25.0, 5.0);
  anim.SetConstantPosition (r0r1.Get(0), 25.0, 45.0);
  anim.SetConstantPosition (r0r1.Get(1), 20.0, 40.0);
  anim.SetConstantPosition (r0r2.Get(1), 30.0, 40.0);
  anim.SetConstantPosition (r0r3.Get(1), 25.0, 35.0);
  anim.SetConstantPosition (r3r4.Get(1), 25.0, 30.0);
  anim.SetConstantPosition (r5r6.Get(0), 25.0, 25.0);
  anim.SetConstantPosition (r5r6.Get(1), 25.0, 20.0);
  anim.SetConstantPosition (r6r7.Get(1), 15.0, 20.0);
  anim.SetConstantPosition (r6r8.Get(1), 20.0, 15.0);


  //Finaliza simulação após 200 segundos
  Simulator::Stop (Seconds (200.0));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}
