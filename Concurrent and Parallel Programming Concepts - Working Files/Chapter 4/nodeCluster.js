/** To run from the command-line (requires Node.js to be installed):

    % node nodeCluster.js
       
    This file is nodeCluster.js.

    Node.js is available at: https://nodejs.org
**/
var cluster = require('cluster');  /* cluster package */
var http = require('http');
var processors = require('os').cpus().length; /* get processor count */       /** line 1 **/
var port = 8888;                              /* await requests */

if (cluster.isMaster) { // master process                                     /** line 2 **/
    console.log('Listening on port %d', port);
    console.log('Processor count: %d', processors);

    // Fork a worker per processor. Should the worker exit, create another.
    for (var i = 0; i < processors; i++) { cluster.fork(); }                  /** line 3 **/
    cluster.on('exit', function(worker, code, signal) {	cluster.fork(); });   /** line 4 **/
} 
else {                  // worker processes
    http.createServer(function(req, res) {                                    /** line 5 **/
	res.writeHead(200);
	res.end("Echoing request URI: " + req.url + "\n");
    }).listen(port); // listen on the shared port
}

