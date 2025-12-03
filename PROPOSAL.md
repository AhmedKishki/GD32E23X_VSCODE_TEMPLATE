# Supernova LIN

In Supernova LIN, Master and Slave are decoupled. The Master implements the Master Task and can optionally implement Slave Task(s). A Slave only implements Slave Task(s). A Master Task is a polling task, while a Slave Task is a response task. Decoupling ensures that the Master is not concerned with how the Slave handles its side of the communication.

## Divergences from the LIN protocol standard

- No checksum or CRC (Slave side)

- Full use of the 8-bit ID field (Master side)

### Rationale:

The LIN standard specifies only 63 usable addresses — too restrictive. By discarding the parity bits in the ID, the system gains 255 usable addresses. With this expanded ID space, checksum/CRC becomes less critical and is therefore optional.

## Master

Break → Sync → ID

## Slave

### Binding process

On each power up the **MASTER** polls every possible ID ( to detect new slaves ) and publishes its unique ID (for binding )
**Note: first publish ID for theft protection to unlock then master can detect by polling and a slave (leader) must reply to poll**
On each power up, any **slave** listens for the unique ID of the master. If it receives one, it checks if it already has stored an ID ( in this case, the slave IS already bound ) and refuses to work if they do not match.
If the slave has NOT yet stored an ID, it is factory new. In this case, it stores the ID in Flash.

### Leader-Follower definition the plug-and-play style

Factory default state is to be a leader device because that may imply some functions which are disabled if becoming a follower.
**Note: currently factory default is a third state -> Undefined, and undefined can become leader or follower - this should not affect how it works anyway**
Devices which see the unique ID of the master within a (short) timeout after their power-up are leader devices. 
Devices which do NOT see the master's unique ID within a (short) timeout after their own power-up declare themselves to be follower and store that information in flash. 
This missing of the ID is ensured by connecting devices which should become followers AFTER powering up the System (the master to be more precise)

At the next power-up, these devices **will** see the master's unique ID (because now they are connected) **but** as they have stored that they are folowers, they will be followers. 

### General startup process of bus slaves

Due to the two above defined special functions of the power-up action, the following overall sequence must be maintained:

**Check for bus** the lamp checks the presence of a bus by waiting for a SYNC signal.
  - IF a SYNC is seen, the lamp is on a bus. Proceed with "Check for master ID"
  - IF no SYNC is seen, this lamp is on a bike without a working bus. proceed with "Compare ID"

**Check for master ID**
  - IF a master ID is received, proceed with "Compare ID"
  - IF no master ID is received, this lamp is on a working bus, but was connected after the master ID was published
    * declare yourself to be follower, store that fact in the "follower flag" in flash, start running in bus mode.
      Binding to the master ID will happen at the next power up. Skip next checks.

**Compare ID**
- IF no ID is stored in memory so far, this is a new lamp. Store the ID in flash and proceed with "check for follower flag"
- IF a ID is stored in memory, compare it with the received ID. This also applies if NO id was received, 
  in which case this is a stolen lamp on a bke without bus.
  * IF IDs match: Continue with "check for follower flag"
  * IF IDs do not match: Stolen lamp, refuse to run. Skip next check.

**check for follower flag**
 Check if we have the "follower flag" set in Memory.
   - IF this is the case, switch to follower mode and run in bus mode.
   - IF this is not the case, run in bus mode as leader.

### Regular Operation

For a slave with a matching ID, the reply consists of 4 data bytes:

byte0 → byte1 → byte2 → byte3

This sequence follows little-endian format for a 32-bit value:

byte3, byte2, byte1, byte0

## Slaves

A slave can operate in one of three modes: Leader, Follower, or Independent. Only one slave is assigned as the Leader for a given address, which prevents collisions when multiple slaves with the same address exist in the network.

The Leader is responsible for publishing when polled and may also take decisions. A Follower does not publish on its own; instead, it mirrors the Leader’s state. The Follower assumes that the node publishing on its address is the Leader. Therefore, it only needs to subscribe to its own ID (i.e., the Leader’s messages) and must ignore all other traffic.

An Independent node publishes whenever it has data available. This mode must be handled carefully, as it can lead to collisions if multiple Independents publish at the same time. The role of the Independent is to ensure that a control device can always inject commands into the network, even in cases where duplicate nodes are present.

A node can be assigned as a Leader or a Follower, but an Independent is hard coded to be as such. 

### HB Button (0x01)

The button functions as an Independent event producer. It publishes an event whenever one is generated, without persistence. For example, when the button is pressed, a single event is published to indicate the press. Each button in the network can publish its own events independently.

#### byte0: 
  - 0x01 Single Press
  - 0x02 Double Press
  - 0xFF Long Press
#### byte1:
  - Time of long Press (first 8 bits)
#### byte2:
  - Time of long press (second 8 bits)
#### byte3:
  - Empty (0xFF)

### Front Light (0xF0)

The Beam can operate as a Leader or Follower

#### byte0: 
  - 0xA0 LB
  - 0xA1 HB
  - 0xAF OFF

#### byte1:
  - Duty Cycle %age (0-100)
#### byte2:
  - Duty Cycle %age
#### byte3:
  - Duty Cycle %age


### Tail Light (0xF1)

#### byte0:
  - OFF       0xAF
  - Tail      0xA0
  - Brake     0xA1
  - Emergency 0xEE

#### byte1:
  - 0x00

#### byte2:
  - 0x00

#### byte3:
  - 0x00
  
### Light Sensor 0xE0

#### byte0: 
  - Dark
  - Bright

#### byte1:
  - 

#### byte2:

#### byte3:

