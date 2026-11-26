public class Main 
{
    final static int N = 5;
	public static void main(String[] args) 
    {
        Magazzino mag = new Magazzino();
        
		// creo e avvio N operatori
        Operatore operatori[] = new Operatore[N];
		for (int i = 0; i < N; i++) 
        {
            operatori[i] = new Operatore(mag);
            operatori[i].start();
        }
		
		// attendo la terminazione degli N operatori
        for (int i = 0; i < N; i++) 
        {
            try 
            {
                operatori[i].join();
            } 
            catch(InterruptedException e){}
        }
		
		// stampo lo stato finale del magazzino
        System.out.println("Magazzino:");
        System.out.println(""+mag);
    }
}