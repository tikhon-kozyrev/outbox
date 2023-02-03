Simple SQLite-based design pattern Outbox. Used in application which produces a stream of messages and sends it to the collector service over unreliable network connection.

```mermaid
flowchart TB

msrc("Producer [thread1]") -->|sending message|ob(Outbox)
ob -->|reading message|mprx("Consumer [thread2]")
mprx-->| sending message|net("Network")
net-->|ACK|mprx
mprx-->|"deleting message"|ob
```
*Pic 1. Successful delivery*

```mermaid
flowchart TB

msrc("Producer [thread1]") -->|sending message|ob(Outbox)
ob -->|reading message|mprx("Consumer [thread2]")
mprx-->| sending message|net("Network")
net-...-x|no ACK|mprx

```
*Pic 1. Unsuccessful delivery*

