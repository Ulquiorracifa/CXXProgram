## 无锁编程






#####  memory_order 
		上述 6 中访存次序(内存序)分为 3 类，顺序一致性模型(std::memory_order_seq_cst)，Acquire-Release 模型(std::memory_order_consume, std::memory_order_acquire, std::memory_order_release, std::memory_order_acq_rel,) （获取/释放语义模型）和 Relax 模型(std::memory_order_relaxed)（宽松的内存序列化模型）。
		
		memory_order_relaxed: 只保证当前操作的原子性，不考虑线程间的同步，其他线程可能读到新值，也可能读到旧值。比如 C++ shared_ptr 里的引用计数，我们只关心当前的应用数量，而不关心谁在引用谁在解引用。
		
		memory_order_release:
		1.对写入施加 release 语义（store），在代码中这条语句前面的所有读写操作都无法被重排到这个操作之后，即 store-store 不能重排为 store-store, load-store 也无法重排为 store-load
		2.当前线程内的所有写操作，对于其他对这个原子变量进行 acquire 的线程可见
		3.当前线程内的与这块内存有关的所有写操作，对于其他对这个原子变量进行 consume 的线程可见
		
		memory_order_acquire:
		1.对读取施加 acquire 语义（load），在代码中这条语句后面所有读写操作都无法重排到这个操作之前，即 load-store 不能重排为 store-load, load-load 也无法重排为 load-load
		2.在这个原子变量上施加 release 语义的操作发生之后，acquire 可以保证读到所有在 release 前发生的写入，举个例子：
	        c = 0;
	        thread 1:{ 
	            a = 1; 
	            b.store(2, memory_order_relaxed); 
	            c.store(3, memory_order_release);
	        }
	        thread 2:{ 
	            while (8) ; // 以下 assert 永远不会失败 
	            assert(a == 1 && b == 2);
	             assert(b.load(memory_order_relaxed) == 2);
	        }
	
		　memory_order_consume:
		　1.对当前要读取的内存施加 release 语义（store），在代码中这条语句后面所有与这块内存有关的读写操作都无法被重排到这个操作之前
		　2.在这个原子变量上施加 release 语义的操作发生之后，acquire 可以保证读到所有在 release 前发生的并且与这块内存有关的写入，举个例子：
			a = 0;
	        c = 0;
	        thread 1:{
	            a = 1; 
	            c.store(3, memory_order_release);
	        }
	        thread 2:{ 
	            while (c.load(memory_order_consume) != 3) ; 
	            assert(a == 1); // assert 可能失败也可能不失败
	        }
	        
	       memory_order_acq_rel:
			1.对读取和写入施加 acquire-release 语义，无法被重排
			2.可以看见其他线程施加 release 语义的所有写入，同时自己的 release 结束后所有写入对其他施加 acquire 语义的线程可见
			
			memory_order_seq_cst:（顺序一致性）
			1.如果是读取就是 acquire 语义，如果是写入就是 release 语义，如果是读取+写入就是 acquire-release 语义
			2.同时会对所有使用此 memory order 的原子操作进行同步，所有线程看到的内存操作的顺序都是一样的，就像单个线程在执行所有线程的指令一样
