# Projet Réseaux 2022
### Laurent Raphaël 
### Wang Jinhua
-------------------------------
## 1. Configuration Réseau
## 2. L'interface virtuelle TUN
### 2.2 Configuration de l'interface
1. Configurer l'interface tun0 avec l'adresse fc00:<zero-width space>1234:ffff::1, mettre un masque adéquat. Ecrire un script configure-tun.sh reprenant vos commandes de configuration.
```
#!/bin/bash
ip link set tun0 up
ip addr add fc00:1234:ffff::1/64 dev tun0
```
2. Routage : Suite à la disparition tragique de VM2-6, faut-il modifier les informations de routage sur VM1 ? ou sur VM1-6 ?
Sur VM1-6. Parce que VM3-6 est liée par VM2-6, la suppression de vm2 aura pour conséquence que vm1-6 et vm3-6 ne pourront pas se connecter.
   
3. Faire un ping6 fc00:<zero-width space>1234:ffff::1. Donner la capture sur tun0 (avec wireshark). Que constatez-vous ?
```
1	0.000000000	fe80::d07:3d30:856d:f23	ff02::2	ICMPv6	48	Router Solicitation
Frame 1: 48 bytes on wire (384 bits), 48 bytes captured (384 bits) on interface tun0, id 0
Raw packet data
Internet Protocol Version 6, Src: fe80::d07:3d30:856d:f23, Dst: ff02::2
Internet Control Message Protocol v6

2	29.952897617	fe80::d07:3d30:856d:f23	ff02::2	ICMPv6	48	Router Solicitation
Frame 2: 48 bytes on wire (384 bits), 48 bytes captured (384 bits) on interface tun0, id 0
Raw packet data
Internet Protocol Version 6, Src: fe80::d07:3d30:856d:f23, Dst: ff02::2
Internet Control Message Protocol v6
```
Chaque intervalle icmp est long, mais la connexion est établie.
4. Faire ping6 fc00:<zero-width space>1234:ffff::10. Que constatez-vous?
```
1	0.000000000	fc00:1234:ffff::1	fc00:1234:ffff::10	ICMPv6	104	Echo (ping) request id=0xdfd3, seq=8, hop limit=64 (no response found!)
Frame 1: 104 bytes on wire (832 bits), 104 bytes captured (832 bits) on interface tun0, id 0
Raw packet data
Internet Protocol Version 6, Src: fc00:1234:ffff::1, Dst: fc00:1234:ffff::10
Internet Control Message Protocol v6

2	1.026806293	fc00:1234:ffff::1	fc00:1234:ffff::10	ICMPv6	104	Echo (ping) request id=0xdfd3, seq=9, hop limit=64 (no response found!)
Frame 2: 104 bytes on wire (832 bits), 104 bytes captured (832 bits) on interface tun0, id 0
Raw packet data
Internet Protocol Version 6, Src: fc00:1234:ffff::1, Dst: fc00:1234:ffff::10
Internet Control Message Protocol v6

Il n'y a que des requêtes et pas de réponses.
```
5. Expliquez.
Comme cette adresse n'existe pas, il n'y a qu'une demande et pas de réponse.

### 2.3 Récupération des paquets
2. Tester si la fonction implémentée permet de récupérer la sortie est bien lue.

