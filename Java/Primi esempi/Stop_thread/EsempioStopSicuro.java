public class EsempioStopSicuro implements Runnable 
{
    
    // La keyword volatile garantisce che il valore non venga memorizzato 
    // nella cache locale del thread: le letture/scritture vanno dritte in memoria centrale
    private volatile Thread blinker;

    // Metodo per avviare il thread
    public void start() 
    {
        blinker = new Thread(this, "Thread-Lavoratore");
        blinker.start();
    }

    // Metodo sicuro per richiedere la terminazione
    public void stop() 
    {
        System.out.println("Inviata richiesta di stop dal " + Thread.currentThread().getName());
        blinker = null;
    }

    // Il codice effettivamente eseguito dal thread
    public void run() 
    {
        // Continua a ciclare finché blinker non viene impostato a null da un altro thread
        while (blinker != null)
        {
            System.out.println(Thread.currentThread().getName() + " sta lavorando...");
            try 
            {
                // Simula un'operazione e attende mezzo secondo
                Thread.sleep(500);
            } 
            catch (InterruptedException e) 
            { 
                // Gestione di un'eventuale interruzione forzata
            }
        }
        
        // Uscito dal ciclo, il thread può chiudere le risorse prima di morire
        System.out.println(Thread.currentThread().getName() + " ha terminato in modo sicuro.");
    }
}