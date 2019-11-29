# LiveText

Developed as an assignment project for the System and Device Programming course at PoliTO, LiveText is a client-server "real-time" collaborative text editor built in C++ using the Qt framework.

We began our project by studying other collaborative editors such as [Conclave](https://conclave-team.github.io/conclave-site/) and Google Docs and then we developed our own version in the form of a desktop application.

Here you can download the latest server and client packages:
- [Server](https://github.com/paolo257428/livetext-pds/releases/download/v1.0.0/textserver-v1.0.0-win-x64.zip)
- [Client](https://github.com/paolo257428/livetext-pds/releases/download/v1.0.0/textedit-v1.0.0-win-x64.zip)

# Index
1. [What is a real-time text editor?](#What-is-a-real-time-text-editor)
2. [Which are the new problems?](#Which-are-the-new-problems)
   - [Commutativity](#Commutativity)
   - [Idempotency](#Idempotency)
3. [How we solved the new issues](#How-we-solved-new-issues)
   - [Conflict-Free Replicated Data Type (CRDT)](#Conflict-Free-Replicated-Data-Type-CRDT)
   - [Globally Ordered Characters](#Globally-Ordered-Characters)
   - [Globally Ordered Formatting Operations](#Globally-Ordered-Formatting-Operations)
4. [Architecture](#Architecture)
   - [Server](#Server)
   - [Client](#Client)
5. [Security](#Security)
6. [Performance considerations](#Performance-considerations)

# What is a real-time text editor?

In a simple (or "classical") text editor there is only one user who edits the document, but what if we want to allow multiple users to edit the same document at the same time? Is it possible?

First of all we must deliver a local copy of the document to all the on-line clients and then allow them to edit it in real-time just like in any other text editor, but we also have to notify other clients of the applied changes as soon as possible: this leads to new problems that need to be handled.

**Note:** An example of a non-real-time collaborative editor is a version control system like *Git*.

In order to coordinate the editing of a shared document and allow the users to communicate with each other, we provide a central server that acts as a relay for clients' messages.

# Which are the new problems?

LiveText is a distributed system, and the presence of network communication creates a bunch of new problems for a text editor:
the main issue that we had to face was making sure that all copies of the document as seen by different users were coherent, or atleast always guarantee the convergence to the same final state.

Any text editor provides 2 basic operations, ***insertion*** and ***deletion***, which are both applied immediately and without issues to the local copy of the document. However, sending these operations through the network means that a non-negligible delay is introduced, causing some operations to be applied in a different order on other clients and possibly creating inconsistencies due to their un-commutativity.

### Commutativity
From [Wikipedia](https://en.wikipedia.org/wiki/Commutative_property)
>In mathematics, a binary operation is commutative if changing the order of the operands does not change the result.

For us, commutativity is a requirement because (due to network delay) we cannot ensure that operations are applied in the same order on each and every client editing a document.

The second problem was that, even when documents may converge to the same final state, something else could go wrong:
if multiple users "delete" the same character at the same time, this change would be applied multiple times instead of once resulting in more than one character being deleted, due to the un-idempotency of the operation.

### Idempotency
From [Wikipedia](https://en.wikipedia.org/wiki/Idempotence)
>Idempotence (UK: /ˌɪdɛmˈpoʊtəns/,[1] US: /ˌaɪdəm-/)[2] is the property of certain operations in mathematics and computer science whereby they can be applied multiple times without changing the result beyond the initial application.

In our case, this means that the delete operation must be applied only once (if it refers to the same character), even when there are multiples deletions sent from different clients. Insertion is a lesser issue here because duplicated characters are easy to detect and be removed by the users.

# How we solved the new issues

The real cause of the aforementioned issues is that traditional text editors identify each character with an absolute position index inside the document, and this does not fit well in a scenario where (due to network delay) the clients' views of the document may not be coherent for short periods of time.

We reimplemented the basic structure of the document by adding to each character extra information providing a global ordering and unique identification of symbols, in order to achieve the required properties.

### Conflict-Free Replicated Data Type (CRDT)
From [Wikipedia](https://en.wikipedia.org/wiki/Conflict-free_replicated_data_type)
>In distributed computing, a conflict-free replicated data type (CRDT) is a data structure which can be replicated across multiple computers in a network, where the replicas can be updated independently and concurrently without coordination between the replicas, and where it is always mathematically possible to resolve inconsistencies which might result.

CRDT is a different way to approach the problem: each character is not identified by its index (or position) in the document, but instead CRDT provides a structure containing a list of numerical values (**fractional position**) that identifies each symbol in an absolute way.

### Globally Ordered Characters
The fractional position serves as a unique symbol identifier (based on the client ID, avoiding conflicts when 2 users insert a character in the same spot at the same time) and is also an ordered type, therefore it provides information on where the symbol has to be inserted in the document and allows us to insert new values without having to logically shift the index of all following characters.

This technique resembles a binary tree: when a new character is added to the document, it gets assigned a new fractional position generated by selecting a middle value between the previous and the next symbols' fractional indices and increasing the depth of the list in case there is not enough room to do so (no floating point math is used).

### Globally Ordered Formatting Operations
The solution presented above completely solves the issues related to "insert" and "delete", but what about other operations that only change some *attributes* of existing symbols (such as text formatting) ?

The same network and ordering issues apply here, but solving this problem was much easier: the change is sent to the central server and then dispatched not only to all other clients in the same document, but also sent back to the source, allowing the server to enforce a "global ordering" of formatting operations (convergence to the same final state is guaranteed).

# Architecture

We chose the simplest distributed system: a [client-server architecture](https://it.wikipedia.org/wiki/Sistema_client/server).
This might not be the best choice perfomance-wise (especially when thinking about scaling to a large number of users), but  avoids many other problems that would've showed up in a peer-to-peer communication model.

The information exchange between clients and the server follows a custom bytestream application-level protocol based on Qt's serialization primitives.

## Server
The server application is a multi-threaded process.

The main thread is in charge of serving all user requests such as the creation of a new account, login and profile updates, while also handling the creation, deletion and opening of documents and updating the database accordingly.

All editors working on the a shared document are connected to the same *Workspace*, which is run on a separate thread and handles all the received editing operations, automatically saves the document on the server file system and dispatches messages to all connected clients (no synchronization needed due to clear roles separation between threads). 
All documents that are not being currently edited are stored on disk and unloaded from memory.

## Client
The LiveText client is a QtGUI-based desktop application.

It provides a landing page with basic account operations such login, registration and profile editing and grants access to the user's documents, which are opened inside the TextEditor.

The editor is based on Qt's powerful **QTextEdit** component (supporting *RichText*), on top of which we added a completely custom extra layer in charge of translating all editing operations between Qt's "traditional" text editor (that is, using indexes to identify characters in the document) and our CRDT representation, in both directions.
The editor presents a fully customized GUI with action menus, toolbars, context menus and supports keyboard shortcuts.

A secondary thread is always active and handles the network communication with the server, requiring little to no synchronization with the main application thread.

# Security

Our assignment had no security requirement, but we believe that in today's world every application should protect the users' privacy through the available security protocols for exchanging and storing data in a safe way.

Every single byte exchanged between a client and the server is encrypted using [SSL](https://it.wikipedia.org/wiki/Transport_Layer_Security) with the *ECDHE-RSA-AES256-GCM-SHA384* cipher suite.

**Note:** for practicality the server uses a self-signed certificate, which is fine for the scope of this project, but could easily be upgraded to a proper CA-signed certificate if needed.

In addition to that, the authentication process is implemented through a challenge protocol which removes the need of sending critical data (such as passwords) on the network during the Login phase. 
The password is only sent to the server once during the Registration phase, and then it is hashed with a random "salt" and stored in the server's SQLite Database along with all other user informations.

# Performance considerations

Our internal document representation stores the text in an **ordered array** of symbol objects (while paragraphs and lists make use of additional data structures) which grants us random access to any character and simplifies the translation between fractional positions and "traditional" indexes (required for QTextEdit interoperability) but suffers from a complexity of O(N) for the insertion or deletion of a character in the middle of the document.

While single-character operations (*e.g.* user typing on a keyboard) do not cause any perceivable delay, operations that involve a large number of symbols (*e.g.* copy-pasting or selecting and deleting thousands of characters) come with a big performance cost that may even freeze the editor for a few seconds.

An ordered tree structure could have avoided the cost of shifting all the elements at every editing operation, but it would've still presented a linear complexity (although with a lower overall cost due to no memory copy) for the translation between fractional positions and array indexes.
