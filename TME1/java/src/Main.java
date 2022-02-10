
public class Main {
	
	public Tapis tapis;
	public volatile int cpt;
	
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
		
		//NOMBRE DE THREAD PRODUCTEUR
		int n = 5;
		
		//CIBLE DE RPODUCTION
		int x = 55;
		
		//NOMBRE DE THREAD CONSOMATEUR
		int m = 5;
		
		
		Main main = new Main(20, n*x);
		
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
