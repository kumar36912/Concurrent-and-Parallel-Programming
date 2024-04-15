/* Thread synchronization mechanisms: code examples */

/** C example **/
static pthread_mutex_t lock;  /* declare a lock */              /** line 1 **/
void increment_n() {                                            /** line 2 **/
    pthread_mutex_lock(&lock);                                  /** line 3 **/
    n = n + 1;  /* critical section code */                     /** line 4 **/
    pthread_mutex_unlock(&lock);                                /** line 5 **/
}

/** Java example **/
class Test {
    static int n; // a single, shared storage location          /** line 6 **/
    static Object lock = new Object(); // can't be null         /** line 7 **/
    void incrementN() {
	synchronized(lock) {                                    /** line 8 **/
	    n = n + 1; /* critical section code */              /** line 9 **/
	}                                                       /** line 10 **/
    }
}
