package dllp;
/**Data format:
 * 1 byte: Info
 * 1 - 8 bytes: data
 *
 * Info byte structrue:
 * 00000000: message
 * 10000000: ack 
 * */
class SAWSenderClientClass extends ClientClass {
	/**Send and Wait data link layer protocol implementation */
	
	protected static final String SENDER_MAC_ADDR 		= "10101010";
	protected static final String RECEIVER_MAC_ADDR 	= "10101011";
	protected static final String MESSAGE_HEADER 		= "00000000";
	protected static final String AWK_HEADER 		= "10000000";
	
	protected Boolean canSend;
	protected String MSG;

	public SAWSenderClientClass() {
		super();
		canSend = true;
		MSG = "000111";
	}
	

	public synchronized void run() {
		super.run(SENDER_MAC_ADDR);
		int iter = 10;
		while((iter)!=0) {
			if(canSend) {
				try {
					System.out.println("Sending message: " 
							+ MSG 
							+ " to:"
						       	+ RECEIVER_MAC_ADDR);
					sendMsg(MESSAGE_HEADER + MSG, RECEIVER_MAC_ADDR);
					canSend = false;
					iter--;
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
					System.out.println();
				}
			}
		}
	}

	@Override
	protected synchronized void receiveMsg(String msg) {
		String dest_mac = msg.substring(0,8);
		String source_mac = msg.substring(8,16);
		String info = msg.substring(16,24);
		if(info.equals(AWK_HEADER)) {
			System.out.println("Ack received from:" + source_mac);
			canSend = true;
			notify();
		}
	}
}
