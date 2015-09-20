# EmbeddedHTTPServer

## A lightwieght HTTP Server

This is a small and efficient HTTP Server implementation in an object oriented language (cpp), instead of the classic C implementation.
It is based on **libevent2** library, which is not object oriented.

## How to use it

Just include it in your project, and create a new class implementing the class HTTPServlet : this class offers you the ability to treat HTTP request, according to the HTTP method.
For each HTTP method is corresponding a HTTPServlet method, doGet, doPost, doPut, etc... Just like in Java.

```
virtual void doGet(HTTPRequest &request, HTTPResponse &response);
virtual void doPost(HTTPRequest &request, HTTPResponse &response);
virtual void doPut(HTTPRequest &request, HTTPResponse &response);
virtual void doHead(HTTPRequest &request, HTTPResponse &response);
virtual void doOptions(HTTPRequest &request, HTTPResponse &response);
virtual void doDelete(HTTPRequest &request, HTTPResponse &response);
virtual void doTrace(HTTPRequest &request, HTTPResponse &response);
virtual void doConnect(HTTPRequest &request, HTTPResponse &response);
virtual void doPatch(HTTPRequest &request, HTTPResponse &response);
```

To write back something to the client, just use the response as as buffer :
```
response << "<HTML>" << std::endl;
response << "<body>Hello World !!!</body>" << std::endl; 
response << "</HTML>" << std::endl;
```

Then, in your main loop, just create your HTTP Server and add your servlet :
```
HTTPServer *server = server = new HTTPServer("0.0.0.0", 8080);
HTTPServlet *fileServlet = new HTTPFileServlet("~/");
HTTPServlet *restServlet = new MyRestReceiverServlet("~/");
server->setDefaultServlet( fileServlet);
server->addServlet("/rest/modes.*", restServlet);
server->start();
    
// wait to receive a SIGINT signal
server->stop();

// free resources
delete server;
delete fileServlet;
delete restServlet;
```


##Next improvments

For now no session are managed, it will be added in the next version.