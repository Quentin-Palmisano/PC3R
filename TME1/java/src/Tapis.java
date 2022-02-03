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
		if(this.file.size()<this.capacity) this.file.add(p);
	}

	public synchronized Paquet depiler () {
		Paquet p = null;
		while(this.file.size()==0) {			
		}
		p = this.file.get(0);
		this.file.remove(0);
		return p;
	}

}
