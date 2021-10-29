# simple-http-proxy-socket-programming
Networking Assignment, simple http proxy that blocks website if words are contained in url.

made to run on linux

COMPILE

$g++ main.cpp -o main

RUN using port 1235

YOU HAVE THREE CHOICES, YOU CAN EITHER RUN ONLY USING PRESET ARGS (ENDING WITH CONTINUE TO NOT USE TELNET)

$./main SpongeBob Floppy CONTINUE

YOU CAN SET ARGS AFTER RUNNING USING TELNET

$./main

THEN RUN

$telnet <ip> 1235

$BLOCK Floppy

$BLOCK SpongeBob

$CONTINUE

THE CONTINUE COMMAND ALLOWS THE PROXY TO START WORKING

YOU MAY ALSO REMOVE BLOCKED WORKS USING

$REMOVE Floppy

BUT NOT AFTER CONTINUE

YOU CAN ALSO USE BOTH ARGS AND TELNET BY NOT PUTTING CONTINUE AT THE END OF ARGS

TESTING WAS DONE BOTH AT HOME AND AT THE UNIVERSITY, CODE WAS RUN EXCLUSIVELY ON linux.cpsc.ucalgary.ca

I HAVE TESTED FOR A LOT OF BUGS, MOSTLY USING PROVIDED WEBSITES
  
  
#Assignment was as follows
Assignment 1: Web Censorship Proxy (40 marks)
Due: Friday, October 1, 2021 (11:59pm)
Learning Objectives
The purpose of this assignment is to learn about the HyperText Transfer Protocol (HTTP) used by the World Wide Web. In particular, you will design and implement a Web proxy using HTTP to demonstrate your understanding of this application-layer protocol. Along the way, you will also learn a lot about socket programming, TCP/IP, network debugging, and more.

Preamble
As you know, there is a LOT of undesirable content on the World Wide Web, with SpongeBob, Justin Bieber, and Britney Spears being just a few of the most glaring examples of this. In this assignment, you are going to develop a Web censorship proxy that blocks access to certain content, based on keywords that appear in the URL that is being accessed. To keep the assignment simple, we will restrict ourselves only to HTTP (not HTTPS).

Background
A Web proxy is a piece of software that functions as an intermediary between a Web client (browser) and a Web server. The Web proxy intercepts Web requests from clients and determines whether they should be transmitted to a Web server or not. If the request is blocked, the proxy informs the client directly. If the request is forwarded to the Web server, then any response that the proxy receives from the Web server is forwarded back to the client. From the server's point of view, the proxy is the client, since that is where the request comes from. Similarly, from the client's point of view, the proxy is the server, since that is where the response comes from. A Web proxy thus provides a single point of control to regulate Web access between clients and servers. A lot of Calgary schools use Web proxies to limit the types of Web sites that students are allowed to access. Net Nanny and Barracuda are examples of commercially available Web proxies.

Technical Requirements
In this assignment, you will implement your very own Web censorship proxy, in either C or C++. The goals of the assignment are to build a properly functioning Web proxy for simple Web pages, and then use your proxy to block undesirable Web content from being delivered to the browser.

There are three main pieces of functionality needed in your proxy. The first is the ability to handle HTTP requests and responses, while still forwarding them between client and server. This is called a transparent proxy. The second is the ability to parse (and possibly modify) HTTP requests, so that you can extract URL information, decide whether requests should be blocked or not, and block them in some reasonable way. This is called a censorship proxy. The third piece is to provide a way to update the list of blocked keywords for your censorship proxy while it is still running. This is called a dynamically configurable Web censorship proxy.

The most important HTTP command for your Web proxy to handle is the "GET" request, which specifies the URL for an object to be retrieved. In the basic operation of your proxy, it should be able to parse, understand, and forward to the Web server a (possibly modified) version of the client HTTP request. Similarly, the proxy should be able to parse, understand, and return to the client a (possibly modified) version of the HTTP response that the Web server provided to the proxy. Please give some careful thought to how your proxy handles commonly occurring HTTP response codes, such as 200 (OK), 206 (Partial Content), 301 (Moved Permanently), 302 (Found), 304 (Not Modified), 403 (Forbidden), and 404 (Not Found).

You will need at least one TCP socket (i.e., SOCK_STREAM) for client-proxy communication, and at least one additional TCP socket for each Web server that your proxy talks to during proxy-server communication. If you want your proxy to support multiple concurrent HTTP transactions, you may need to fork child processes or create threads for request handling. Each child process or thread will use its own socket instances for its communications with the client and with the server.

When implementing your proxy, feel free to compile and run your Web proxy on any suitable department machine, or even your home machine or laptop, but please be aware that you will ultimately have to demo your proxy to your TA on campus at some point. You should try to access your proxy from your favourite Web browser (e.g., Edge, Firefox, Chrome, Safari), and computer (either on campus or at home). To test the proxy, you will have to configure your Web browser to use your specific Web proxy (e.g., look for menu selections like Tools, Internet Options, Proxies, Advanced, LAN Settings).

