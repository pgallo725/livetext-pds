# LiveText
University assignment project for System and Device Programming at PoliTO

LiveText is a client-server, "real-time" text editor built in C++ using the Qt library.
We started studying other collaborative text editors such as Google Docs and Conclave and then we built up our own version.

Click below to download server and client exe:
- server (link al .exe)
- client (link al .exe)

# Index
1. [What is a real-time text editor?](#What-is-a-real-time-text-editor)
2. [What are the new problems?](#What-are-the-new-problems)
   - [Commutativity](#Commutativity)
   - [Idempotency](#Idempotency)
3. [How we solved the new issues](#How-we-solved-new-issues)
   - [Conflict-Free Replicated Data Type (CRDT)](#Conflict-Free-Replicated-Data-Type-CRDT)
   - [Globally Ordered Characters](#Globally-Ordered-Characters)
4. [Architecture](#Architecture)
   - [Server](#Server)
   - [Client](#Client)
5. [Security](#Security)
6. [Main functions code](#Main-functions-code)

# What is a real-time text editor?
In a simple text editor there is one user who edits the document, but what if we want more users editing the same document at the same time?
Is it possible?

First of all we must deliver a local copy of the document to all the on-line clients and then allow them to edit the document.
The challenge here is that we want a real-time editor, so users edit the document like in a simple local text editor, their changes must be applied immediately.
This leads to new problems to be handled.

We need to find a way to make users communicate to inform others clients about changes in the document.
We provide a server to solve this task, that acts as a relay for client's messages.

# What are the new problems?

LiveText is a distributed system, so we have new problems due to how networks work.
	
The main issue at first was making the documents converge on the same final state.
Users can use two operations, insertion and deletion, and both changes are applied immediately to the local side, but there is some delay between the users.
Sometimes operations could invert their order and the documents might not converge at all, this due to the un-commutativity of these two operations.

### Commutativity
From [Wikipedia](https://en.wikipedia.org/wiki/Commutative_property)
>In mathematics, a binary operation is commutative if changing the order of the operands does not change the result.

This means that the final state is not the same if operations are not applied in the same order, but the network cannot ensure that this could not happen.

The second problem we met was that the document could merge to the same final state, but something else went wrong:
if more than one user did a "delete" operation at the same time for the same character, this change was applied multiple times instead of once, this due to the un-idempotency of the delete operation.

### Idempotency
From [Wikipedia](https://en.wikipedia.org/wiki/Idempotence)
>Idempotence (UK: /ˌɪdɛmˈpoʊtəns/,[1] US: /ˌaɪdəm-/)[2] is the property of certain operations in mathematics and computer science whereby they can be applied multiple times without changing the result beyond the initial application.

This means that the delete operation must be applied only one time, even though there are multiples deletions.

The insert operation is not a problem here, because if a character is duplicated it's easy for the users to detect it and delete one of the two copies.
The same principle is not applicable for the delete operation, because the additional character that is deleted could not be easy to see to be replaced by the users.

# How we solved the new issues

Traditional text editors use an absolute value for the position of each character, and this cannot provide as the two main features that we want.
We reimplemented the basic structure of the text editor in which each character has additional informations that can provide what we needed.

This different approach is called "fractional position" or "CRDT".

### Conflict-Free Replicated Data Type (CRDT)
From [Wikipedia](https://en.wikipedia.org/wiki/Conflict-free_replicated_data_type)
>In distributed computing, a conflict-free replicated data type (CRDT) is a data structure which can be replicated across multiple computers in a network, where the replicas can be updated independently and concurrently without coordination between the replicas, and where it is always mathematically possible to resolve inconsistencies which might result.

CRDT is a different way to approach the problem, each character doesn't have just one value for the position, but CRDT provides a list of numerical values that identify the position of the character in an absolute way.
This new structure adds other properties that allowed characters to be "globally ordered".

### Globally Ordered Characters
We needed all the characters to be ordered and be consistent in all the documents and this means that when some user inserts a new character, its location must be unique and placed in the same position on every opened document.
Fractional indices allow to not shift all the positions of the other characters when a new character is being placed.

We could image this as a tree: the new characters are added to the existing ones, but their position doesn't change.

# Architecture

We chose the simplest distributed system: [client-server architecture](https://it.wikipedia.org/wiki/Sistema_client/server).
This might not be the best choice perfomance-wise, but we avoided a lot of new other problems that would have showed up due to peer-to-peer communication.

## Server
The server is a multiple-threads process.
The main thread is in charge of all the critical operations such as creation of a new account, login, update and all that concerns users. It also handles the creation and opening of documents.
Secondary threads are created when a document is opened, which handle all editing messages from and to clients, making sure that all changes are consistent.

## Client
//TODO

# Security

University assignment has no security requirements, but security issues are an interesting and important topic nowadays.
We are concerning in this topics so we want to provide a basic way to exchange data in a safty way.

First of all we set up SSL connections between the server and clients using a self-signed certificate.
In this way all data flow in the network encripted and no one can see what is inside messages despite someone could sniff packets.

If SSL connection is not enough, we provide a simple way to exchange critical data, as passwords, during Registration and Login operations. 
We send to clients a random "nonce" within they hashed the password and sand back to the server.

User DataBase is also a critical point under security view, so we save all the password hashed with a "salt", which is also store in the DataBase.

# Main functions code
//TODO
