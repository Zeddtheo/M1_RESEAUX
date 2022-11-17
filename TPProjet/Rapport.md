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
Il n'y a que des requests et pas de réponses
```
5. Expliquez.
Comme cette adresse n'existe pas, il n'y a qu'une demande et pas de réponse.

### 2.3 Récupération des paquets
## 3. Un tunnel simple pour IPv6
## 4. Validation Fonctionnelle
## 5. Améliorations
