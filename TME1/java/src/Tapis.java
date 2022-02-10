import java.util.ArrayList;

public class Tapis {

	public int capacity;
	public ArrayList<Paquet> file;

	public Tapis(int c) {
		this.capacity=c;
		file = new ArrayList<Paquet>();
		file.ensureCapacity(c);
	}

	public synchronized void empiler (Paquet p) {
		while(this.file.size()==this.capacity) {
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		this.file.add(p);
		this.notify();
	}

	public synchronized Paquet depiler () {
		Paquet p = null;
		while(this.file.size()==0) {		
			try {
				this.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		p = this.file.get(0);
		this.file.remove(0);
		this.notify();
		return p;
	}

}