On peut bien voir ci dessous que cela fonctionne, les données lues sont bien ce qui écrit sur le terminal. 
```
00000000  31 3a 20 6c 6f 3a 20 3c  4c 4f 4f 50 42 41 43 4b  |1: lo: <LOOPBACK|
00000010  2c 55 50 2c 4c 4f 57 45  52 5f 55 50 3e 20 6d 74  |,UP,LOWER_UP> mt|
00000020  75 20 36 35 35 33 36 20  71 64 69 73 63 20 6e 6f  |u 65536 qdisc no|
00000030  71 75 65 75 65 20 73 74  61 74 65 20 55 4e 4b 4e  |queue state UNKN|
00000040  4f 57 4e 20 67 72 6f 75  70 20 64 65 66 61 75 6c  |OWN group defaul|
00000050  74 20 71 6c 65 6e 20 31  30 30 30 0a 20 20 20 20  |t qlen 1000.    |
00000060  6c 69 6e 6b 2f 6c 6f 6f  70 62 61 63 6b 20 30 30  |link/loopback 00|
00000070  3a 30 30 3a 30 30 3a 30  30 3a 30 30 3a 30 30 20  |:00:00:00:00:00 |
00000080  62 72 64 20 30 30 3a 30  30 3a 30 30 3a 30 30 3a  |brd 00:00:00:00:|
00000090  30 30 3a 30 30 0a 20 20  20 20 69 6e 65 74 20 31  |00:00.    inet 1|
000000a0  32 37 2e 30 2e 30 2e 31  2f 38 20 73 63 6f 70 65  |27.0.0.1/8 scope|
000000b0  20 68 6f 73 74 20 6c 6f  0a 20 20 20 20 20 20 20  | host lo.       |
000000c0  76 61 6c 69 64 5f 6c 66  74 20 66 6f 72 65 76 65  |valid_lft foreve|
000000d0  72 20 70 72 65 66 65 72  72 65 64 5f 6c 66 74 20  |r preferred_lft |
000000e0  66 6f 72 65 76 65 72 0a  20 20 20 20 69 6e 65 74  |forever.    inet|
000000f0  36 20 3a 3a 31 2f 31 32  38 20 73 63 6f 70 65 20  |6 ::1/128 scope |
00000100  68 6f 73 74 20 0a 20 20  20 20 20 20 20 76 61 6c  |host .       val|
00000110  69 64 5f 6c 66 74 20 66  6f 72 65 76 65 72 20 70  |id_lft forever p|
00000120  72 65 66 65 72 72 65 64  5f 6c 66 74 20 66 6f 72  |referred_lft for|
00000130  65 76 65 72 0a 32 3a 20  65 74 68 30 3a 20 3c 42  |ever.2: eth0: <B|
00000140  52 4f 41 44 43 41 53 54  2c 4d 55 4c 54 49 43 41  |ROADCAST,MULTICA|
00000150  53 54 2c 55 50 2c 4c 4f  57 45 52 5f 55 50 3e 20  |ST,UP,LOWER_UP> |
00000160  6d 74 75 20 31 35 30 30  20 71 64 69 73 63 20 70  |mtu 1500 qdisc p|
00000170  66 69 66 6f 5f 66 61 73  74 20 73 74 61 74 65 20  |fifo_fast state |
00000180  55 50 20 67 72 6f 75 70  20 64 65 66 61 75 6c 74  |UP group default|
00000190  20 71 6c 65 6e 20 31 30  30 30 0a 20 20 20 20 6c  | qlen 1000.    l|
000001a0  69 6e 6b 2f 65 74 68 65  72 20 30 38 3a 30 30 3a  |ink/ether 08:00:|
000001b0  32 37 3a 38 64 3a 63 30  3a 34 64 20 62 72 64 20  |27:8d:c0:4d brd |
000001c0  66 66 3a 66 66 3a 66 66  3a 66 66 3a 66 66 3a 66  |ff:ff:ff:ff:ff:f|
000001d0  66 0a 20 20 20 20 61 6c  74 6e 61 6d 65 20 65 6e  |f.    altname en|
000001e0  70 30 73 33 0a 20 20 20  20 69 6e 65 74 20 31 30  |p0s3.    inet 10|
000001f0  2e 30 2e 32 2e 31 35 2f  32 34 20 62 72 64 20 31  |.0.2.15/24 brd 1|
00000200  30 2e 30 2e 32 2e 32 35  35 20 73 63 6f 70 65 20  |0.0.2.255 scope |
00000210  67 6c 6f 62 61 6c 20 64  79 6e 61 6d 69 63 20 65  |global dynamic e|
00000220  74 68 30 0a 20 20 20 20  20 20 20 76 61 6c 69 64  |th0.       valid|
00000230  5f 6c 66 74 20 36 39 38  34 32 73 65 63 20 70 72  |_lft 69842sec pr|
00000240  65 66 65 72 72 65 64 5f  6c 66 74 20 36 39 38 34  |eferred_lft 6984|
00000250  32 73 65 63 0a 20 20 20  20 69 6e 65 74 36 20 66  |2sec.    inet6 f|
00000260  65 38 30 3a 3a 61 30 30  3a 32 37 66 66 3a 66 65  |e80::a00:27ff:fe|
00000270  38 64 3a 63 30 34 64 2f  36 34 20 73 63 6f 70 65  |8d:c04d/64 scope|
00000280  20 6c 69 6e 6b 20 0a 20  20 20 20 20 20 20 76 61  | link .       va|
00000290  6c 69 64 5f 6c 66 74 20  66 6f 72 65 76 65 72 20  |lid_lft forever |
000002a0  70 72 65 66 65 72 72 65  64 5f 6c 66 74 20 66 6f  |preferred_lft fo|
000002b0  72 65 76 65 72 0a 33 3a  20 65 74 68 31 3a 20 3c  |rever.3: eth1: <|
000002c0  42 52 4f 41 44 43 41 53  54 2c 4d 55 4c 54 49 43  |BROADCAST,MULTIC|
000002d0  41 53 54 2c 55 50 2c 4c  4f 57 45 52 5f 55 50 3e  |AST,UP,LOWER_UP>|
000002e0  20 6d 74 75 20 31 35 30  30 20 71 64 69 73 63 20  | mtu 1500 qdisc |
000002f0  70 66 69 66 6f 5f 66 61  73 74 20 73 74 61 74 65  |pfifo_fast state|
00000300  20 55 50 20 67 72 6f 75  70 20 64 65 66 61 75 6c  | UP group defaul|
00000310  74 20 71 6c 65 6e 20 31  30 30 30 0a 20 20 20 20  |t qlen 1000.    |
00000320  6c 69 6e 6b 2f 65 74 68  65 72 20 30 38 3a 30 30  |link/ether 08:00|
00000330  3a 32 37 3a 66 63 3a 61  31 3a 66 38 20 62 72 64  |:27:fc:a1:f8 brd|
00000340  20 66 66 3a 66 66 3a 66  66 3a 66 66 3a 66 66 3a  | ff:ff:ff:ff:ff:|
00000350  66 66 0a 20 20 20 20 61  6c 74 6e 61 6d 65 20 65  |ff.    altname e|
00000360  6e 70 30 73 38 0a 20 20  20 20 69 6e 65 74 20 31  |np0s8.    inet 1|
00000370  37 32 2e 31 36 2e 32 2e  31 33 31 2f 32 38 20 62  |72.16.2.131/28 b|
00000380  72 64 20 31 37 32 2e 31  36 2e 32 2e 31 34 33 20  |rd 172.16.2.143 |
00000390  73 63 6f 70 65 20 67 6c  6f 62 61 6c 20 6e 6f 70  |scope global nop|
000003a0  72 65 66 69 78 72 6f 75  74 65 20 65 74 68 31 0a  |refixroute eth1.|
000003b0  20 20 20 20 20 20 20 76  61 6c 69 64 5f 6c 66 74  |       valid_lft|
000003c0  20 66 6f 72 65 76 65 72  20 70 72 65 66 65 72 72  | forever preferr|
000003d0  65 64 5f 6c 66 74 20 66  6f 72 65 76 65 72 0a 20  |ed_lft forever. |
000003e0  20 20 20 69 6e 65 74 36  20 66 65 38 30 3a 3a 34  |   inet6 fe80::4|
000003f0  66 61 39 3a 37 63 66 35  3a 64 37 35 33 3a 36 33  |fa9:7cf5:d753:63|
00000400  61 63 2f 36 34 20 73 63  6f 70 65 20 6c 69 6e 6b  |ac/64 scope link|
00000410  20 6e 6f 70 72 65 66 69  78 72 6f 75 74 65 20 0a  | noprefixroute .|
00000420  20 20 20 20 20 20 20 76  61 6c 69 64 5f 6c 66 74  |       valid_lft|
00000430  20 66 6f 72 65 76 65 72  20 70 72 65 66 65 72 72  | forever preferr|
00000440  65 64 5f 6c 66 74 20 66  6f 72 65 76 65 72 0a 34  |ed_lft forever.4|
00000450  3a 20 65 74 68 32 3a 20  3c 42 52 4f 41 44 43 41  |: eth2: <BROADCA|
00000460  53 54 2c 4d 55 4c 54 49  43 41 53 54 2c 55 50 2c  |ST,MULTICAST,UP,|
00000470  4c 4f 57 45 52 5f 55 50  3e 20 6d 74 75 20 31 35  |LOWER_UP> mtu 15|
00000480  30 30 20 71 64 69 73 63  20 70 66 69 66 6f 5f 66  |00 qdisc pfifo_f|
00000490  61 73 74 20 73 74 61 74  65 20 55 50 20 67 72 6f  |ast state UP gro|
000004a0  75 70 20 64 65 66 61 75  6c 74 20 71 6c 65 6e 20  |up default qlen |
000004b0  31 30 30 30 0a 20 20 20  20 6c 69 6e 6b 2f 65 74  |1000.    link/et|
000004c0  68 65 72 20 30 38 3a 30  30 3a 32 37 3a 30 61 3a  |her 08:00:27:0a:|
000004d0  35 36 3a 32 31 20 62 72  64 20 66 66 3a 66 66 3a  |56:21 brd ff:ff:|
000004e0  66 66 3a 66 66 3a 66 66  3a 66 66 0a 20 20 20 20  |ff:ff:ff:ff.    |
000004f0  61 6c 74 6e 61 6d 65 20  65 6e 70 30 73 39 0a 20  |altname enp0s9. |
00000500  20 20 20 69 6e 65 74 36  20 66 63 30 30 3a 31 32  |   inet6 fc00:12|
00000510  33 34 3a 33 3a 3a 31 2f  36 34 20 73 63 6f 70 65  |34:3::1/64 scope|
00000520  20 67 6c 6f 62 61 6c 20  6e 6f 70 72 65 66 69 78  | global noprefix|
00000530  72 6f 75 74 65 20 0a 20  20 20 20 20 20 20 76 61  |route .       va|
00000540  6c 69 64 5f 6c 66 74 20  66 6f 72 65 76 65 72 20  |lid_lft forever |
00000550  70 72 65 66 65 72 72 65  64 5f 6c 66 74 20 66 6f  |preferred_lft fo|
00000560  72 65 76 65 72 0a 20 20  20 20 69 6e 65 74 36 20  |rever.    inet6 |
00000570  66 65 38 30 3a 3a 61 64  61 63 3a 63 63 38 65 3a  |fe80::adac:cc8e:|
00000580  64 65 34 61 3a 37 35 65  39 2f 36 34 20 73 63 6f  |de4a:75e9/64 sco|
00000590  70 65 20 6c 69 6e 6b 20  6e 6f 70 72 65 66 69 78  |pe link noprefix|
000005a0  72 6f 75 74 65 20 0a 20  20 20 20 20 20 20 76 61  |route .       va|
000005b0  6c 69 64 5f 6c 66 74 20  66 6f 72 65 76 65 72 20  |lid_lft forever |
000005c0  70 72 65 66 65 72 72 65  64 5f 6c 66 74 20 66 6f  |preferred_lft fo|
000005d0  72 65 76 65 72 0a 39 3a  20 74 75 6e 30 3a 20 3c  |rever.9: tun0: <|
000005e0  50 4f 49 4e 54 4f 50 4f  49 4e 54 2c 4d 55 4c 54  |POINTOPOINT,MULT|
000005f0  49 43 41 53 54 2c 4e 4f  41 52 50 2c 55 50 2c 4c  |ICAST,NOARP,UP,L|
00000600  4f 57 45 52 5f 55 50 3e  20 6d 74 75 20 31 35 30  |OWER_UP> mtu 150|
00000610  30 20 71 64 69 73 63 20  70 66 69 66 6f 5f 66 61  |0 qdisc pfifo_fa|
00000620  73 74 20 73 74 61 74 65  20 55 4e 4b 4e 4f 57 4e  |st state UNKNOWN|
00000630  20 67 72 6f 75 70 20 64  65 66 61 75 6c 74 20 71  | group default q|
00000640  6c 65 6e 20 35 30 30 0a  20 20 20 20 6c 69 6e 6b  |len 500.    link|
00000650  2f 6e 6f 6e 65 20 0a 20  20 20 20 69 6e 65 74 36  |/none .    inet6|
00000660  20 66 63 30 30 3a 31 32  33 34 3a 66 66 66 66 3a  | fc00:1234:ffff:|
00000670  3a 31 2f 36 34 20 73 63  6f 70 65 20 67 6c 6f 62  |:1/64 scope glob|
00000680  61 6c 20 0a 20 20 20 20  20 20 20 76 61 6c 69 64  |al .       valid|
00000690  5f 6c 66 74 20 66 6f 72  65 76 65 72 20 70 72 65  |_lft forever pre|
000006a0  66 65 72 72 65 64 5f 6c  66 74 20 66 6f 72 65 76  |ferred_lft forev|
000006b0  65 72 0a 20 20 20 20 69  6e 65 74 36 20 66 65 38  |er.    inet6 fe8|
000006c0  30 3a 3a 61 62 31 33 3a  36 37 36 3a 63 32 63 35  |0::ab13:676:c2c5|
000006d0  3a 35 30 30 66 2f 36 34  20 73 63 6f 70 65 20 6c  |:500f/64 scope l|
000006e0  69 6e 6b 20 73 74 61 62  6c 65 2d 70 72 69 76 61  |ink stable-priva|
000006f0  63 79 20 0a 20 20 20 20  20 20 20 76 61 6c 69 64  |cy .       valid|
00000700  5f 6c 66 74 20 66 6f 72  65 76 65 72 20 70 72 65  |_lft forever pre|
00000710  66 65 72 72 65 64 5f 6c  66 74 20 66 6f 72 65 76  |ferred_lft forev|


00000720  65 72 0a 43 72 c3 a9 61  74 69 6f 6e 20 64 65 20  |er.Cr..ation de |
00000730  74 75 6e 30 0a 46 61 69  72 65 20 6c 61 20 63 6f  |tun0.Faire la co|
00000740  6e 66 69 67 75 72 61 74  69 6f 6e 20 64 65 20 74  |nfiguration de t|
00000750  75 6e 30 2e 2e 2e 0a 41  70 70 75 79 65 7a 20 73  |un0....Appuyez s|
00000760  75 72 20 75 6e 65 20 74  6f 75 63 68 65 20 70 6f  |ur une touche po|
00000770  75 72 20 63 6f 6e 74 69  6e 75 65 72 0a 49 6e 74  |ur continuer.Int|
00000780  65 72 66 61 63 65 20 74  75 6e 30 20 43 6f 6e 66  |erface tun0 Conf|
00000790  69 67 75 72 c3 a9 65 3a  0a 41 70 70 75 79 65 7a  |igur..e:.Appuyez|
000007a0  20 73 75 72 20 75 6e 65  20 74 6f 75 63 68 65 20  | sur une touche |
000007b0  70 6f 75 72 20 74 65 72  6d 69 6e 65 72 0a        |pour terminer.|
000007be

```

# Problème concerant les VMs et donc l'avancée du TP. 

L'un des deux membres à eu des soucis concernant la VDI durant la semaine (au début de la semaine, lenteur sur la session et le 17 et 18 novembre impossibilité de créer / lancer une VM).
3. 
## 3. Un tunnel simple pour IPv6
## 4. Validation Fonctionnelle
## 5. Améliorations


## Lancer le reseaux
$./up.sh
#VM: sudo -s
#VM: ansible-playbook -c local -v /vagrant/config.yml
$./halt.sh
