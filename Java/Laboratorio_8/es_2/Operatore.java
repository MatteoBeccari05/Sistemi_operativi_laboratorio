public class Operatore extends Thread 
{
	private Magazzino mag = null;
    
    public Operatore(Magazzino mag)
	{
        this.mag = mag;
    }

    @Override
    public void run() 
	{
		boolean exist = mag.verificaOggetto("bulloni");
		if (!exist) 
		{
			System.out.println(Thread.currentThread().threadId() + ": in magazzino non esiste l'oggetto bulloni, lo creo");
            mag.aggiungiOggetto("bulloni");
        
			// eventuale pausa
			Simulate.Break();
		}
		
		System.out.println(Thread.currentThread().threadId() + ": aggiungo 1000 pezzi di bulloni");
		mag.increaseQuantity("bulloni", 1000);
		
		// eventuale pausa
		Simulate.Break();
		
		System.out.println(Thread.currentThread().threadId() + ": rimuovo 500 pezzi di bulloni");
		mag.increaseQuantity("bulloni", -500);
    }
}