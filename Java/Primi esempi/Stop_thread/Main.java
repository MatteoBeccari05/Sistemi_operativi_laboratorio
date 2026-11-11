public class Main 
{
    public static void main(String[] args) 
    {
        EsempioStopSicuro processo = new EsempioStopSicuro();
        
        System.out.println("Avvio dell'attività...");
        processo.start();
        
        try 
        {
            // Il thread principale (main) attende 2 secondi 
            // lasciando lavorare il "Thread-Lavoratore"
            Thread.sleep(2000);
        } 
        catch (InterruptedException e) {}
        
        // Il main richiede la terminazione del thread figlio
        processo.stop();
    }
}