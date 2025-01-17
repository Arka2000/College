package dllp;
/**Data format:
 * 1 byte: Info
 * 1 byte: Sequence number
 * 1 - 7 bytes: data
 *
 * Info byte structrue:
 * 00000000: message
 * 10000000: ack 
 * 11111111: invalid frame
 *
 * */
class GNSenderClientClass extends ClientClass {
	/**Send and Wait data link layer protocol implementation */
	
	protected static final String SENDER_MAC_ADDR 		= "10101010";
	protected static final String RECEIVER_MAC_ADDR 	= "10101011";
	protected static final String MESSAGE_HEADER 		= "00000000";
	protected static final String AWK_HEADER 		= "10000000";
	protected static final String INVALID_FRAME 		= "11111111";

	protected String MSG;
	protected int EVENT_REQ_TO_SEND;
	protected boolean EVENT_TIMEOUT;
	protected Timer timer;
	protected String frames[];
	protected int sw = 3;
	protected int sf = 0;
	protected int sn = 0;
	
//--------------------------------------

	public class Timer extends Thread{
		private boolean running;
		private final static int TIME = 300;
		private int time;
		
		public Timer() {
			running = false;
			time = TIME;
		}

		public boolean isRunning() {return running;}

		public  void startTimer() {
			running = true;
			EVENT_TIMEOUT = false;
			time = TIME;
			resume();
		}
	
		public  void stopTimer() {
			try {
				time = TIME;
				running = false;
				suspend();
			} catch (Exception e) {
				e.printStackTrace();
				System.exit(0);
			}
		}

		public void run() {
			try {
				while(true) {
					while((time--)!=0) {
						sleep(1);
					}
					EVENT_TIMEOUT = true;
					timeout();
				}
			} catch (Exception e) {
				e.printStackTrace();
				System.exit(0);
			}
		}
	}
//--------------------------------------


	public GNSenderClientClass() {
		super();
		MSG = "000111";	
		EVENT_REQ_TO_SEND = 0;
		EVENT_TIMEOUT = false;
		timer = new Timer();
	}

	protected class FrameSendingRequester implements Runnable {
		public void run() {
			int iter = 10;
			while((iter--)!=0) {
				requestToSend();
			}
		}

		public synchronized void requestToSend() {
			EVENT_REQ_TO_SEND++;
		}
	}
	
	protected void printN(Integer ack) {
		if(ack!=null)
			System.out.println("sn:" + sn
				+ " sf:" + sf
				+ " sw:" + sw
				+ " ack:" + ack
				);
		else 
			
			System.out.println("sn:" + sn
				+ " sf:" + sf
				+ " sw:" + sw
				);
	}

	protected String makeSequenceString (int n) {
		String sq = Integer.toBinaryString(n);
		if(sq.length() > 8)
			sq = sq.substring(sq.length() - 8);
		else if(sq.length() < 8)
			while(sq.length()!=8)
				sq = "0" + sq;
		return sq;
	}

	protected void sendFrame(int sn) {
		System.out.println("Sending Message-"
				+ sn
				+ ": " 
				+ MSG 
				+ " to:"
				+ RECEIVER_MAC_ADDR);
		frames[sn] = MESSAGE_HEADER + makeSequenceString(sn) + MSG;
		sendMsg(frames[sn], RECEIVER_MAC_ADDR);
		EVENT_REQ_TO_SEND--;
	}

	protected void resendFrame(int sn) {
		System.out.println("Reseending Message-"
				+ sn
				+ ": " 
				+ MSG 
				+ " to:"
				+ RECEIVER_MAC_ADDR);
		frames[sn] = MESSAGE_HEADER + makeSequenceString(sn) + MSG;
		sendMsg(frames[sn], RECEIVER_MAC_ADDR);
		//EVENT_REQ_TO_SEND--;
	}

	public synchronized void run() {
		super.run(SENDER_MAC_ADDR);

		sw = 3;
		sf = 0;
		sn = 0;

		frames = new String[sw + 1];
		for(int i=0;i<sw + 1;i++)
			frames[i] = MSG;

		timer.start();
		//new Thread(new FrameSendingRequester()).start();

		EVENT_REQ_TO_SEND = 100;

		while(true) {
			try {
				if(EVENT_REQ_TO_SEND!=0) {
					int nf = sn - sf;
					if(nf<0) nf = nf + sw + 1;
					if(nf < sw){
						sendFrame(sn);
						sn = (sn + 1)%(sw + 1);
						//Thread.sleep(30);
						timer.startTimer();
					} else {
						wait();
					}
				}

				/*if(EVENT_TIMEOUT) {
					wait();
				}*/

			} catch (Exception e) {
				e.printStackTrace();
				System.exit(0);
			}
		}
	}

	protected boolean checkValidAck(int ackNo) {
		int f=-1,n=-1,w=-1,shift=-1;
		int sack = ackNo;
		if(sf<sn)
			if(ackNo>sf && ackNo<=sn)
				return true;
		if(sf==sn)
			return true;
		if(sf>sn) {
			f = sf;
			n = sn;
			w = sw + 1;
			shift = w - f;
			f = (f+shift)%w;
			n = (n+shift)%w;
			ackNo = (ackNo + shift)%w;
			if(ackNo>f && ackNo<=n)
				return true;
		}
		//printN(ackNo);
		//System.out.print(sf + " " + sn + " " + sack + " ");
		//System.out.print(ackNo>f);
		//System.out.println(ackNo<=n);

		return false;
	}

	@Override
	protected synchronized void receiveMsg(String msg) {
		String dest_mac = msg.substring(0,8);
		String source_mac = msg.substring(8,16);
		String info = msg.substring(16,24);
		if(info.equals(AWK_HEADER)) {
			int ackNO = Integer.parseInt(msg.substring(24,32),2);
			//printN(ackNO);
			if(checkValidAck(ackNO)) { //problem here
				notifyAll();
				while(sf != ackNO){
					System.out.println("Ack-" 
						+ ackNO 
						+ " received from:" 
						+ source_mac + " sf:" + sf + " sn:" + sn);
					//printN(ackNO);
					//frame[sf] = INVALID_FRAME;
					sf = (sf + 1) % (sw + 1);
					//System.out.println("sf:" + sf + " sn:" + sn);
				} 
				//printN(null);
				timer.stopTimer();
			} else {
				System.out.println("Invalid Ack received with Ack no:"
						+ ackNO);
			}	
		}
	}
	
	public synchronized void timeout() {
		System.out.println("Timeout sf:" + sf + " sn:" + sn);
		timer.startTimer();
		//System.out.println("sn:" + sn
		//	+ " sf:" + sf
		//	+ " sw:" + sw
		//	);
		int temp = sf;
		while(temp != sn) {
			resendFrame(temp);
			temp = (temp + 1) % (sw + 1);
		}
		notifyAll();
	}
}
