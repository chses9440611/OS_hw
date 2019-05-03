# Project #3 Multitasking
0310020 黃百川

## Code Explain
* Environment: Ubuntu 16.04 LTS
* Python version: 3.5.2
* Code:
	
	1. Import necessary package and library

	```python
		import random
		import string
		import hashlib
		from threading import Thread
		from socketserver import BaseRequestHandler, TCPServer
	```
	2. server_work to calculate PoW with corresponding difficulity

	```python
		def server_work(message, pow_diff=3):
    		"""genetrate POW string"""
    		pow_check = pow_diff * '0' # decide the # of 0
    		seed = id_generator() # create the seed
    		hash_value = create_hashvalue(seed+message) # create the hash value
    		while hash_value[:pow_diff] > pow_check: # while create failure, redo
        		seed = id_generator()
        		hash_value = create_hashvalue(seed+message)

	    	return message + ',' + seed + ',' + hash_value + '\n'
	```
	3. id_generator produce the random seed and return 5 characters 

	```python
		def id_generator(size=5, chars=string.ascii_letters + string.digits):
		    """Generating seed"""
		    value = ''
		    for _ in range(size):
		        value += random.choice(chars)
		
		    return value

	```
	4. create_hashvalue get "seed" + "message" as parameters and use sha256() to do sha-256 calculation

	```python
		def create_hashvalue(message):
		    """Create hash value"""
	    	hash_obj = hashlib.sha256()
	    	hash_obj.update(message.encode("utf-8")) 
			# change message from string to bytes
	    	return hash_obj.hexdigest()
	```

	5. Define the handle function, first convert the received message from bytes to string and then replace the '\n' with '', store in msg and as paramter passed to server_work(). Finally, convet returned value msg2 which is string  to bytes by decode().

	```python
		class EchoHandler(BaseRequestHandler):
    		"""Process request"""
    		def handle(self):
        		""" The function to recv and send result"""
        		while True:
        	    	msg = self.request.recv(8192)
        	    	if not msg:
        	    	    break
        	    	msg = (msg.decode()).replace('\n', '') 
					#change message from bytes to string and remove '\n'
        	    	msg2 = server_work(msg)
        	    	self.request.send(msg2.encode()) 
					# change msg2 from string to bytes

	```
	6. In the main function, creating the 100 threads to process the connections from the sockets. And use try and except to catch KeyboardInterrupt(Control-C) to terminate the server.

	```python
		if __name__ == '__main__':
    		try:
        		NUMTHREADING = 100
        		SERV = TCPServer(('', 20000), EchoHandler)
        		for n in range(NUMTHREADING):
            		tSocket = Thread(target=SERV.serve_forever)
            		tSocket.daemon = True
            		tSocket.start()
        		SERV.serve_forever()
    		except KeyboardInterrupt:
            	print("ByeBye!")
            	SERV.server_close()
	```

## Result & Server performance

* Result under # of connection is 100: 
	Pow 難度 |     0    |     1     |    2    |   3    |   4   |   5   |   6   |
	---------|:--------:|:---------:|:-------:|:------:|:-----:|:-----:|:-----:|
	回覆數/秒| 42011.45 | 10581.717 | 523.083 | 37.667 | 2.383 | 0.167 |   0   |
	Error 104|  4 times |   3 times | 1 times |    0   |   0   |   0   |   0   |

* Result under # of connection is cpu.length: 
	Pow 難度 |     0    |     1     |    2    |   3    |   4   |   5   |   6   |
	---------|:--------:|:---------:|:-------:|:------:|:-----:|:-----:|:-----:|
	回覆數/秒| 36747.50 | 8906.117 | 497.017 |  28.7  | 2.850 | 0.200 |   0   |
	Error 104|  4 times |   3 times | 2 times |    0   |   0   |   0   |   0   |

* Server perfomance:
	- 這邊討論會影響 Server 效能的因素，在 client.js 中會根據 CPU 的核心數來 fork 出相同數量的 child，這邊每個child產生的連線數目跟CPU核心數相同。  
	- 但即便在多核心架構中，每顆CPU在一時間內只能處理一個process，一個 socket connection 視為一個thread ，而不同CPU同時處理thread的能力亦不同。  
	- 故 Server 的效能會受到 CPU core 的數量跟處理 thread 的能力影響。例如使用八核心的 CPU 的 Server 效能會比使用四核心的好；而同樣核心數的處理器下，8T(enable to process 8 threads) 的會比 4T(enable to process 4 threads)。
	- 再來，在這次的作業中， Server 處理回應的速度跟 PoW 運算的速度有很大的關係。計算PoW的速度愈快，能處理的 response 數量愈多，而由上表可知 PoW 難度愈低則 response 數量愈高，但若 client send request 的速度太快會造成 client 端的 connection reset
(request 寄送太快， Server 來不及處理造成)
	- 會影響PoW的運算速度，主要原因是因為 sha-256 實作方法，在這次作業中我採用的是內建於 python 函式庫 hashlib 的sha256()，加上前後有 bytes 跟 string 的型別轉換跟 random seed 的運算，都會影響 PoW 的運算速度，更不用說對於 PoW 的難度有額外的要求，難度愈高，則 Server 的處理時間愈長。


## Problem faced & Discussion

* Error 104 connection reset by user，當PoW 難度低於等於2時會出現這個問題，但不會造成 Server 出問題，應該是因為在Client 端提前結束，讓Server端認為這個是不正常的連線中斷。