As you design and build your Web proxy, give careful consideration to how you will debug and test it. For example, you may want to print out information about requests and responses received, processed, forwarded, redirected, or altered. Once you become confident with the basic operation of your Web proxy, you can toggle off the verbose debugging output. If you are testing on your home network, you can also use tools like WireShark to collect network packet traces. By studying the HTTP messages and TCP/IP packets going to and from your proxy, you might be able to figure out what is working, what isn't working, and why.

When you are finished, please submit your solution in electronic form to your TA via D2L. Your submission should include the source code for your Web proxy, a brief user manual describing how to compile and use your proxy, and a description of the testing done with your proxy. Please remember that assignments are to be done individually, and submitted to your assigned TA on time. You should also plan to give a brief demo of your proxy to your TA during a tutorial time slot just after the assignment deadline.

Testing
During your demo, your proxy will be tested on the following test cases:

a very simple Web page
a Web page with a link to a photo of Floppy
a Web page with a link to a photo of my curling team
a Web page talking about Floppy
a Web page talking about curling
a Web page talking about SpongeBob
Once you have these cases working, you can try your proxy on other pages, such as the Wikipedia page for Floppy Disks, to see what happens. Good luck, and have fun!

Grading Rubric
The grading scheme for the assignment is as follows:

10 marks for the design and implementation of a functional Web proxy that can handle simple HTTP GET interactions between client and server, using either HTTP/1.0 or HTTP/1.1. This basic proxy should be able to deliver Web pages in unaltered form, and be able to handle HTTP redirection when it occurs. Your implementation should include proper use of TCP/IP socket programming in C or C++, and reasonably commented code.
8 marks for that part of your Web proxy that can parse HTTP requests and responses, identify what content is being requested, and make proper decisions about content blocking.
8 marks for that part of your Web proxy that can dynamically update the list of keywords to be blocked, including both adding and deleting keywords.
4 marks for a clear and concise user manual (at most 1 page) that describes how to compile, configure, and use your Web proxy. Make sure to indicate the required features and optional features (if any) that the proxy supports. Make sure to clarify where and how the testing was done (e.g., home, university, office), what works, and what does not. Be honest!
10 marks for a suitable demonstration of your proxy to your TA in your tutorial section, or to your professor at a mutually convenient time. A successful demo will include marks for the test cases given above, as well as clear answers to questions asked during your code walk-through.
Bonus (optional)
Up to 4 bonus marks will be given for a Web censorship proxy that can also block undesirable content based on keywords seen in the body of an HTTP response, rather than just based on the URL requested. Make sure to show this bonus feature to your TA during the demo.

Tips
This is a very challenging assignment, so please get started early. You will likely need 7-14 days of thinking/coding/debugging time to get it fully working.
If you have never done socket programming in C/C++ before, you should make sure to get to your CPSC 441 tutorials on this topic. Don't miss them!
Make sure that you have completed and understood Assignment 0, which provides a basic introduction to client-server socket programming.
If you don't speak HTTP already, make sure to get to your CPSC 441 tutorial on this topic. For example, the "Content-Length" header in an HTTP response will tell you how big on object is (in bytes), and the "Content-Type" header is useful for determining the object type (Hint: you don't want to print binary JPG images onto your screen in debugging messages!)
Focus on the basic HTTP proxy functionality first, by simply forwarding everything that you receive from the client directly to the server, and everything you receive from the server directly back to the client. Then add more functionality, such as request parsing, URL rewriting, or HTTP redirection.
Your proxy will need one socket for talking to the client, and another socket for talking to the server. Make sure to keep track of which one is which. This is very important to understand!
Your proxy will likely need to dynamically create a socket for every new server that it talks to. Most of the examples above involve only one server, which is easier. But you will likely need to generalize this to multiple servers. If so, make sure to manage these sockets properly.
Start with very simple Web pages, such as those indicated above. Once you have these working, then you can try more complicated Web pages with lots of embedded objects, possibly from multiple servers.
Be aware that most Web browsers like to cache content locally (i.e., browser cache), rather than retrieving it from the server each time it is requested. You will want to force a reload from the server each time so that you don't get fooled during your testing.
When testing your censorship functionality, start with a single keyword that is statically configured (i.e., hardcoded) in your proxy. Once this is working as intended, you can add the dynamic configuration functionality to update the keyword lists (i.e., adding or removing keywords). There are several ways to make your proxy dynamically reconfigurable, but some might be harder than others. Don't fret if you can't get it fully working; at least you are learning a LOT in the process!
You may find that network firewalls block certain ports, which may make configuration and use of your proxy tricky. For example, it might be easier to do all of your testing using machines within the CPSC network, rather than external ones. A good Wireshark trace can help show you what is actually happening on the network.
Try to avoid servers that automatically redirect HTTP to HTTPS, since TLS handshakes and encrypted content are well beyond the intended scope of the assignment. Let's keep things simple with HTTP only.
The assignment is structured into incremental pieces that are each worth partial marks. As you get the different pieces of functionality working, make sure to save working versions of your code somewhere. Adding new functionality sometimes breaks things in a bad way, and you don't want this happening just prior to the assignment deadline. Expressed another way, it is probably better to have a fully-working partial solution for your demo, rather than a partially-working full solution. If your code does not even compile, there is not much that you can demo, so please be careful if/when making last-minute code changes.
Here is a generic debugging checklist for socket programming that you might find helpful. Good luck!
