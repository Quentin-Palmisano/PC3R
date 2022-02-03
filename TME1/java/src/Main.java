
public class Main {
	
	public Tapis tapis;
	public int cpt;
	
	public Main(int c, int cpt) {
		this.tapis=new Tapis(c);
		this.cpt = cpt;
	}
	
	public void createProducteur(String nom, int x) {
		int y = 1;
		while(y<=x) {
			Paquet p = new Paquet(nom+" "+y);
			tapis.empiler(p);
			y++;
		}
	}
	
	public void createConsommateur(int id) {
		while(this.cpt>0) {
			Paquet p = tapis.depiler();
			System.out.println("C"+id+" mange "+p.name);
			--this.cpt;
		}
	}
	
	
	public static void main(String[] args) {
		int n = 2;
		int x = 50;
		int m = 2;
		
		System.out.println("coucou");
		
		Main main = new Main(10, n*x);
		
		for(int i=0; i<n; i++) {
			final int j = i;
			new Thread(() -> {main.createProducteur("Produit"+j, x);}).start();		
		}
		
		for(int i=0; i<m; i++) {
			final int j = i;
			new Thread(() -> {main.createConsommateur(j);}).start();
		}
	}

}
