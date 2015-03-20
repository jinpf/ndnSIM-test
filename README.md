# ndnSIM-test
Modify the code of ndnsim to realize network experiment.

For ndnSIM, official web: [http://ndnsim.net/2.0/index.html](http://ndnsim.net/2.0/index.html "http://ndnsim.net/2.0/index.html")

The file tree is organized as follows:

* `graphs/` Rscript for drawing picture from experiment data records. see: [http://ndnsim.net/2.0/metric.html ](http://ndnsim.net/2.0/metric.html "http://ndnsim.net/2.0/metric.html")
* `ns-3/` mainly modified source code from ns-3
	* `scratch/` custom experiment scenario. Developing example: [http://ndnsim.net/2.0/examples.html](http://ndnsim.net/2.0/examples.html "Example") [http://ndnsim.net/2.0/helpers.html](http://ndnsim.net/2.0/helpers.html "ndnHelper")
	* `src/` souce code
		* `ndnSIM/` ndnSIM related code
			* `apps/` Consumer Producer application
			* `NFD/`  NDN Forwarding Daemon src code


